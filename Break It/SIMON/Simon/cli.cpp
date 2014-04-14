#include <getopt.h>
#include <string>
#include <iostream>
#include <chrono>
#include <stdlib.h>
#include <gmpxx.h>
#include <exception>

#include "generator.hpp"
#include "simon.hpp"

using std::string;
using std::stoi;
using std::cout;
using std::cerr;
using std::endl;

const unsigned IO_BASE = 10;

void keygen(int argc, char* argv[]);
void encrypt(int argc, char* argv[]);
void decrypt(int argc, char* argv[]);

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Please specify the mode to run this program in.  Choose one of 'keygen', 'encrypt', or 'decrypt'.  Run the program with the command line input:\n./program <mode>" << endl;
        exit(1);
    }
    std::string mode(argv[1]);

    if (mode == "keygen") {
        keygen(argc, argv);
    } else if (mode == "encrypt") {
        encrypt(argc, argv);
    } else if (mode == "decrypt") {
        decrypt(argc, argv);
    } else {
        cerr << mode << " is an invalid mode.  Please choose one of 'keygen', 'encrypt', and 'decrypt'." << endl;
    }
}

void keygen(int argc, char* argv[]) {
    if (argc != 3) {
        cerr << "'keygen' requires a single argument, the key size to generate.\n./program keygen <key-size>" << endl;
        exit(1);
    }

    int key_size = atoi(argv[2]);
    if (key_size <= 0) {
        cerr << "The key size must be a positive number." << endl;
        exit(1);
    }

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    KeyGenerator gen(key_size, seed);
    mpz_class key = gen();
    cout << key.get_str(IO_BASE) << endl;
}

void encrypt(int argc, char* argv[]) {
    if (argc != 6) {
        cerr << "'encrypt' requires 4 arguments, the block size, the key size, the key, and the plaintext.  The key and plaintext must be in base "
             << IO_BASE << ".\n" << "./program encrypt <block-size> <key-size> <key> <plaintext>" << endl;
             exit(1);
    }

    int block_size = atoi(argv[2]),
        key_size = atoi(argv[3]);
    
    mpz_class key, plaintext;
    key.set_str(argv[4], IO_BASE);
    plaintext.set_str(argv[5], IO_BASE);

    try {
        Simon cipher(block_size, key_size);
        mpz_class ciphertext = cipher.encrypt(key, plaintext);
        cout << ciphertext.get_str(IO_BASE) << endl;
    } catch (std::invalid_argument e) {
        cerr << e.what() << endl;
        exit(1);
    }
}

void decrypt(int argc, char* argv[]) {
    if (argc != 6) {
        cerr << "'decrypt' requires 4 arguments, the block size, the key size, the key, and the ciphertext.  The key and ciphertext must be in base "
             << IO_BASE << ".\n" << "./program encrypt <block-size> <key-size> <key> <ciphertext>" << endl;
             exit(1);
    }
    int block_size = atoi(argv[2]),
        key_size = atoi(argv[3]);

    mpz_class key, ciphertext;
    key.set_str(argv[4], IO_BASE);
    ciphertext.set_str(argv[5], IO_BASE);

    try {
        Simon cipher(block_size, key_size);
        mpz_class plaintext = cipher.decrypt(key, ciphertext);
        cout << plaintext.get_str(IO_BASE) << endl;
    } catch (std::invalid_argument e) {
        cerr << e.what() << endl;
        exit(1);
    }
}
