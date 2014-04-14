#include <iostream>
#include <cstdlib>
#include <string>
#include <algorithm>
#include <random>
#include <gmpxx.h>
#include "dsa.h"
#include "sha256.h"

using namespace std;


int main() 
{
	//Test vector. This runs 100 key generations, signatures, and verifications
	//By plugging in the encryption output to the verification we should recieve a verified
	//signature.
	srand(time(NULL));
	mpz_class bitLength;
	bitLength = "256";
	gmp_randclass rr(gmp_randinit_default);
	rr.seed(time(NULL));

	cout << "***Running correctness testing***" << endl;
	for (int i = 0; i < 100; ++i) {
		mpz_class plaintext;
		plaintext = rr.get_z_bits(bitLength);
		mpz_class myX = keyGeneration();
		pair<mpz_class, mpz_class> encryptedMessage = encrypt(myX, plaintext);
		if (!(verify(encryptedMessage, plaintext))) 
			{
				cout << "ERROR: Could not verify\n";
				exit(0);
			}
	}
	cout << "Passed correctness testing\n" << endl;

	//This runs 100 fake signautres and tests that the verify function realizes they are not
	//valid
	cout << "***Catching bad inputs ***" << endl;
	for (int i = 0; i < 100; ++i) {
		mpz_class fakeR, fakeS;
		fakeR = rr.get_z_bits(bitLength);
		fakeS = rr.get_z_bits(bitLength);
		pair<mpz_class, mpz_class> fakeSignature(fakeR, fakeS);
		mpz_class plaintext;
		plaintext = rr.get_z_bits(bitLength);
		mpz_class myX = keyGeneration();
		if ((verify(fakeSignature, plaintext))) {
			cout << "ERROR: Did not catch faulty signature" << endl;
			exit(0);
		}
	}
	cout << "Passed faulty input testing" << endl;
	
	return 0;
}







