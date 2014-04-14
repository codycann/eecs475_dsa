#include "speck.h"
#include <stdexcept>
#include <iostream>
#include <random>
#include <ctime>

speck::speck(size_t word_size, size_t key_size)
{
	bit = word_size;
	alpha = 8;
	beta = 3;
	key_words = key_size/word_size;
	switch (word_size)
	{
	case 16:
		if(key_size == 64)
		{
			word_mask = (word_t)0xfffful;
			n_rounds = 22;
			alpha = 7;
			beta = 2;
		}
		else
			throw std::invalid_argument("ERROR: Invalid key size!\n");
		break;
	case 24:
		word_mask = (word_t)0xfffffful;
		if(key_size == 72)
			n_rounds = 22;
		else if(key_size == 96)
			n_rounds = 23;
		else
			throw std::invalid_argument("ERROR: Invalid key size!\n");
		break;
	case 32:
		word_mask = (word_t)0xfffffffful;
		if(key_size == 96)
			n_rounds = 26;
		else if(key_size == 128)
			n_rounds = 27;
		else
			throw std::invalid_argument("ERROR: Invalid key size!\n");
		break;
	case 48:
		word_mask = (word_t)0xfffffffffffful;
		if(key_size == 96)
			n_rounds = 28;
		else if(key_size == 144)
			n_rounds = 29;
		else
			throw std::invalid_argument("ERROR: Invalid key size!\n");
		break;
	case 64:
		word_mask = (word_t)0xfffffffffffffffful;
		if(key_size == 128)
			n_rounds = 32;
		else if(key_size == 192)
			n_rounds = 33;
		else if(key_size == 256)
			n_rounds = 34;
		else
			throw std::invalid_argument("ERROR: Invalid key size!\n");
		break;
	default:
		throw std::invalid_argument("ERROR: Invalid word size!\n");
	}
	l = new word_t[n_rounds - 2 + key_words];
	k = new word_t[n_rounds];
	std::cout << "alpha:" << alpha << " beta:" << beta << " m:" << key_words << " T:" << n_rounds << std::endl;
}

speck::~speck()
{
	delete[] l;
	delete[] k;
}

/* requires the key to be array of words;
   the key should be little_endian. i.e. the key is k[0]k[1]...k[m-1]
*/
void speck::set_key(const word_t* key)
{
	for(size_t i = 0; i <= key_words - 2; i++)
	{
		l[key_words - 2 - i] = key[i];
	}
	k[0] = key[key_words - 1];
	for(size_t i = 0; i <= n_rounds - 2; i++)
	{
		l[i + key_words - 1] = ((k[i] + rshift(l[i], alpha)) & word_mask) ^ i;
		k[i + 1] = lshift(k[i], beta) ^ l[i + key_words - 1];
	}
	//for(size_t i = 0; i < n_rounds; i++)
		//std::cout << "l[" << i << "]: " << std::hex << l[i] << std::dec << std::endl;
}

std::vector<word_t> speck::separate(mpz_class text)
{
	std::vector<word_t> retval;
	mpz_t zero;
	mpz_init_set_ui(zero, 0ul);
	mpz_t text_num;
	mpz_init(text_num);
	mpz_set(text_num, text.get_mpz_t());
	mpz_t remainder;
	mpz_init(remainder);
	while(mpz_cmp(text_num, zero) != 0)
	{
		mpz_tdiv_r_2exp(remainder, text_num, bit);
		mpz_tdiv_q_2exp(text_num, text_num, bit);
		retval.push_back(mpz_get_ui(remainder));
		mpz_tdiv_r_2exp(remainder, text_num, bit);
		mpz_tdiv_q_2exp(text_num, text_num, bit);
		retval.push_back(mpz_get_ui(remainder));
	}
	//std::cout << "sep" << retval.size() << std::endl;
	return retval;
}

mpz_class speck::reconstruct(const std::vector<word_t>& list)
{
	mpz_t retval;
	mpz_init_set_ui(retval, 0ul);
	//std::cout << list.size() << std::endl;
	auto it = list.end();
	do
	{
		it--;
		mpz_mul_2exp(retval, retval, bit);
		mpz_add_ui(retval, retval, *it);
	}while(it !=  list.begin());
	return mpz_class(retval);
}	

std::pair<word_t, word_t> speck::encrypt(word_t word1, word_t word2)
{
	if((word1 & (~word_mask)) != 0)
		throw std::domain_error("First word is too large!\n");
	if((word2 & (~word_mask)) != 0)
		throw std::domain_error("Second word is too large!\n");
	for(size_t i = 0; i < n_rounds; i++)
	{
		word1 = ((rshift(word1, alpha) + word2) & word_mask) ^ k[i];
		word2 = lshift(word2, beta) ^ word1;
	}
	return std::make_pair(word1, word2);
}

std::vector<word_t> speck::encrypt(std::vector<word_t> word_list)
{
	std::vector<word_t> retval;
	int i = 0;
	while(2*i < word_list.size())
	{
		word_t word1 = word_list[2*i];
		word_t word2;
		if(2*i + 1 < word_list.size())
			word2 = word_list[2*i + 1];
		else
			word2 = 0;
		auto enc = encrypt(word1, word2);
		retval.push_back(enc.first);
		retval.push_back(enc.second);
		i++;
	}
	return retval;
}

mpz_class speck::encrypt(mpz_class plaintext)
{
	return reconstruct(encrypt(separate(plaintext)));
}

std::pair<word_t, word_t> speck::decrypt(word_t word1, word_t word2)
{
	if((word1 & (~word_mask)) != 0)
		throw std::domain_error("First word is too large!\n");
	if((word2 & (~word_mask)) != 0)
		throw std::domain_error("Second word is too large!\n");
	for(int i = n_rounds - 1; i >= 0; i--)
	{
		word2 = rshift(word2 ^ word1, beta);
		word1 = lshift(((word1 ^ k[i]) - word2) & word_mask, alpha);
	}
	return std::make_pair(word1, word2);
}

std::vector<word_t> speck::decrypt(std::vector<word_t> word_list)
{
	std::vector<word_t> retval;
	int i = 0;
	while(2*i < word_list.size())
	{
		word_t word1 = word_list[2*i];
		word_t word2;
		if(2*i + 1 < word_list.size())
			word2 = word_list[2*i + 1];
		else
			throw std::invalid_argument("ERROR: word is not in pair!");
		auto dec = decrypt(word1, word2);
		retval.push_back(dec.first);
		retval.push_back(dec.second);
		i++;
	}
	return retval;
}

mpz_class speck::decrypt(mpz_class cyphertext)
{
	return reconstruct(decrypt(separate(cyphertext)));
}

word_t* speck::gen_key(void)
{
	word_t* retval = new word_t[4]();
	std::mt19937 gen(time(0));
	if(bit > 32u)
	{
		for(size_t i = 0; i < key_words; i++)
		{
			retval[i] = (((word_t)gen()) << 32 | (word_t)gen()) & word_mask;
		}
	}
	else
	{
		for(size_t i = 0; i < key_words; i++)
		{
			retval[i] = gen() & word_mask;
		}
	}
	return retval;
}
