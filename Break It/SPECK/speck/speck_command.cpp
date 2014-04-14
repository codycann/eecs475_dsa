#include <unistd.h>
#include <getopt.h>
#include <cstdio>
#include <cassert>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <exception>
#include <limits>
#include <algorithm>
#include <string>
#include <sstream>
#include <iomanip>
#include "./speck.h"
#include "./tlspadding.h"
#include "./ecb.h"
#include "./unixerror.h"

using namespace crypto;
using namespace std;

#include "./speck_getopt.h"

static vector<int> validKeyBitSizes = {
    64, 72, 96, 128, 144, 192, 256
};

static vector<uint8_t> a2b_hex(string hexstr, int bits) {
    if (hexstr.find("0x") == 0) {
        hexstr = hexstr.substr(2);
    }
    transform(hexstr.begin(), hexstr.end(), hexstr.begin(), ::tolower);
    if ((int) hexstr.size() * 4 > bits) {
        throw runtime_error(string("hex string ") + hexstr + " too long (" +
            to_string(hexstr.size() * 4) + " > " + to_string(bits) + ")");
    }

    size_t digits = bits / 4;

    while (hexstr.size() < digits) {
        hexstr = '0' + hexstr;
    }

    const char* str = hexstr.c_str();
    vector<uint8_t> result;
    result.reserve(bits / 8);
    int temp = 0;

    for (size_t i = 0; i < hexstr.size(); i += 2) {
        if (sscanf(str + i, "%02x", &temp) != 1) {
            throw std::runtime_error(string("invalid hex string ") + hexstr);
        }

        result.push_back(temp);
    }

    assert((int) result.size() == bits / 8);
    return result;
}

static string b2a_hex(const vector<uint8_t>& data, int bits) {
    ostringstream result;
    assert((int) data.size() == bits / 8);
    for (auto b : data) {
        result << hex << setw(2) << setfill('0') << (int) b;
    }
    return result.str();
}

static vector<uint8_t> a2b_str(string str, int bits) {
    if ((int) str.size() * 8 > bits) {
        throw runtime_error(string("string ") + str + " too long (" +
            to_string(str.size() * 8) + " > " + to_string(bits) + ")");
    }
    if ((int) str.size() * 8 < bits) {
        cerr << "Warning: string " << str
             << " too short. Will be padded with null bytes on the right"
             << endl;
    }

    size_t digits = bits / 8;

    while (str.size() < digits) {
        str.push_back('\0');
    }
    vector<uint8_t> result(str.size());
    copy(str.begin(), str.end(), result.begin());
    return result;
}

static string b2a_str(const vector<uint8_t>& data, int bits) {
    assert((int) data.size() == bits / 8);
    string result;
    result.append((char*) data.data());
    return result;
}

static vector<uint8_t> keygen(int keySize) {
    if (find(validKeyBitSizes.begin(), validKeyBitSizes.end(), keySize) ==
        validKeyBitSizes.end()) {
        throw runtime_error("invalid key size");
    }

    keySize /= 8;

    vector<uint8_t> key;
    key.resize(keySize);
    FILE* rndfile = fopen("/dev/urandom", "rb");
    if (!rndfile) {
        throw unix_error("cannot open /dev/urandom; are you on a POSIX system?");
    }

    // all or nothing guarantee
    if (fread(key.data(), keySize, 1, rndfile) == 0) {
        throw unix_error();
    }

    fclose(rndfile);
    return key;  // hopefully RVO optimized
}

template <class stream_t> static void writeAllStdin(stream_t& stream) {
    uint8_t buffer[4096];
    while(true) {
        size_t read = fread(buffer, 1, sizeof(buffer), stdin);
        if (read == 0) {
            break;
        }

        stream.write(buffer, read);
    }
}

namespace {
    class StdoutWriter {
     public:
        void write(const uint8_t* data, size_t size) {
            if (fwrite(data, size, 1, stdout) != 1) {
                throw runtime_error("write to stdout failed");
            }
        }

        void close() {
            fflush(stdout);
        }
    };
}

template <class cipher_t>
static void mainInternal(cipher_t cipher, const Options& options) {
    if (options.command == "encrypt" || options.command == "decrypt") {
        if (options.positionals.size() < 2) {
            throw runtime_error(string("insufficient arguments for " + options.command));
        }
        vector<uint8_t> key = a2b_hex(options.positionals.at(0), cipher.getKeyBitSize());
        vector<uint8_t> result(cipher.getBlockSize());
        cipher.setKey(key.data());

        if (options.command == "encrypt") {
            vector<uint8_t> pt;
            if (options.hex) {
                pt = a2b_hex(options.positionals.at(1), cipher.getBlockBitSize());
            } else {
                pt = a2b_str(options.positionals.at(1), cipher.getBlockBitSize());
            }
            cipher.encryptBlock(pt.data(), result.data());

            cout << b2a_hex(result, cipher.getBlockBitSize()) << endl;
        } else {
            vector<uint8_t> ct = a2b_hex(options.positionals.at(1), cipher.getBlockBitSize());
            cipher.decryptBlock(ct.data(), result.data());

            if (options.hex) {
                cout << b2a_hex(result, cipher.getBlockBitSize()) << endl;
            } else {
                cout << b2a_str(result, cipher.getBlockBitSize()) << endl;
            }
        }
    } else if (options.command == "keygen") {
        int keySize = 256;
        if (options.positionals.size() > 0) {
            keySize = atoi(options.positionals.at(0).c_str());
        }

        cout << b2a_hex(keygen(keySize), keySize) << endl;
    } else if (options.command == "encrypt-stream" || options.command == "decrypt-stream") {
        if (freopen(nullptr, "rb", stdin) == nullptr || freopen(nullptr, "wb", stdout) == nullptr) {
            throw std::runtime_error("failed to open stdin/stdout as binary");
        }
        if (options.positionals.size() < 1) {
            throw runtime_error(string("insufficient arguments for ") + options.command);
        }

        cerr << "Warning: you are using Speck in the insecure ECB mode." << endl;

        vector<uint8_t> key = a2b_hex(options.positionals.at(0), cipher.getKeyBitSize());
        cipher.setKey(key.data());

        ECBMode mode;

        if (options.command == "encrypt-stream") {
            mode = ECBMode::Encrypt;
        } else {
            mode = ECBMode::Decrypt;
        }

        StdoutWriter writer;

        if (options.tlsecb) {
            if (options.command == "encrypt-stream") {
                ECB<StdoutWriter, cipher_t> ecb(writer, cipher, mode);
                TLSPadding<ECB<StdoutWriter, cipher_t>, cipher_t> padding(ecb, cipher);
                writeAllStdin(padding);
                padding.close();
            } else {
                TLSDepadding<StdoutWriter, cipher_t> padding(writer, cipher);
                ECB<TLSDepadding<StdoutWriter, cipher_t>, cipher_t> ecb(padding, cipher, mode);
                writeAllStdin(ecb);
                ecb.close();
            }
        } else {
            ECB<StdoutWriter, cipher_t> ecb(writer, cipher, mode);
            writeAllStdin(ecb);
            ecb.close();
        }
    }
}

int main(int argc, char* argv[]) {
    Options options = parseOptions(argc, argv);

    try {
        Speck128 cipher(options.blockSize, options.keySize);
        mainInternal(cipher, options);
    } catch (runtime_error e) {
        cerr << argv[0] << " error: " << e.what() << endl;
        return 1;
    }

    return 0;
}