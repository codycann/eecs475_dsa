//Implementation of SPECK 128/128 for EECS475 Project
//Requires x86intrin.h to have _lrotl and _lrotr to be defined
//Requires uint64_t to be defined
#ifndef _SPECK_H
#define _SPECK_H
#include <x86intrin.h>
#include <stdint.h>
#include <openssl/rand.h>
#include <openssl/err.h>
#include <iostream>
#include <string>

/* IMPORTANT NOTE ABOUT ALL FUNCTIONS:
All functions treat the 128 bit inputs as little endian. What this means is that buffer[0] is
the least significant 64 bits, and buffer[1] is the most significant 64 bits. */

//round function for encryption
inline void round(uint64_t &x, uint64_t &y, uint64_t &k);

//round function for decryption
inline void inverse_round(uint64_t &x, uint64_t &y, uint64_t &k);

/* Uses openssl's RAND_bytes to generate cryptographically strong bytes for generating a 128 bit key stored 
into the given key buffer.
On failure, will return -1 with an error message printed to stdout. On success, returns 0. */
int speck_generate_key(uint64_t* key);

/* Expands the given 128 bit key into the 2048 bits of round key used in encryption/decryption.
The length of the expanded_key buffer must be 32 (2048 bits).
Returns -1 if key or expanded_key is nullptr. On success, returns 0.*/
int speck_expand_key(uint64_t* key, uint64_t* expanded_key);

/* Encrypts the given plaintext buffer with the given expanded key, stores the result in the given ciphertext buffer. 
If any pointers are nullptr, returns -1. On success, returns 0.
NOTE: plaintext[1] is the most significant part of the 128 bit block, and plaintext[0] is the least
significant part of the block, when comparing with public test vectors & other implementations.
In short, this means that the 128 bit blocks are little endian in respect to the 64 bit words.	
This follows the given implementation in the SPECK Paper in appendix A.*/
int speck_encrypt(uint64_t* plaintext, uint64_t* ciphertext, uint64_t* expanded_key);

/* Decrypts the given ciphertext buffer with the given expanded key into the given plaintext buffer.
If any pointers are nullptr, returns -1. On success, returns 0. */
int speck_decrypt(uint64_t* plaintext, uint64_t* ciphertext, uint64_t* expanded_key);

/* Encrypts the given 64 bit stream using ECB mode with the given key.
Adds padding if enable_padding is true, otherwise returns the stream as is (requires len is a multiple of 2).
On success, returns the ciphertext buffer in a dynamically allocated buffer. 
On failure, returns nullptr. new_len represents the new (padded) size.*/
uint64_t* speck_encrypt_ecb(uint64_t* key, uint64_t* pt, bool enable_padding, size_t len, size_t &new_len);

/* Decrypts the given 64 bit stream using ECB mode with the given key.
Removes padding if enable_padding is true, otherwise returns the stream as is. 
On success, returns the ciphertext buffer in a dynamically allocated buffer. 
On failure, returns nullptr. new_len represents the new (unpadded) size.*/
uint64_t* speck_decrypt_ecb(uint64_t* key, uint64_t* ct, bool enable_padding, size_t len, size_t &new_len);

/* Converts the given string to a dynamically allocated uint64_t array, concatinating blocks of 8 bytes into a single 64 bit word
using little endian encoding.
If enable_padding is true, pads the output to a final size being a multiple of two, for use in ECB encryption.
If enable_padding is false, returns nullptr if buffer size is not a multiple of 8. */
uint64_t* transform_string(std::string buffer, bool enable_padding, size_t &new_size);

/* Converts the given uint64_t array to std::string, breaking up each 64 bit word into a stream of chars using 
little endian encoding.
If enable_padding is true, cuts all padding off at the end of the buffer.
If enable_padding is false, returns the string as-is.
Returns an empty string if len is 0. */
std::string transform_uint64_array(uint64_t* buffer, bool enable_padding, size_t len);

#endif //_SPECK_H