#include "speck.h"
#include <iostream>
#include <cstring>

void TestVectorSpeck128();
void TestSingleBlock();
void TestString(const char * plaintext, u64 key[]);
void TimingTest();

using namespace std;

int main(void)
{
    TestVectorSpeck128();
    TestSingleBlock();

    u64 k[2];
    Speck128GenerateKey(k);
    TestString("The quick brown fox jumps over the lazy dog", k);

    return 0;
}


// published test vector from speck paper 
void TestVectorSpeck128()
{
    u64 l[32] = { 0x0f0e0d0c0b0a0908 };
    u64 k[32] = { 0x0706050403020100 };

    cout << "Test Vector: " << endl;
    cout << "        Key: " << hex << l[0] << "\t" << k[0] << endl;
    Speck128KeyExpansion(l, k);

    u64 pt[2] = { 0x6c61766975716520, 0x7469206564616d20 };
    u64 ct[2] = { 0 };
    cout << "  Plaintext: " << pt[0] << "\t" << pt[1] << endl;

    Speck128Encrypt(pt, ct, k);
    cout << " Encryption: " << hex << ct[0] << "\t" << ct[1] << endl;

    pt[0] = 0;
    pt[1] = 0;
    Speck128Decrypt(pt, ct, k);
    cout << " Decryption: " << hex << pt[0] << "\t" << pt[1] << endl;

    if(!(u64(0xa65d985179783265) == ct[0]) || !(u64(0x7860fedf5c570d18) == ct[1]))
    {
        cout << "ENCRYPTION FAILED!" << endl;
    }

    if(!(u64(0x6c61766975716520) == pt[0]) || !(u64(0x7469206564616d20) == pt[1]))
    {
        cout << "DECRYPTION FAILED!" << endl;
    }
    cout << endl;
}

// Example of using EncryptString and DecryptString to encrypt and decrypt a single block
void TestSingleBlock()
{
    u64 k[2] = { 0x0f0e0d0c0b0a0908, 0x0706050403020100 };


    u64 pt[2] = { 0x6c61766975716520, 0x7469206564616d20 };
    int pt_len = 16;
    cout << "Test Single Block: " << endl;
    cout << "        Key: " << hex << k[0] << "\t" << k[1] << endl;
    cout << "  Plaintext: " << pt[0] << "\t" << pt[1] << endl;

    u64 *ct;
    int ct_len;
    Speck128EncryptSequence((void *)pt, pt_len, (void **)&ct, &ct_len, k);
    cout << " Encryption: " << hex << ct[0] << "\t" << ct[1] << endl;
    cout << "    Padding: " << hex << ct[2] << "\t" << ct[3] << endl;

    u64 * plainText;
    int dec_len;
    Speck128DecryptSequence((char *)ct, ct_len, (void **)&plainText, &dec_len, k);
    cout << " Decryption: " << hex << plainText[0] << "\t" << plainText[1] << endl;
    cout << "   PT/CT/PT: Length - " << dec << pt_len << "/" << ct_len << "/" << dec_len << endl;

    if(!(u64(0xa65d985179783265) == ct[0]) || !(u64(0x7860fedf5c570d18) == ct[1]))
    {
        cout << "ENCRYPTION FAILED!" << endl;
    }

    if(!(u64(0x6c61766975716520) == plainText[0]) || !(u64(0x7469206564616d20) == plainText[1]))
    {
        cout << "DECRYPTION FAILED!" << endl;
    }

    cout << endl;

    delete ct;
    delete plainText;
}

void TestString(const char * plaintext, u64 key[])
{
    int pt_len = strlen(plaintext);
    cout << "Test String: " << endl;
    cout << "        Key: " << hex << key[0] << "\t" << key[1] << endl;
    cout << "  Plaintext: " << plaintext << endl;

    char * ciphertext;
    int ct_len;
    Speck128EncryptSequence(plaintext, pt_len, (void **)&ciphertext, &ct_len, key);
    cout << " Encryption: ";
    for(int i = 0; i < (ct_len/16); ++i)
    {
        cout << ((u64 *)ciphertext)[i] << " ";
    }
    cout << endl;

    char * decrypted;
    int dec_len;
    Speck128DecryptSequence(ciphertext, ct_len, (void **)&decrypted, &dec_len, key);
    cout << " Decryption: " << decrypted << endl;
    cout << "   PT/CT/PT: Length - " << dec << pt_len << "/" << ct_len << "/" << dec_len << endl;

    delete ciphertext;
    delete decrypted;
}