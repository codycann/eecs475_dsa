Team 34 (CAR)

Speck Encryption
vector<uint64_t> encrypt(const vector<uint64_t> &key,
	const vector<uint64_t> &plain_text)
The encrypt function takes a vector of uint64_t as the key and a vector of
uint64_t as the plain_text. The key vector must contain 4-64 bit numbers that
make up the whole 256 bit key. Our function does the Speck key expansion
before encryption. The plain_text must be of size 2 or, in other words, a
block size of 128 bits. Our encrypt function does the Speck 128/256
encryption, which has 34 rounds.

Speck Decryption
vector<uint64_t> decrypt(const vector<uint64_t> &key,
	const vector<uint64_t> &cipher_text)
The decrypt function takes a vector of uint64_t as the key and a vector of
uint64_t as the cipher_text. The key vector must contain 4-64 bit numbers that
make up the whole 256 bit key. Our function does the Speck key expansion
before decryption. The plain_text must be of size 2 or, in other words, a
block size of 128 bits. Our decrypt function does the Speck 128/256
decryption, which has 34 rounds.
