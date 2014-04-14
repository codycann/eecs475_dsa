#ifndef SPECK_TEST_H
#define SPECK_TEST_H

#include "speck.h"

uberzahl from_hex(const std::string& hexString);
std::vector<uberzahl> from_hex(const std::vector<std::string>& hexStrings);

struct TestParams {
    TestParams(short blockSize_,
               short keySize_,
               const std::string& key_,
               const std::string& plaintext_,
               const std::string& ciphertext_);
    short blockSize;
    short keySize;
    uberzahl key;
    uberzahl plaintext;
    uberzahl ciphertext;
};

const std::vector<TestParams> kTests {
    {32, 64,
     "1918 1110 0908 0100", // key
     "6574 694c",// plaintext
     "a868 42f2" // ciphertext
    },

    {48, 72,
     "121110 0a0908 020100", // key
     "20796c 6c6172",// plaintext
     "c049a5 385adc" // ciphertext
    },

    {48, 96,
     "1a1918 121110 0a0908 020100", // key
     "6d2073 696874",// plaintext
     "735e10 b6445d" // ciphertext
    },

    {64, 96,
     "13121110 0b0a0908 03020100", // key
     "74614620 736e6165",// plaintext
     "9f7952ec 4175946c" // ciphertext
    },

    {64, 128,
     "1b1a1918 13121110 0b0a0908 03020100", // key
     "3b726574 7475432d",// plaintext
     "8c6fa548 454e028b" // ciphertext
    },

    {96, 96,
     "0d0c0b0a0908 050403020100", // key
     "65776f68202c 656761737520",// plaintext
     "9e4d09ab7178 62bdde8f79aa" // ciphertext
    },

    {96, 144,
     "151413121110 0d0c0b0a0908 050403020100", // key
     "656d6974206e 69202c726576",// plaintext
     "2bf31072228a 7ae440252ee6" // ciphertext
    },

    {128, 128,
     "0f0e0d0c0b0a0908 0706050403020100", // key
     "6c61766975716520 7469206564616d20",// plaintext
     "a65d985179783265 7860fedf5c570d18" // ciphertext
    },

    {128, 192,
     "1716151413121110 0f0e0d0c0b0a0908 0706050403020100", // key
     "7261482066656968 43206f7420746e65",// plaintext
     "1be4cf3a13135566 f9bc185de03c1886" // ciphertext
    },

    {128, 256,
     "1f1e1d1c1b1a1918 1716151413121110 0f0e0d0c0b0a0908 0706050403020100", // key
     "65736f6874206e49 202e72656e6f6f70",// plaintext
     "4109010405c0f53e 4eeeb48d9c188f43" // ciphertext,
    }
};

const std::vector<std::pair<short, short>> kBlockKeySizes = {
    {32, 64},
    {48, 96},
    {64, 128},
    {96, 144},
    {128, 256}
};

// Run all published tests to confirm basic correctness
void run_all_tests();
bool run_test(const TestParams& params);

// Run timed tests to get timing data.
void run_all_timed_tests();
double run_timed_test(short blockSize, short keySize);

#endif
