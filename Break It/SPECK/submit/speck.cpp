#include "speck.h"
uberzahl Speck_128_256::wordsize = -1;

// Method for performing encryption, given two inputs and the key value
// Sets argument values in the method header for intended output; returns nothing
void Speck_128_256::speck2nEncryptionMap(const uberzahl &x, const uberzahl &y, uberzahl &ans1, uberzahl &ans2,
											const uberzahl &keyWord)
{
    // Speck encryption algorithm for two inputs
	ans1 = ((shiftRight64(x,alpha) + y)&(wordsize - 1)) ^ keyWord;
	ans2 = (shiftLeft64(y,beta)) ^ ((shiftRight64(x,alpha) + y)&(wordsize - 1)) ^ keyWord;
}

// Method for performing decryption, given two inputs and the key value
// Sets argument values in the method header for intended output; returns nothing
void Speck_128_256::speck2nDecryptionMap(const uberzahl &x, const uberzahl &y, uberzahl &ans1, uberzahl &ans2,
                                            const uberzahl &keyWord)
{
    // Speck decryption algorithm for two inputs
	ans1 = shiftLeft64( (((x ^ keyWord) + (wordsize - (shiftRight64(x^y,beta)))) & (wordsize - 1)), alpha);
	ans2 = shiftRight64(x^y,beta);
}

// Sets initial values for encryption map and Speck algorithm
// Then sets all subsequent keys to be used for future encryption/decryption
// rounds by calling setKeySchedule()
void Speck_128_256::setInitKeys(std::vector<uberzahl> &keyL, std::vector<uberzahl> &keyK, const uberzahl &l2, const uberzahl &l1, const uberzahl &l0, const uberzahl &k0)
{
    // Set initial values for first round of encryption/decryption map by pushing into L-key vector and K-key vector
	keyL.clear(); keyK.clear();
	keyL.push_back(l0);
	keyL.push_back(l1);
	keyL.push_back(l2);
	keyK.push_back(k0);
	// Calculate all subsequent keys for subsequent rounds
	setKeySchedule(keyL, keyK);
}

// Sets keys for future rounds according to Speck algorithm
void Speck_128_256::setKeySchedule(std::vector<uberzahl> &keyL, std::vector<uberzahl> &keyK)
{
    // Calculates each key for future rounds and stores into vector
	for(int i = 0; i <= T - 2 ; ++i)
	{
		keyL.push_back((((keyK[i] + shiftRight64(keyL[i], alpha)) & (wordsize - 1)) ^ i));
		keyK.push_back((shiftLeft64(keyK[i], beta) ^ (keyL.back())));
	}
}

// Parses key into four blocks of 64-bits each
void Speck_128_256::setInitKey(std::vector<uberzahl> &keyL, std::vector<uberzahl> &keyK, const uberzahl &Key)
{
    // Begins with left-most 64-bits and parses each 64-bit block by moving right in a 256 bit key
    //Breaks a 256 bit key K into (l2, l1, l0, k0) and stores these initial values into L-key vector and K-key vector
	setInitKeys(keyL, keyK, (Key >> 192) & (wordsize - 1),  (Key >> 128) & (wordsize - 1),
             (Key >> 64) & (wordsize - 1), Key & (wordsize - 1));
}

// Encryption method for any given plaintext
uberzahl Speck_128_256::encryption(const uberzahl& plaintext, const uberzahl &key)
{
	std::vector<uberzahl> keyL, keyK;
	//Performs key expansion and expanded keys are in L-key vector and K-key vector
	setInitKey(keyL, keyK, key);
    // Splits 128-bit plaintext message into two 64-bit blocks and stores
    //  them into first and sec
	uberzahl sec = plaintext & ((uberzahl(1)<<64) - 1), first = (plaintext >> 64) & ((uberzahl(1)<<64) - 1);
	// Sets up data storage type for x and y in Speck encryption
	std::pair<uberzahl,uberzahl> ans;
	uberzahl ans1,ans2;

	// Encrypts message by running through rounds of generated keys with the
	//  original plaintext and the intermediate messages from an encryption
	//  round, and sets the final encrypted message blocks to first and sec
	for(int i = 0; i <= T - 1; ++i)
	{
		speck2nEncryptionMap(first,sec,ans1,ans2,keyK[i]);
		first = ans1;
		sec = ans2;
	}

	// Return the concatenation of the two blocks as a 128-bit block
	return ((first << 64) | sec);
}

// Constructor: Sets wordsize equal to 64-bits, which is the value of n in
// our implementation of Speck2n
Speck_128_256::Speck_128_256()
{
	wordsize = (uberzahl(1) << 64);
}

// Decryption method for a given ciphertext
uberzahl Speck_128_256::decryption(const uberzahl& ciphertext, const uberzahl& key)
{
	std::vector<uberzahl> keyL, keyK;
	//Performs key expansion and expanded keys are in L-key vector and K-key vector
	setInitKey(keyL, keyK, key);
    // Split 128-bit ciphertext into two blocks of 64-bits each
	uberzahl sec = ciphertext & ((uberzahl(1)<<64) - 1), first = (ciphertext >> 64) & ((uberzahl(1)<<64) - 1);
	// Sets up data structures for decryption
	std::pair<uberzahl,uberzahl> ans;
	uberzahl ans1,ans2;

	// Decrypts ciphertext by running through decryption algorithm with keys
	//  and intermediate decrypted messages, and stores the final values into
	//  first and sec
	for(int i = T - 1; i >= 0; --i)
	{
		speck2nDecryptionMap(first,sec,ans1,ans2,keyK[i]);
		first = ans1;
		sec = ans2;
	}

	// Return the concatentation of first and sec as a 128-bit block.
	return (first << 64) | (sec);
}
