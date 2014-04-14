#include "speck.h"
#include <vector>
#include <iostream>
#include <string>
#include <random>
#include <chrono>

using namespace std;


template <typename T, typename U>
void run_test_vector(const vector<T> &k, const U &m, unsigned int numRounds) {
	vector<T> subkeys = expandKey<T, U>(numRounds, k);
	U cipher = encipherBlock(m, numRounds, subkeys);
	U plain = decipherBlock(cipher, numRounds, subkeys);
	cout << sizeof(T) * 16 << '/' << (sizeof(T) * 8) * (k.size()) << " test vector\n";
	cout << "message:\t" << m << '\n';
	cout << "ciphertext:\t" << cipher << '\n';
	cout << "deciphered:\t" << plain << '\n';
	cout << '\n';	
}

int main()
{
	/* NOTE: the key parts appear in the opposite order that they appear in the paper.
	 * This is because lower elements of the vector represent less significant digits of the key.
	 * In the command-line interface, keys are entered in THE SAME WAY that they appear
	 * in the paper--that is, entering the key "1111 2222" will result in a vector
	 * that looks like {0x2222, 0x1111}. */

	// 32/64 test vector
	vector<uint16> k_16 = {0x0100, 0x0908, 0x1110, 0x1918};
	uint32 m_32 = string("6574694c");
	run_test_vector(k_16, m_32, 22);

	// 48/72 test vector
	vector<uint24> k_24 = {0x020100, 0x0a0908, 0x121110};
	uint48 m_48 = uint48(string("20796c6c6172"));
	run_test_vector(k_24, m_48, 22);

	// 48/96 test vector
	k_24 = {0x020100, 0x0a0908, 0x121110, 0x1a1918};
	m_48 = uint48(string("6d2073696874"));
	run_test_vector(k_24, m_48, 23);

	// 64/96 test vector
	vector<uint32> k_32 = {0x03020100, 0x0b0a0908, 0x13121110};
	uint64 m_64 = uint64(string("74614620736e6165"));
	run_test_vector(k_32, m_64, 26);

	// 64/128 test vector
	k_32 = {0x03020100, 0x0b0a0908, 0x13121110, 0x1b1a1918};
	m_64 = uint64(string("3b7265747475432d"));
	run_test_vector(k_32, m_64, 27);

	// 96/96 test vector
	vector<uint48> k_48 = {0x050403020100, 0x0d0c0b0a0908};
	uint96 m_96 = uint96(string("65776f68202c656761737520"));
	run_test_vector(k_48, m_96, 28);

	// 96/144 test vector
	k_48 = {0x050403020100, 0x0d0c0b0a0908, 0x151413121110};
	m_96 = uint96(string("656d6974206e69202c726576"));
	run_test_vector(k_48, m_96, 29);

    	// 128/128 test vector
	vector<uint64> k_64 = {0x0706050403020100, 0x0f0e0d0c0b0a0908};
	uint128 m_128 = uint128(string("6c617669757165207469206564616d20"));
	run_test_vector(k_64, m_128, 32);

	// 128/192 test vector
	k_64 = {0x0706050403020100, 0x0f0e0d0c0b0a0908, 0x1716151413121110};
	m_128 = uint128(string("726148206665696843206f7420746e65"));
	run_test_vector(k_64, m_128, 33);

	// 128/256 test vector
	k_64 = {0x0706050403020100, 0x0f0e0d0c0b0a0908, 0x1716151413121110, 0x1f1e1d1c1b1a1918};
	m_128 = uint128(string("65736f6874206e49202e72656e6f6f70"));
	run_test_vector(k_64, m_128, 34);
}
