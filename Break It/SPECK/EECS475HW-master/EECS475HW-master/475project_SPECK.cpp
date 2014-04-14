//=============================================================================
// Team:      34 (CAR)
// Uniqnames: calexan, jamcheng, rreneker
// Algorithm: Speck
//=============================================================================

#include <iostream>
#include <vector>
#include <random>
#include <cstdlib>
#include <ctime>
#include <utility>
#include <string>
#include <cstring>
#include "uberzahl.h"

using namespace std;

vector<uint64_t> key_words(64,0);
vector<uint64_t> keys(64,0);

//=============================================================================
uint64_t r_rotate(uint64_t val, unsigned int shift)
{
	return (val >> shift) | (val << (64 - shift));
}

//=============================================================================
uint64_t l_rotate(uint64_t val, unsigned int shift)
{
	return (val << shift) | (val >> (64 - shift));
}

//=============================================================================
void key_expansion(int rounds, int m)
{
	for(int i = 0; i < rounds-1; i++)
	{
		key_words[i+m-1] =
			(keys[i] + r_rotate(key_words[i], 8)) ^ i;
		keys[i+1] = l_rotate(keys[i], 3) ^ key_words[i+m-1];
	}
}

//=============================================================================
vector<uint64_t> encrypt(const vector<uint64_t> &key,
	const vector<uint64_t> &plain_text)
{
	// Copy the keys into the appropriate places
	keys[0] = key[3];
	for (int i = 0; i < 3; ++i)
		key_words[i] = key[2 - i];

	// Expand the keys
	key_expansion(34, 4);

	// Split the plain_text into two words
	uint64_t y = plain_text[1];
	uint64_t x = plain_text[0];

	// Encrypt with 34 rounds
	for (int i = 0; i < 34; ++i)
	{
		x = (r_rotate(x,8) + y) ^ keys[i];
		y = l_rotate(y,3) ^ x;
	}
	
	return {x,y};
}

//=============================================================================
vector<uint64_t> decrypt(const vector<uint64_t> &key,
	const vector<uint64_t> &cipher_text)
{
	// Copy the keys into the appropriate places
	keys[0] = key[3];
	for (int i = 0; i < 3; ++i)
		key_words[i] = key[2 - i];

	// Expand the keys
	key_expansion(34, 4);

	// Split the cipher into two words
	uint64_t y = cipher_text[1];
	uint64_t x = cipher_text[0];

	// Decrypt using 34 rounds
	for (int i = 33; i >= 0; --i)
	{
		y = r_rotate(y ^ x, 3);
		x = l_rotate((x ^ keys[i]) - y, 8);
	}

	return {x,y};
}

//=============================================================================
int main(int argc, char* argv[])
{
	srand(time(NULL));
	vector<uint64_t> key = {0x1f1e1d1c1b1a1918, 0x1716151413121110,
		0x0f0e0d0c0b0a0908, 0x0706050403020100};
	for(int i = 0; i < 1; i++){
		vector<uint64_t> message = {0x65736f6874206e49, 0x202e72656e6f6f70};
		vector<uint64_t> cipher = encrypt(key, message);
		cout << hex << cipher[0] << " " << cipher[1] << endl;
		vector<uint64_t> check = decrypt(key, cipher);
		cout << hex << check[0] << " " << check[1] << endl;
	}
}

