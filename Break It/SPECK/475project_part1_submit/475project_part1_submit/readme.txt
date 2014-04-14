Implement: Speck Algorithm 128 key size and 128 block size


Usage: use keyGen() function to generate key(defined in SpeckKey struct)
	   set plaintext to any uberzahl number less than or equal to 128 bit
	   call encrypt method with key and plaintext and the function will return the ciphertext(type uberzahl)
	   call decrypt method with key and ciphertext and the function will return the plaintext(type uberzahl)

Compile Command: g++ speck.cpp uberzahl.h uberzahl.cpp
Attachments: the performance plot and the test vector are attached, there are two copy of test vector file, one saved in pdf, the other xlsx, in case it's hard to read.
