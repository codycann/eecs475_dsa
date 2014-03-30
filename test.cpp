#include "DSA.h"
#include "uberzahl/uberzahl.h"
#include <iostream>
using namespace std;


int main(){
	// test internals of DSA (Montgomery, modexp, etc.)
	testDSA();

	// test signing and verification
	uberzahl privateKey;
	PublicKey publicKey (1024, 160, privateKey);
	uberzahl m = 1397642052; // ASCII representation of string "SNSD"
	Signature sig = sign(publicKey, privateKey, m);
	if(verify(sig, publicKey, m)) {
		cout << "Successfuly verified!\n";
	} else {
		cout << "Did not successfuly verify :(\n";
	}

	return 0;
}
