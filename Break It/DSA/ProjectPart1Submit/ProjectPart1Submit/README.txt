EECS475 Project
Cipher: DSA
Team #30 Professor Fu's Army
Uniqnames: adilip, desaiar, zickd
Discussion Section: Fri 10:30am

/******** Compiling and running **********/
To compile our project we recommend using the makefile
1.) make clean; make  (compiles)
2.) build/main (runs)
Note: If you chose to rewrite main and put it in a different file you
	will need to change the program source in the makefile to your
	new file. To run with your own file: build/filename

/********* Function Signatures ***********/
A list of function signatures is contained in dsa.h

Key Generation:
To generate the keys for DSA give two bitlengths (p and q) type = integers. Two values have already
been defaulted if you chose not to declare your own.
The public keys P, Q, G, Y will be set in global variables.
To print them call the function printPublic()
The private key x will be the return value of keyGeneration
Example:
mpz_class privateKey = keyGeneration();
mpz_class privateKey = keyGeneration(32, 16);

Encryption:
To perfrom the encryption/signature your two parameters need to be the
private key x from keyGeneration and the plaintext you wish to sign.
Both should be given as mpz_class.
The output of the encryption is a pair of mpz_class that holds r and s
Example:
mpz_class plaintext = 1234; mpz_class privateKey;
pair<mpz_class, mpz_class> signature = encrypt(privateKey, plaintext);

Verify:
The inputs to verify should be the r and s from the signature and also
the plaintext you wish to verify as real.
r and s should be given as a pair of mpz_class, plaintext should be given as
an mpz_class
If it returns true then the plaintext you are verifying is the one that was
encrypted.
Example:
pair<mpz_class, mpz_class> signature; mpz_class plaintext;
bool verified = verify(signature, plaintext);

/************ Performance *************/
We tested performance using performance.cpp
This test ran various bitlenghts to generate the keys and then determined to both sign
and verify. The results are located in performace.xlxs (an excel spreadsheet) and 
Data.pdf (a graph of our results)
By looking at our graph data it appears that encryption (including the key generation) is increasing exponentially.
The decryption which simply checks some math appears to have a constant runtime over the bitlenghts.

The test vectors for our cipher are located in main.cpp
