//Implementation of SPECK 128/128 for EECS475 Project
#include "speck.h"

/* IMPORTANT NOTE ABOUT ALL FUNCTIONS:
All functions treat the 128 bit inputs as little endian. What this means is that buffer[0] is
the least significant 64 bits, and buffer[1] is the most significant 64 bits. */

//round function for encryption
inline void round(uint64_t &x, uint64_t &y, uint64_t &k){
	x = _lrotr(x, 8);
	x += y;
	x ^= k;
	y = _lrotl(y, 3);
	y ^= x;
}

//round function for decryption
inline void inverse_round(uint64_t &x, uint64_t &y, uint64_t &k){
	y ^= x;
	y = _lrotr(y, 3);
	x ^= k;
	x -= y;
	x = _lrotl(x, 8);
}

/* Uses openssl's RAND_bytes to generate cryptographically strong bytes for generating a 128 bit key stored 
into the given key buffer.
On failure, will return -1 with an error message printed to stdout. On success, returns 0. */
int speck_generate_key(uint64_t* key){
	if (key == nullptr) {
		return -1;
	}

	unsigned char tempKey[16];
	int rc = RAND_bytes(tempKey, sizeof(tempKey));

	if (rc != 1) {
		//openssl error thrown
		ERR_print_errors_fp(stdout);
		return -1;
	}

	//convert char[] key to uint64_t[] key
	for (int i = 0; i < 2; i++){
		for (int j = 0; j < 8; j++){
			key[i] = (key[i] << 8) | tempKey[j + i * 8];
		}
	}

	return 0;
}

/* Expands the given 128 bit key into the 2048 bits of round key used in encryption/decryption.
The length of the expanded_key buffer must be 32 (2048 bits).
Returns -1 if key or expanded_key is nullptr. On success, returns 0.*/
int speck_expand_key(uint64_t* key, uint64_t* expanded_key){
	if (key == nullptr || expanded_key == nullptr){
		return -1;
	}

	uint64_t l[32];
	expanded_key[0] = key[0];
	l[0] = key[1];

	for (uint64_t i = 0; i < 31; i++){
		expanded_key[i+1] = expanded_key[i];
		l[i+1] = l[i];
		round(l[i+1], expanded_key[i+1], i);
	}
	return 0;
}

/* Encrypts the given plaintext buffer with the given expanded key, stores the result in the given ciphertext buffer. 
If any pointers are nullptr, returns -1. On success, returns 0.
NOTE: plaintext[1] is the most significant part of the 128 bit block, and plaintext[0] is the least
significant part of the block, when comparing with public test vectors & other implementations.
In short, this means that the 128 bit blocks are little endian in respect to the 64 bit words.	
This follows the given implementation in the SPECK Paper in appendix A.*/
int speck_encrypt(uint64_t* plaintext, uint64_t* ciphertext, uint64_t* expanded_key) {
	if (plaintext == nullptr || ciphertext == nullptr || expanded_key == nullptr){
		return -1;
	}

	ciphertext[0] = plaintext[0];
	ciphertext[1] = plaintext[1];
	for (uint64_t i = 0; i < 32; i++){
		round(ciphertext[1], ciphertext[0], expanded_key[i]);
	}

	return 0;
}

/* Decrypts the given ciphertext buffer with the given expanded key into the given plaintext buffer.
If any pointers are nullptr, returns -1. On success, returns 0. */
int speck_decrypt(uint64_t* plaintext, uint64_t* ciphertext, uint64_t* expanded_key) {
	if (plaintext == nullptr || ciphertext == nullptr || expanded_key == nullptr){
		return -1;
	}

	plaintext[0] = ciphertext[0];
	plaintext[1] = ciphertext[1];
	for (uint64_t i = 0; i < 32; i ++){
		inverse_round(plaintext[1], plaintext[0], expanded_key[31-i]);
	}

	return 0;
}


/* Encrypts the given 64 bit stream using ECB mode with the given key.
Adds padding if enable_padding is true, otherwise returns the stream as is (requires len is a multiple of 2).
On success, returns the ciphertext buffer in a dynamically allocated buffer. 
On failure, returns nullptr. new_len represents the new (padded) size.*/
uint64_t* speck_encrypt_ecb(uint64_t* key, uint64_t* pt, bool enable_padding, size_t len, size_t &new_len){
	if(key == nullptr || pt == nullptr){
		return nullptr;
	}

	if (len == 0) {
		return nullptr;
	}

	if (!enable_padding && (len & 0x1) == 1) {
		return nullptr;
	}

	uint64_t* ct;
	bool padOne = false;

	//Pad to length multiple of 2
	if (enable_padding) {
		if ((len & 0x1) == 1) {
			new_len = len + 1;
			padOne = true;
		} else {
			new_len = len + 2; 
		}
	} else {
		new_len = len;
	}

	ct = new uint64_t[new_len];

	//encode amount padded in end of ciphertext
	if(enable_padding){
		if(padOne){
			ct[new_len - 1] = 1; 
		} else {
			ct[new_len - 2] = 2;
			ct[new_len - 1] = 2;
 		}
	}

	uint64_t expanded_key[32];
	speck_expand_key(key, expanded_key);

	for(size_t i = 0; (i + 1) < len; i +=2){
		speck_encrypt(pt + i, ct + i, expanded_key);
	}

	//Encrypt last block(s) if padded
	if (enable_padding){
		if(padOne){
			ct[new_len - 2] = pt[new_len - 2];
			speck_encrypt(ct + new_len - 2, ct + new_len - 2, expanded_key);
		} else {
			speck_encrypt(ct + len, ct + len, expanded_key);
		}
	}

	return ct;
}

/* Decrypts the given 64 bit stream using ECB mode with the given key.
Removes padding if enable_padding is true, otherwise returns the stream as is. 
On success, returns the ciphertext buffer in a dynamically allocated buffer. 
On failure, returns nullptr. new_len represents the new (unpadded) size.*/
uint64_t* speck_decrypt_ecb(uint64_t* key, uint64_t* ct, bool enable_padding, size_t len, size_t &new_len){
	if(key == nullptr || ct == nullptr){
		return nullptr;
	}

	if (len == 0) {
		return nullptr;
	}

	if ((len & 0x1) == 1) {
		return nullptr;
	}

	uint64_t* pt = new uint64_t[len];
	uint64_t expanded_key[32];
	speck_expand_key(key, expanded_key);

	for (size_t	i = 0; i < len; i += 2){
		speck_decrypt(pt + i, ct + i, expanded_key);
	}

	new_len = len;
	if (enable_padding){
		//remove padding
		if (pt[len - 1] == 2){
			new_len -= 2;
		} else {
			new_len--;
		}
	}

	return pt;
}

/* Converts the given string to a dynamically allocated uint64_t array, concatinating blocks of 8 bytes into a single 64 bit word
using little endian encoding.
If enable_padding is true, pads the output to a final size being a multiple of two, for use in ECB encryption.
If enable_padding is false, returns nullptr if buffer size is not a multiple of 8. 
Returns nullptr if buffer is empty. */
uint64_t* transform_string(std::string buffer, bool enable_padding, size_t &new_size){
	if (buffer.empty()){
		return nullptr;
	}

	if (!enable_padding && (buffer.size() % 8 != 0)){
		return nullptr;
	}

	char pad_amount = 0;

	//Use PKCS#7 byte level padding
	if (enable_padding) {
		pad_amount = 16 - (buffer.size() % 16);
		buffer.append(pad_amount, pad_amount);
		buffer.back() = pad_amount;
	}

	new_size = buffer.size() / 8;

	uint64_t* new_buffer = new uint64_t[new_size];

	for (size_t i = 0; i < new_size; i++){
		new_buffer[i] = 0;
		for (int j = 7; j >= 0; j--){
			new_buffer[i] = (new_buffer[i] << 8) | (unsigned char) buffer[i * 8 + j];
		}
	}

	return new_buffer;
}

/* Converts the given uint64_t array to std::string, breaking up each 64 bit word into a stream of chars using 
little endian encoding.
If enable_padding is true, cuts all padding off at the end of the buffer.
If enable_padding is false, returns the string as-is.
Returns an empty string if len is 0. */
std::string transform_uint64_array(uint64_t* buffer, bool enable_padding, size_t len){
	std::string new_buffer;

	if(len == 0) {
		return new_buffer;
	}

	new_buffer.reserve(len * 8);

	uint64_t temp;

	for (size_t i = 0; i < len; i++){
		temp = buffer[i];
		for(int j = 0; j < 8; j++){
			new_buffer.push_back(temp);
			temp = temp >> 8;
		}
	}

	//unpad if necessary
	if(enable_padding){
		size_t pad_amount = new_buffer.back();
		new_buffer.resize(new_buffer.size() - pad_amount);
	}

	return new_buffer;
}
