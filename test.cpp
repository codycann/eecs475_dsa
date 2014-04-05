#include "DSA.h"
#include "uberzahl.h"
#include <iostream>
#include <cstdlib>
#include <string>
#include <sstream>
#include <cassert>
using namespace std;


int main(){
    srand(time(0));

	// test internals of DSA (Montgomery, modexp, etc.)
	// testDSA();

	for(int i = 0; i < 50; i++) {
		uberzahl privateKey;
		PublicKey publicKey(1024, 160, privateKey);
		uberzahl message ("47433820293");
		Signature sig = sign(publicKey, privateKey, message);

		if(!verify(sig, publicKey, message)) {
			cout << "Did not successfuly verify :(\n";
			abort();
		}

		cout << "g: " << publicKey.g << endl;
		cout << "p: " << publicKey.p << endl;
		cout << "q: " << publicKey.q << endl;
		cout << "y: " << publicKey.y << endl;
		cout << "N: " << publicKey.N << endl;
		cout << "r :" << sig.r << endl;
		cout << "s :" << sig.s << endl;

		PublicKey wrongP = publicKey;
		stringstream p;
		p << wrongP.p;
		mpz_class mpz_p (p.str());
		mpz_nextprime(mpz_p.get_mpz_t(), mpz_p.get_mpz_t());
		wrongP.p = mpz_p;

		PublicKey wrongQ = publicKey;
		stringstream q;
		q << wrongQ.q;
		mpz_class mpz_q (q.str());
		mpz_nextprime(mpz_q.get_mpz_t(), mpz_q.get_mpz_t());
		wrongQ.q = mpz_q;

		PublicKey wrongG = publicKey;
		wrongG.g = wrongG.g + 1;

		PublicKey wrongY = publicKey;
		wrongY.y = wrongY.y + 1;

		PublicKey wrongN = publicKey;
		wrongN.N = wrongN.N + 1;

		if(verify(sig, wrongP, message)) {
			cout << __LINE__ << endl;
			abort();
		}

		if(verify(sig, wrongQ, message)) {
			cout << __LINE__ << endl;
			abort();
		}

		if(verify(sig, wrongG, message)) {
			cout << __LINE__ << endl;
			abort();
		}

		if(verify(sig, wrongY, message)) {
			cout << __LINE__ << endl;
			abort();
		}

		if(verify(sig, wrongN, message)) {
			cout << __LINE__ << endl;
			abort();
		}

		if(verify(sig, publicKey, message + 1)) {
			abort();
			cout << __LINE__ << endl;
		}

		if(verify(sig, publicKey, message - 1)) {
			abort();
			cout << __LINE__ << endl;
		}

		if(verify(sig, publicKey, message * 2)) {
			cout << __LINE__ << endl;
			abort();
		}

		Signature wrongR = sig;
		wrongR.r = wrongR.r + 1;

		Signature wrongS = sig;
		wrongS.s = wrongS.s + 1;

		if(verify(wrongR, publicKey, message)) {
			cout << __LINE__ << endl;
			abort();
		}

		if(verify(wrongS, publicKey, message)) {
			cout << __LINE__ << endl;
			abort();
		}

		// TODO more combinations and stuff

	}

	return 0;
}
