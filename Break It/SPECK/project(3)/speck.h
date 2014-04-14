#ifndef _speck_h_
#define _speck_h_

#include <cstdint>
#include <vector>
#include <utility>
#include <cstddef>
#include <gmpxx.h>
#define word_t uint_fast64_t


class speck {
	word_t word_mask;
	size_t bit;
	size_t key_words;	// m
	size_t n_rounds;	// T
	size_t alpha;
	size_t beta;
	word_t* l;
	word_t* k;
	inline word_t lshift(word_t in, size_t lnum)
	{
		return ((in >> (bit - lnum)) | (in << lnum)) & word_mask;
	}
	
	inline word_t rshift(word_t in, size_t rnum)
	{
		return ((in >> rnum) | (in << (bit - rnum))) & word_mask;
	} 
	
public:
	// Construct a encryption/decryption class with word size and 
	// key size specified. Invalid word size of key size will lead
	// to exception thrown. 
	speck(size_t word_size, size_t key_size);
	
	// Automatically invoked when object destroyed. Acts on internal
	// elements only.
	~speck();
	
	// Set the key to the key pointed by key. The length of key must
	// be larger or equal to the number of words leaded. If not, it
	// leads to undefined behaviour.
	void set_key(const word_t* key);
	
	// Separate the text passed in into several small chunks, saved 
	// in the vector returned. The first element in the vector 
	// corresponds to the smallest part in the text.
	std::vector<word_t> separate(mpz_class text);
	
	// Reconstruct a long number using the list. The first element 
	// in the list corresponds to the smallest part in the number.
	mpz_class reconstruct(const std::vector<word_t>& list);
	
	// Encode the two words specified by word1 and word2. Return value
	// is pair <E(word1),E(word2)>. Causes undefined behaviour if key
	// was not previously set.
	std::pair<word_t, word_t> encrypt(word_t word1, word_t word2);
	
	// Encode a list of words. The encryption is done in pairs. If there
	// are not enough pairs, 0 is used to fill the position.
	std::vector<word_t> encrypt(std::vector<word_t> word_list);
	
	// Encrypt a long number and return cyphertext returned in ECB mode.
	mpz_class encrypt(mpz_class plaintext);
	
	// Decrypt a number produced by encrypt function and return plaintext.
	mpz_class decrypt(mpz_class cyphertext);
	
	// Decode the two words specified by word1 and word2. Return value
	// is pair <D(word1),D(word2)>. Causes undefined behaviour if key
	// was not previously set.
	std::pair<word_t, word_t> decrypt(word_t cypher1, word_t cypher2);
	
	// Decode a list of words. The list must come in pair. If there
	// are not enough pairs, an exception will be thrown.
	std::vector<word_t> decrypt(std::vector<word_t> word_list);
	
	// Generate a key stored in a length 4 array. If the required
	// key length is shorter than 4 words, unused part is filled 
	// with 0. Delete returned array after use.
	word_t* gen_key(void);
};

#endif
