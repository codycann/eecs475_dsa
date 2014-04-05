#include "DSA.h"
#include "uberzahl.h"
#include <iostream>
#include <cstdlib>
using namespace std;


int main(){
	// seed randomness
    srand(time(0));

	// test internals of DSA (Montgomery, modexp, etc.)
	// testDSA();
	// return 0;

	// test signing and verification
	uberzahl privateKey;
	PublicKey publicKey (1024, 160, privateKey);
	// PublicKey publicKey(privateKey);
	cout << "PUBLIC KEY ========================\n";
	cout << "g: " << publicKey.g << endl;
	cout << "p: " << publicKey.p << endl;
	cout << "q: " << publicKey.q << endl;
	cout << "y: " << publicKey.y << endl;
	cout << "M: " << publicKey.M << endl;
	cout << "PUBLIC KEY ========================\n";
	uberzahl m = 1397642052; // ASCII representation of string "SNSD"
	Signature sig = sign(publicKey, privateKey, m);
	cout << "SIGNATURE ========================\n";
	cout << "r :" << sig.r << endl;
	cout << "s :" << sig.s << endl;
	cout << "SIGNATURE ========================\n";

	if(verify(sig, publicKey, m)) {
		cout << "Successfuly verified!\n";
	} else {
		cout << "Did not successfuly verify :(\n";
	}

	return 0;
}
