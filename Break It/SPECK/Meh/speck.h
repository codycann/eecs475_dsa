#ifndef SPECK_H
#define SPECK_H

#include <cstdint>

typedef uint64_t u64;

// Key Manipulations
void Speck128GenerateKey(u64 key[2]);
void Speck128KeyExpansion(u64 l[32], u64 k[32]);

// Encryption/Decryption Interfaces
void Speck128Encrypt(u64 pt[2], u64 ct[2], u64 key[32]);
void Speck128Decrypt(u64 pt[2], u64 ct[2], u64 key[32]);
void Speck128EncryptSequence(const void * pt, int ptBytes, void ** ct, int * ctBytes, u64 key[2]);
void Speck128DecryptSequence(const void * ct, int ctBytes, void ** pt, int * ptBytes, u64 key[2]);

#endif // SPECK_H