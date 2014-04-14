#include "speck.h"

#include <iostream>
#include <cassert>
#include <cstdlib>
#include <ctime>
#include <random>

using namespace std;


//LCS and RCS 

#define RCS_64(x,s) (((x) >> (s)) | ((x) << (64-s)))
#define LCS_64(x,s) (((x) << (s)) | ((x) >> (64-s)))

void Speck128CreatePaddedSection(u64 key[2], char inout_padding[16]);

// ****************************************************************************
// Key Manipulations

void Speck128GenerateKey(u64 outKey[2])
{
    std::random_device dev;
    std::uniform_int_distribution<int> dist;

    outKey[0] = 0;
    outKey[1] = 0;
    for(int keyIndex = 0; keyIndex < 2; ++keyIndex)
    {
        for(int i = 0; i < 64; ++i)
        {
            outKey[keyIndex] <<= 1;
            outKey[keyIndex] += (dist(dev) & 1);
        }
    }
    int i = 0;
    i++;
}

void Speck128KeyExpansion ( u64 l[32], u64 k[32] )
{
    for(int i = 0 ; i < 31 ; ++i )
    {
        l[i + 1] = ( k[i] + RCS_64 ( l[i], 8 ) ) ^ i;
        k[i + 1] = LCS_64 ( k[i], 3 ) ^ l[i + 1];
    }
}    



// ****************************************************************************
// Encryption/Decryption Functions

void Speck128Encrypt ( u64 pt[2], u64 ct[2], u64 k[32] )
{
    // ct[0] is x and ct[1] is y
    ct[0] = pt[0];
    ct[1] = pt[1];

    for(int i = 0 ; i < 32 ; ++i)
    {
        ct[0] = ( RCS_64 ( ct[0], 8 ) + ct[1] ) ^ k[i];
        ct[1] = LCS_64 ( ct[1], 3 ) ^ ct[0];
    }
}

void Speck128Decrypt ( u64 pt[2], u64 ct[2], u64 k[32] )
{
    // ct[0] is x and ct[1] is y
    pt[0] = ct[0];
    pt[1] = ct[1];

    for(int i = 0 ; i < 32 ; ++i)
    {
        pt[1] = RCS_64(pt[0] ^ pt[1], 3);
        pt[0] = LCS_64((pt[0] ^ k[31 - i]) - pt[1], 8);
    }
}

void Speck128EncryptSequence(const void * pt, int ptBytes, void ** ct, int * ctBytes, u64 key[2])
{
    assert(ptBytes > 0);

    // Key Expansion
    u64 l[32] = { key[0] };
    u64 k[32] = { key[1] };

    Speck128KeyExpansion(l, k);

    // Always at least 1 byte of buffer.
    // 16 bytes = 128 bits = block size
    const int ptBightsNoBuffer = (ptBytes - (ptBytes & 15));
    const int ct_length = ptBightsNoBuffer + 16;
    const char * const plaintextBegin = (const char *)pt;
    const char * const plaintextNoBufferEnd = plaintextBegin + ptBightsNoBuffer;
    const char * const plaintextEnd = plaintextBegin + ptBytes;
    char * const ciphertextBegin = new char[ct_length];
    char * const ciphertextEnd = ciphertextBegin + ct_length;

    *ctBytes = ct_length;
    *ct = ciphertextBegin;

    // Create Ciphertext
    // Encrypt in chunks of 16 bytes = block size
    char const * cur_plaintext = plaintextBegin;
    char * cur_ciphertext = ciphertextBegin;

    // Encrypt all plaintext without a buffer
    while(cur_plaintext < plaintextNoBufferEnd)
    {
        Speck128Encrypt((u64 *)cur_plaintext, (u64 *)cur_ciphertext, k);
        cur_plaintext += 16;
        cur_ciphertext += 2*sizeof(u64);
    }

    assert(cur_plaintext == plaintextNoBufferEnd);

    // Set up padded ending
    {
        // Fill padded section with random bits seeded by the key
        char padding[16];
        Speck128CreatePaddedSection(key, padding);

        char * const paddingEnd = padding + 16;
        char * cur_padding = padding;
        for(; cur_plaintext < plaintextEnd; ++cur_padding, ++cur_plaintext)
        {
            assert(cur_padding < paddingEnd);
            *cur_padding = *cur_plaintext;
        }

        // Sentinal
        *cur_padding ^= 1;

        // Encrypt final padded region
        Speck128Encrypt((u64 *)padding, (u64 *)(ciphertextEnd - 16), k);
    }
}

// TODO: memcpy might not actually be necessary...
// The size of the retrned array is actually ct_length, but the valid
// data only extends to out_pt_length bytes
void Speck128DecryptSequence(const void * ct, int ctBytes, void ** pt, int * ptBytes, u64 key[2])
{
    assert(ctBytes > 0);
    assert((ctBytes & 15) == 0);

    // Key Expansion
    u64 l[32] = { key[0] };
    u64 k[32] = { key[1] };

    Speck128KeyExpansion(l, k);

    // Always at least 1 byte of buffer.
    // 16 bytes = 128 bits = block size
    const char * const ciphertextBegin = (char *)ct;
    const char * const ciphertextEnd = ciphertextBegin + ctBytes;
    char * const plaintextBegin = new char[ctBytes];
    char * const plaintextEnd = plaintextBegin + ctBytes;

    // Create Ciphertext
    // Decrypt in chunks of 16 bytes = block size
    char const * cur_ciphertext = ciphertextBegin;
    char * cur_plaintext = plaintextBegin;

    // Decrypt all plaintext without a buffer
    while(cur_ciphertext < ciphertextEnd)
    {
        Speck128Decrypt((u64 *)cur_plaintext, (u64 *)cur_ciphertext, k);
        cur_ciphertext += 16;
        cur_plaintext += 2 * sizeof(u64);
    }

    assert(cur_plaintext == plaintextEnd);
    assert(cur_ciphertext == ciphertextEnd);

    // Zero out padded data
    {
        // Fill padded section with random bits seeded by the key
        char padding[16];
        Speck128CreatePaddedSection(key, padding);
        
        char * const paddingEnd = padding + 16;
        char * cur_padding = paddingEnd - 1;
        cur_plaintext = plaintextEnd - 1;
        for(; *cur_plaintext == *cur_padding; --cur_padding, --cur_plaintext)
        {
            assert(cur_padding >= padding);
            *cur_plaintext = 0;
        }

        // Zero out centinal
        assert(*cur_plaintext == (*cur_padding ^ 1));
        *cur_plaintext = 0;
    }

    *ptBytes = cur_plaintext - plaintextBegin;
    *pt = plaintextBegin;
}




// Array of size 16
void Speck128CreatePaddedSection(u64 key[2], char inout_padding[16])
{
    std::mt19937_64 dev;
    std::uniform_int_distribution<int> dist;

    for(int i = 0; i<16; ++i)
    {
        inout_padding[i] = (char)dist(dev);
    }
}