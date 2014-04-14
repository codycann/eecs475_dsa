Team 5, Bob Alice and Co. mfstep, joeherm, byoshi
Cipher: GCM

Compilation: Type make

NOTE: Since Uberzahl trims leading 0's, IV, KEY, and AAD cannot be bitstrings of only zeroes. (e.g. 0000000). "For IVs, it is recommended that implementations restrict support to the length of 96 bits, to
promote interoperability, efficiency, and simplicity of design."


Generate an AES key/iv using the following command:
	openssl enc -aes-128-gcm -k secret -P -md sha1

Sample Key/IV/plaintext:
		key=e5b5763c6f9fd7f1a6a68af9435ca
		iv=3915ef469e65e4c6ba13bea
		plaintext=d9313225f88406e5a55909c5aff5269a

Encryption execution: ./gcm -encrypt [128-bit AES key] [96-bit IV] [Additional Authenticated Data (must be same for encryption and decryption)] [plaintext]
Decryption execution: ./gcm -decrypt [128-bit AES key] [96-bit IV] [Additional Authenticated Data (must be same for encryption and decryption)] [ciphertext] [tag]
Generate AES key/IV:	./gcm -keygen
