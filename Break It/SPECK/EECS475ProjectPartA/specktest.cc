#include "speck.h"
#include "speckpub.h"
#include <iostream>

using namespace std;

/* NOTE: All 128 bit words are little endian encoded */

int main(){

	uint64_t plaintext[2];
	uint64_t ciphertext[2];
	uint64_t key[2];
	uint64_t pt_test[2];
	uint64_t ct_test[2];
	uint64_t expKey[32];
	plaintext[1] = 0x6c61766975716520;
	plaintext[0] = 0x7469206564616d20;
	ciphertext[1] = 0xa65d985179783265;
	ciphertext[0] = 0x7860fedf5c570d18;
	key[1] = 0x0f0e0d0c0b0a0908;
	key[0] = 0x0706050403020100;

	cout << "Public SPECK 128/128 Test vector check: " << endl;
	cout << "Plaintext: " << hex << plaintext[1] << " " << plaintext[0] << endl;
	cout << "Ciphertext: " << hex << ciphertext[1] << " " << ciphertext[0] << endl;

	speck_expand_key(key, expKey);
	speck_encrypt(plaintext, ct_test, expKey);

	bool ctEqual = ciphertext[0] == ct_test[0] && ciphertext[1] == ct_test[1];

	cout << "Cipher text equality " << (ctEqual ? "true" : "false") << endl;

	speck_decrypt(pt_test, ciphertext, expKey);

	bool ptEqual = plaintext[0] == pt_test[0] && plaintext[1] == pt_test[1];

	cout << "Plain text equality " << (ptEqual ? "true" : "false") << endl;

	cout << "Plaintext computed: " << hex << pt_test[1] << " " << pt_test[0] << endl;
	cout << "Ciphertext computed: " << hex << ct_test[1] << " " << ct_test[0] << endl;

	cout << "-----------------------" << endl << endl;

	cout << "Testing encryption/decryption of 0x0 0" << endl;

	uint64_t genKey[2];
	uint64_t zeroTest[2];
	zeroTest[1] = 0;
	zeroTest[0] = 0;
	int rc = speck_generate_key(genKey);
	if (rc == -1) {
		cout << "key generation failed" << endl;
		return -1;
	}
	speck_expand_key(genKey, expKey);

	cout << "Generated key: " << hex << genKey[1] << " " << genKey[0] << endl;
	cout << "Zero vector: " << hex << zeroTest[1] << " " << zeroTest[0] << endl;
	speck_encrypt(zeroTest, zeroTest, expKey);

	cout << "Encryption: " << hex << zeroTest[1] << " " << zeroTest[0] << endl;
	speck_decrypt(zeroTest, zeroTest, expKey);

	cout << "Decryption: " << hex << zeroTest[1] << " " << zeroTest[0] << endl;

	cout << "-------------------------" << endl << endl;

	cout << "Testing ECB encryption of 0 stream, length 9" << endl;

	uint64_t ecbTest[9];
	uint64_t* ecbResult;
	uint64_t* ecbDec;
	uint64_t newLen;
	for (int i = 0; i < 9; i++){
		ecbTest[i] = 0;
	} 
	ecbResult = speck_encrypt_ecb(genKey, ecbTest, true, 9, newLen);
	cout << "New Length: " << dec << newLen << endl;
	cout << "Encrypted 0 ECB stream : " << endl;
	for (size_t i = 0; i < newLen; i += 2){
		cout << hex << ecbResult[i + 1] << " " << ecbResult[i] << endl;
	}

	ecbDec = speck_decrypt_ecb(genKey, ecbResult, true, newLen, newLen);
	cout << "New Length: " << dec << newLen << endl;
	cout << "Decrypted 0 ECB stream : " << endl;
	for (size_t i = 0; i < newLen - 2; i += 2){
		cout << hex << ecbDec[i + 1] << " " << ecbDec[i] << endl;
	}
	
	//Printout last one specially because it's not a multiple of 2
	cout << hex << ecbDec[newLen - 1] << endl;
	delete[] ecbResult;
	delete[] ecbDec;

	cout << "--------------------------" << endl << endl;

	//test buffer functions and ecb encryption with them
	cout << "Testing buffer functions: " << endl;
	string str;
	str.resize(13);
	for(int i = 0; i < 13; i++){
		str[i] = 0xFF - (unsigned char) i;
	}

	cout << "string: 0x";
	for(unsigned char i : str){
		cout << hex << (unsigned int) i;
	}
	cout << endl;
	uint64_t newBufferSize;
	uint64_t* newBuffer = transform_string(str, true, newBufferSize);
	cout << "64bit buffer: 0x";
	for (size_t i = 0; i < newBufferSize; i++){
		cout << hex << newBuffer[i] << " ";
	}
	cout << endl;

	ecbResult = speck_encrypt_ecb(genKey, newBuffer, true, newBufferSize, newLen);
	cout << "Encrypted ECB stream : 0x" << endl;
	for (size_t i = 0; i < newLen; i += 2){
		cout << hex << ecbResult[i + 1] << " " << ecbResult[i] << endl;
	}

	ecbDec = speck_decrypt_ecb(genKey, ecbResult, true, newLen, newLen);
	cout << "Decrypted ECB stream : 0x" << endl;
	for (size_t i = 0; i < newLen; i += 2){
		cout << hex << ecbDec[i + 1] << " " << ecbDec[i] << endl;
	}


	str = transform_uint64_array(ecbDec, true, newLen);
	cout << "string: 0x";
	for(unsigned char i : str){
		cout << hex << (unsigned int) i;
	}
	cout << endl;
	delete[] newBuffer;
	delete[] ecbResult;
	delete[] ecbDec;

	cout << "----------------------------" << endl << endl;

	//final string test
	str = "Hello World!";
	cout << "Final Hello World string test " << endl;
	cout << "string : " << str << endl;
	newBuffer = transform_string(str, true, newBufferSize);
	ecbResult = speck_encrypt_ecb(genKey, newBuffer, true, newBufferSize, newLen);

	cout << "Encrypted ECB stream : 0x" << endl;
	for (size_t i = 0; i < newLen; i += 2){
		cout << hex << ecbResult[i + 1] << " " << ecbResult[i] << endl;
	}

	ecbDec = speck_decrypt_ecb(genKey, ecbResult, true, newLen, newLen);
	cout << "Decrypted ECB stream : 0x" << endl;
	for (size_t i = 0; i < newLen; i += 2){
		cout << hex << ecbDec[i + 1] << " " << ecbDec[i] << endl;
	}
	string newStr = transform_uint64_array(ecbDec, true, newLen);
	cout << "Decrypted string: " << newStr << endl;


	return 0;
}
