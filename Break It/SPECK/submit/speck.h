#ifndef __SPECK__H__
#define __SPECK__H__
#include "aux.h"
#include "uberzahl/uberzahl.h"

// Implements the Speck 128/256 cryptosystem
class Speck_128_256
{
	private:
	    // Values for our Speck 128/256 cryptosystem
		const static int alpha = 8, beta = 3, T = 34, m = 4;

		// 64 bit number with 10000000000.....64 zeros 2^64 (65 bits), mainly for parsing and use in taking modulus
		static uberzahl wordsize;

		// Round function for each round of encryption
		// R_k(x,y) = (ans1, ans2)
		void speck2nEncryptionMap(const uberzahl &x,const uberzahl &y, uberzahl &ans1, uberzahl &ans2 ,const uberzahl &keyWord);

		// Inverse Round function for each round of decryption
		// R_k^{-1}(x,y) = (ans1, ans2)
		void speck2nDecryptionMap(const uberzahl &x, const uberzahl &y, uberzahl &ans1, uberzahl &ans2 , const uberzahl &keyWord);

		// Calculates and sets keys for rounds of encryption/decryption
		//Assumes that L-key vector contains l0, l1, l2 and K-key vector contains k0 before execution
    	void setKeySchedule(std::vector<uberzahl> &keyL, std::vector<uberzahl> &keyK);

    	// Sets initial and subsequent keys for encryption/decryption in vector for L-keys and K-keys
		void setInitKeys(std::vector<uberzahl> &keyL, std::vector<uberzahl> &keyK, const uberzahl &l0, const uberzahl &l1, const uberzahl &l2, const uberzahl &k0);

		// Sets subsequent keys for encryption/decryption according to initial key values in vector for L-keys and K-keys
		void setInitKey(std::vector<uberzahl> &keyL, std::vector<uberzahl> &keyK, const uberzahl &Key);

	public:
	    // Constructor: Sets wordsize
		Speck_128_256();

		// Encryption method for Speck2n
		// Accepts plaintext of up to 128-bits
        // Accepts Key of size up to 256-bits
		uberzahl encryption(const uberzahl& plaintext, const uberzahl& key);

		// Decryption method for Speck2n
		// Accepts ciphertext of up to 128-bits
        // Accepts Key of size up to 256-bits
		uberzahl decryption(const uberzahl& ciphertext, const uberzahl& key);

};
#endif
