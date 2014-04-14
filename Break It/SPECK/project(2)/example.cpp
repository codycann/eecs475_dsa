#include "speck.h"
#include <iostream>

using namespace std;

int main() {
	// vector of 16-bit words to store 64-bit key
	vector<uint16> k = {0x0100, 0x0908, 0x1110, 0x1918};

	// expand the key with 22 rounds
	// the template arguments are <WORD_SIZE, BLOCK_SIZE>
	vector<uint16> k_ex = expandKey<uint16, uint32>(22, k);

	// 32-bit message to be decoded
	uint32 message = 0x6574694c;

	// encrypt the message with 22 rounds
	uint32 cipher = encipherBlock(message, 22, k_ex);

	// decrypt the message
	uint32 plain = decipherBlock(cipher, 22, k_ex);

	// output our results
	cout << "message:\t" << message << '\n';
	cout << "ciphertext:\t" << cipher << '\n';
	cout << "deciphered:\t" << plain << '\n';

	return 0;
}
