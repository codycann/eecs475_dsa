#include <iostream>
#include <chrono>

#include "simon.hpp"
#include "generator.hpp"

using std::cout;
using std::endl;

/**
 * @brief Class used for plaintext generation for the Simon cipher.
 */
class BlockGenerator {
    /**
     * @var The random number generator using Mersenne Twister.
     */
    std::mt19937 generator;
    /**
     * @var The uniform distribution to use for generating 0s and 1s.
     */
    std::uniform_int_distribution<char> distr;
    /**
     * @var The number of bits to make the generated block.
     */
    unsigned block_size;
public:
    /**
     * @brief Constructs the BlockGenerator class from a seed.
     */
    BlockGenerator(unsigned size, unsigned seed)
            : block_size(size), generator(seed), distr('0', '1') {}

    /**
     * @brief Returns a pseudorandomly generated key of BlockSize bits.
     */
    mpz_class operator() () {
        std::string binary_string;
        for (unsigned i = 0; i < block_size; ++i) {
            binary_string.push_back(distr(generator));
        }
        mpz_class result;
        result.set_str(binary_string, 2);
        return result;
    }
};

int main() {
    mpz_class plaintext, ciphertext, ciphertext_test, key;
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();

    Simon s0(32, 64);
    key.set_str("1918111009080100", 16);
    plaintext.set_str("65656877", 16);
    ciphertext_test = s0.encrypt(key, plaintext);
    ciphertext.set_str("c69be9bb", 16);
    if (ciphertext != ciphertext_test) {
        cout << "Test 0 failed to encrypt the correct ciphertext." << endl;
    } else if (plaintext != s0.decrypt(key, ciphertext)) {
        cout << "Test 0 failed to decrypt the correct plaintext." << endl;
    } else {
        cout << "Test 0 passed." << endl;
    }
    BlockGenerator genB32(32, seed);
    KeyGenerator genK64(64, seed);
    for (unsigned i = 0; i < 10000; ++i) {
        key = genK64();
        plaintext = genB32();
        if (plaintext != s0.decrypt(key, s0.encrypt(key, plaintext))) {
            cout << "Simon<32, 64> failed." << endl;
        }
    }


    Simon s1(48, 72);
    key.set_str("1211100a0908020100", 16);
    plaintext.set_str("6120676e696c", 16);
    ciphertext_test = s1.encrypt(key, plaintext);
    ciphertext.set_str("dae5ac292cac", 16);
    if (ciphertext != ciphertext_test) {
        cout << "Test 1 failed to encrypt the correct ciphertext." << endl;
    } else if (plaintext != s1.decrypt(key, ciphertext)) {
        cout << "Test 1 failed to decrypt the correct plaintext." << endl;
    } else {
        cout << "Test 1 passed." << endl;
    }
    BlockGenerator genB48(48, seed);
    KeyGenerator genK72(72, seed);
    for (unsigned i = 0; i < 10000; ++i) {
        key = genK72();
        plaintext = genB48();
        if (plaintext != s1.decrypt(key, s1.encrypt(key, plaintext))) {
            cout << "Simon<48, 72> failed." << endl;
        }
    }


    Simon s2(48, 96);
    key.set_str("1a19181211100a0908020100", 16);
    plaintext.set_str("72696320646e", 16);
    ciphertext_test = s2.encrypt(key, plaintext);
    ciphertext.set_str("6e06a5acf156", 16);
    if (ciphertext != ciphertext_test) {
        cout << "Test 2 failed to encrypt the correct ciphertext." << endl;
    } else if (plaintext != s2.decrypt(key, ciphertext)) {
        cout << "Test 2 failed to decrypt the correct plaintext." << endl;
    } else {
        cout << "Test 2 passed." << endl;
    }
    KeyGenerator genK96(96, seed);
    for (unsigned i = 0; i < 10000; ++i) {
        key = genK96();
        plaintext = genB48();
        if (plaintext != s2.decrypt(key, s2.encrypt(key, plaintext))) {
            cout << "Simon<48, 96> failed." << endl;
        }
    }


    Simon s3(64, 96);
    key.set_str("131211100b0a090803020100", 16);
    plaintext.set_str("6f7220676e696c63", 16);
    ciphertext_test = s3.encrypt(key, plaintext);
    ciphertext.set_str("5ca2e27f111a8fc8", 16);
    if (ciphertext != ciphertext_test) {
        cout << "Test 3 failed to encrypt the correct ciphertext." << endl;
    } else if (plaintext != s3.decrypt(key, ciphertext)) {
        cout << "Test 3 failed to decrypt the correct plaintext." << endl;
    } else {
        cout << "Test 3 passed." << endl;
    }
    BlockGenerator genB64(64, seed);
    for (unsigned i = 0; i < 10000; ++i) {
        key = genK96();
        plaintext = genB64();
        if (plaintext != s3.decrypt(key, s3.encrypt(key, plaintext))) {
            cout << "Simon<64, 96> failed." << endl;
        }
    }


    Simon s4(64, 128);
    key.set_str("1b1a1918131211100b0a090803020100", 16);
    plaintext.set_str("656b696c20646e75", 16);
    ciphertext_test = s4.encrypt(key, plaintext);
    ciphertext.set_str("44c8fc20b9dfa07a", 16);
    if (ciphertext != ciphertext_test) {
        cout << "Test 4 failed to encrypt the correct ciphertext." << endl;
    } else if (plaintext != s4.decrypt(key, ciphertext)) {
        cout << "Test 4 failed to decrypt the correct plaintext." << endl;
    } else {
        cout << "Test 4 passed." << endl;
    }
    KeyGenerator genK128(128, seed);
    for (unsigned i = 0; i < 10000; ++i) {
        key = genK128();
        plaintext = genB64();
        if (plaintext != s4.decrypt(key, s4.encrypt(key, plaintext))) {
            cout << "Simon<64, 128> failed." << endl;
        }
    }


    Simon s5(96, 96);
    key.set_str("0d0c0b0a0908050403020100", 16);
    plaintext.set_str("2072616c6c69702065687420", 16);;
    ciphertext_test = s5.encrypt(key, plaintext);
    ciphertext.set_str("602807a462b469063d8ff082", 16);
    if (ciphertext != ciphertext_test) {
        cout << "Test 5 failed to encrypt the correct ciphertext." << endl;
    } else if (plaintext != s5.decrypt(key, ciphertext)) {
        cout << "Test 5 failed to decrypt the correct plaintext." << endl;
    } else {
        cout << "Test 5 passed." << endl;
    }
    BlockGenerator genB96(96, seed);
    for (unsigned i = 0; i < 10000; ++i) {
        key = genK96();
        plaintext = genB96();
        if (plaintext != s5.decrypt(key, s5.encrypt(key, plaintext))) {
            cout << "Simon<96, 96> failed." << endl;
        }
    }

    Simon s6(96, 144);
    key.set_str("1514131211100d0c0b0a0908050403020100", 16);
    plaintext.set_str("74616874207473756420666f", 16);
    ciphertext_test = s6.encrypt(key, plaintext);
    ciphertext.set_str("ecad1c6c451e3f59c5db1ae9", 16);
    if (ciphertext != ciphertext_test) {
        cout << "Test 6 failed to encrypt the correct ciphertext." << endl;
    } else if (plaintext != s6.decrypt(key, ciphertext)) {
        cout << "Test 6 failed to decrypt the correct plaintext." << endl;
    } else {
        cout << "Test 6 passed." << endl;
    }
    KeyGenerator genK144(144, seed);
    for (unsigned i = 0; i < 10000; ++i) {
        key = genK144();
        plaintext = genB96();
        if (plaintext != s6.decrypt(key, s6.encrypt(key, plaintext))) {
            cout << "Simon<96, 144> failed." << endl;
        }
    }


    Simon s7(128, 128);
    key.set_str("0f0e0d0c0b0a09080706050403020100", 16);
    plaintext.set_str("63736564207372656c6c657661727420", 16);
    ciphertext_test = s7.encrypt(key, plaintext);
    ciphertext.set_str("49681b1e1e54fe3f65aa832af84e0bbc", 16);
    if (ciphertext != ciphertext_test) {
        cout << "Test 7 failed to encrypt the correct ciphertext." << endl;
    } else if (plaintext != s7.decrypt(key, ciphertext)) {
        cout << "Test 7 failed to decrypt the correct plaintext." << endl;
    } else {
        cout << "Test 7 passed." << endl;
    }
    BlockGenerator genB128(128, seed);
    for (unsigned i = 0; i < 10000; ++i) {
        key = genK128();
        plaintext = genB128();
        if (plaintext != s7.decrypt(key, s7.encrypt(key, plaintext))) {
            cout << "Simon<128, 128> failed." << endl;
        }
    }


    Simon s8(128, 192);
    key.set_str("17161514131211100f0e0d0c0b0a09080706050403020100", 16);
    plaintext.set_str("206572656874206e6568772065626972", 16);
    ciphertext_test = s8.encrypt(key, plaintext);
    ciphertext.set_str("c4ac61effcdc0d4f6c9c8d6e2597b85b", 16);
    if (ciphertext != ciphertext_test) {
        cout << "Test 8 failed to encrypt the correct ciphertext." << endl;
    } else if (plaintext != s8.decrypt(key, ciphertext)) {
        cout << "Test 8 failed to decrypt the correct plaintext." << endl;
    } else {
        cout << "Test 8 passed." << endl;
    }
    KeyGenerator genK192(192, seed);
    for (unsigned i = 0; i < 10000; ++i) {
        key = genK192();
        plaintext = genB128();
        if (plaintext != s8.decrypt(key, s8.encrypt(key, plaintext))) {
            cout << "Simon<128, 192> failed." << endl;
        }
    }


    Simon s9(128, 256);
    key.set_str("1f1e1d1c1b1a191817161514131211100f0e0d0c0b0a09080706050403020100", 16);
    plaintext.set_str("74206e69206d6f6f6d69732061207369", 16);
    ciphertext_test = s9.encrypt(key, plaintext);
    ciphertext.set_str("8d2b5579afc8a3a03bf72a87efe7b868", 16);
    if (ciphertext != ciphertext_test) {
        cout << "Test 9 failed to encrypt the correct ciphertext." << endl;
    } else if (plaintext != s9.decrypt(key, ciphertext)) {
        cout << "Test 9 failed to decrypt the correct plaintext." << endl;
    } else {
        cout << "Test 9 passed." << endl;
    }
    KeyGenerator genK256(256, seed);
    for (unsigned i = 0; i < 10000; ++i) {
        key = genK256();
        plaintext = genB128();
        if (plaintext != s9.decrypt(key, s9.encrypt(key, plaintext))) {
            cout << "Simon<128, 256> failed." << endl;
        }
    }

}
