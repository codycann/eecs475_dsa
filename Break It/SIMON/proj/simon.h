// by Team #x I can't tell you
//
// ----------------- simon.h ------------------------
// General info:
// Perform key expansion, enc and dec of Simon in Electronic codebook mode
// and get an 80 on the project.
//
// May complie and run with terminal commands:
// g++ -std=c++11 -O3 main.cpp simon.cpp uberzahl.cpp -o simon 
//  
// ./simon
//
// Reference: The SIMON and SPECK Families of Lightweight Block Ciphers, 2013
// https://eprint.iacr.org/2013/404
// 

#ifndef SIMON_H
#define SIMON_H

#include "uberzahl.h"
#include <vector>
using namespace std;


// ------------------ Random key generator (keygen)--------------------//
// Effect: Generate a random key with size n * m;
//
// Parameters: n, the Word size, with default value n = 64; 
// m, the Key Words, with default value m = 4;
// Hence the total length of the key will be n * m.
//
// Requires: n and m must corresponding to a pair of feasible value in simon cipher,
// see details in the reference.
// Error message will be put forward by static errorchecking functions.
// 
// Return value: Return key into a vector of size m with each length n;
// where each key has an acutual length (start from left most non zero bit)
// of n >= acutal_length > n-8.
// 
extern vector<uberzahl> keygen(int n = 64, int m = 4);


//----------------Encrypt and Decrypt function-------------------------//
// Effect: Using the given key, perform the encryption/decription of simon cipher in ECB mode.
//
// Parameter: vec<>key: m key blocks with each length n form a total length of n * m;
// where n, the Word size; and m, the Key Words;
// vec<>plain/ciphertext: text blocks with each length corresponding to 2 * n.
// Thus all inputing cipher blocks' length must corresponding to the inputing key word size.
//
// Require: The acutual length (start from left most non zero bit)
// of the first block of key (which is the first element of the input vector)  
// should be n >= acutal_length > n-8. Otherwise the automatic value detect of n WILL CRASH.
// Error message will be put forward by static errorchecking functions.
//
// Return value: Returns the vector of blocks of encrypted text(cipher text)/ decrypted
// text(plaintext).
//
extern vector<uberzahl> encrypt(vector<uberzahl> key, vector<uberzahl> plaintext);
extern vector<uberzahl> decrypt(vector<uberzahl> key, vector<uberzahl> ciphertext);

// More info and performance chart can be found in the reference
// if you are the guy who need to break this, hahaha, good luck.

#endif

