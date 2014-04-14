#include "test.h"

#include "speck.h"

#include <algorithm>
#include <iostream>
#include <iterator>
#include <chrono>

using namespace std;

int digit_value(char c) {
    if('0' <= c and c <= '9') {
        return c - '0';
    }
    if('a' <= c and c <= 'f') {
        return (c - 'a') + 10;
    }
    if('A' <= c and c <= 'F') {
        return (c - 'A') + 10;
    }
    return -1;
}

uberzahl from_hex(const string& hexString) {
    uberzahl result("0");
    for(char digit : hexString) {
        int value = digit_value(digit);
        if(value == -1) {
            continue;
        }
        result = result << 4;
        result = result | value;
    }
    return result;
}

vector<uberzahl> from_hex(const vector<string>& hexStrings) {
    vector<uberzahl> result;
    for(const string& hs : hexStrings) {
        result.push_back(from_hex(hs));
    }
    return result;
}

TestParams::TestParams(short blockSize_,
                       short keySize_,
                       const string& key_,
                       const string& plaintext_,
                       const string& ciphertext_)
      : blockSize{blockSize_},
        keySize{keySize_},
        key{from_hex(key_)},
        plaintext{from_hex(plaintext_)},
        ciphertext{from_hex(ciphertext_)} {

}

template<typename T>
void print(const std::vector<T>& vec) {
    copy(vec.begin(), vec.end(), ostream_iterator<T>(cout, " "));
    cout << endl;
}

bool run_test(const TestParams& params) {
    speck cipher(params.blockSize, params.keySize, params.key);
    auto encrypted = cipher.encrypt_block(params.plaintext);
    auto decrypted = cipher.decrypt_block(params.ciphertext);
    return encrypted == params.ciphertext && decrypted == params.plaintext;
}

void run_all_tests() {
    int index = 1;
    for(const TestParams& params : kTests) {
        cout << "Test " << index << " passes: " << boolalpha << run_test(params) << endl;
        index = index + 1;
    }
}

double run_timed_test(short blockSize, short keySize) {
    static int trials = 50;
    using clock = chrono::high_resolution_clock;
    double total = 0;
    for(int i = 0; i < trials; i++) {
        speck cipher(blockSize, keySize);
        uberzahl plaintext = uberzahl("0").random(blockSize);
        auto start = clock::now();
        cipher.encrypt_block(plaintext);
        auto end = clock::now();
        double elapsed = chrono::duration_cast<chrono::duration<double>>(end - start).count();
        total += elapsed;
    }
    return total / trials * 1000; // Conver to milliseconds
}

void run_all_timed_tests() {
    cout << "Block\tTime(ms)" << endl;
    for(auto& entry : kBlockKeySizes) {
        double elapsed = run_timed_test(entry.first, entry.second);
        cout << entry.first << "\t" << elapsed << endl;
    }
}
