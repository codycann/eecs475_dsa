#include "DSA.h"
#include "uberzahl.h"
#include <iostream>
#include <cstdlib>
#include <string>
#include <sstream>
#include <cassert>
using namespace std;

void runTest();

int main(){
	// seed randomness
    srand(time(0));

	// test internals of DSA (Montgomery, modexp, etc.)
	// testDSA();
	// return 0;

	// test signing and verification
	// uberzahl privateKey;
	// PublicKey publicKey (1024, 160, privateKey);
	// // PublicKey publicKey(privateKey);
	// cout << "PUBLIC KEY ========================\n";
	// cout << "g: " << publicKey.g << endl;
	// cout << "p: " << publicKey.p << endl;
	// cout << "q: " << publicKey.q << endl;
	// cout << "y: " << publicKey.y << endl;
	// cout << "M: " << publicKey.M << endl;
	// cout << "PUBLIC KEY ========================\n";
	// uberzahl m = 1397642052; // ASCII representation of string "SNSD"
	// Signature sig = sign(publicKey, privateKey, m);
	// cout << "SIGNATURE ========================\n";
	// cout << "r :" << sig.r << endl;
	// cout << "s :" << sig.s << endl;
	// cout << "SIGNATURE ========================\n";

	for(int i = 0; i < 50; i++) {
		uberzahl privateKey;
		PublicKey publicKey(1024, 160, privateKey);
		uberzahl message ("47433820293");
		Signature sig = sign(publicKey, privateKey, message);

		if(!verify(sig, publicKey, message)) {
			cout << "Did not successfuly verify :(\n";
			assert(false);
		}

		PublicKey wrongP = publicKey;
		stringstream p;
		p << wrongP.p;
		mpz_class mpz_p (p.str());
		mpz_nextprime(mpz_p.get_mpz_t(), mpz_p.get_mpz_t());
		wrongP.p = mpz_p;
		cout << __LINE__ << endl;

		PublicKey wrongQ = publicKey;
		stringstream q;
		q << wrongQ.q;
		mpz_class mpz_q (q.str());
		mpz_nextprime(mpz_q.get_mpz_t(), mpz_q.get_mpz_t());
		wrongQ.q = mpz_q;
		cout << __LINE__ << endl;

		PublicKey wrongG = publicKey;
		wrongG.g = wrongG.g + 1;
		cout << __LINE__ << endl;

		PublicKey wrongY = publicKey;
		wrongY.y = wrongY.y + 1;
		cout << __LINE__ << endl;

		PublicKey wrongN = publicKey;
		wrongN.N = wrongN.N + 1;
		cout << __LINE__ << endl;

		// if(verify(sig, wrongP, message))
		// 	assert(false);
		// cout << __LINE__ << endl;

		// if(verify(sig, wrongQ, message))
		// 	assert(false);
		// cout << __LINE__ << endl;

		// if(verify(sig, wrongG, message))
		// 	assert(false);
		// cout << __LINE__ << endl;

		// if(verify(sig, wrongY, message))
		// 	assert(false);
		// cout << __LINE__ << endl;

		if(verify(sig, wrongN, message))
			assert(false);

		if(verify(sig, publicKey, message + 1))
			assert(false);

		if(verify(sig, publicKey, message - 1))
			assert(false);

		if(verify(sig, publicKey, message * 2))
			assert(false);

		Signature wrongR = sig;
		wrongR.r = wrongR.r + 1;
		cout << __LINE__ << endl;

		Signature wrongS = sig;
		wrongS.s = wrongS.s + 1;
		cout << __LINE__ << endl;

		if(verify(wrongR, publicKey, message))
			assert(false);
		cout << __LINE__ << endl;

		if(verify(wrongS, publicKey, message))
			assert(false);
		cout << __LINE__ << endl;

		// TODO more combinations and stuff

	}

	return 0;
}

void runTest();
