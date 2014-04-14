This program implements the Speck 128/256 cipher. You can compile it with:

g++ -o Speck Speck.cpp main.cpp uberzahl.cpp -std=c++0x

and run it with:

./Speck -m GENERATE (generates a key)

./Speck -m ENCRYPT -t yourPlaintext -k yourKey (encrypts yourPlaintext using yourKey)

./Speck -m DECRYPT -t yourCiphertext -k yourKey (decrypts yourCiphertext using yourKey)

All flags are as follows:

-m, --mode
	required flag with a required argument, which must be one of GENERATE, ENCRYPT, and DECRYPT
	
-t, --text
	required flag for mode ENCRYPT and DECRYPT with a required argument, your plaintext or ciphertext
	
-k, --key
	required flag for mode ENCRYPT and DECRYPT with a required argument, your key
	
-h, --help
	it asks you to refer to this file...
	
-x, --hexadecimal
	optional flag if you prefer hexadecimal inputs and outputs
	
Important notes:

1. When your input is not long enough (128-bit for plaintext or ciphertext, and 256-bit for key), this
   program will add 0's at the beginnning. For example, plaintext "1234567887654321" in hexadecimal (64-bit)
   will be treated as "00000000000000001234567887654321". The same mechanism applies to form 128-bit blocks
   for plaintext or ciphertext.
   
2. Space marks as the end of the input, so "0000000000000000 1234567887654321" will be treated as
   "0000000000000000". Thus, for hexadecimal inputs, please enter the inputs with no space between the blocks.
