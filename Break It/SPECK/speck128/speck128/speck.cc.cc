// EECS 475 Speck implementation
// group 36, saghevli, bradenrs, jiajuxu

#include "uberzahl/uberzahl.h"
#include <fstream>
#include <iostream>
#include <vector>
using namespace std;


#define n 64
#define m 2
#define T 32
#define a 8
#define b 3

vector<uberzahl> k(T-2 + m, (largeType) 0);
vector<uberzahl> l(T-2 + m, (largeType) 0);
uberzahl two64 = "18446744073709551616";

void keyexpand();
vector<uberzahl> keygen();
vector<uberzahl> encrypt(vector<uberzahl> key,vector<uberzahl> plaintext);
vector<uberzahl> decrypt(vector<uberzahl> key,vector<uberzahl> ciphertext);

int main(int argc, char* argv[]) {
	//Check for input
	if( argc < 2 ){
		cout << "Error: invalid command, possible commands:" << endl;
		cout << "\tkeygen" << endl;
		cout << "\tencrypt <key1> <key2> <plaintext1> <plaintext2>" << endl;
		cout << "\tdecrypt <key1> <key2> <ciphertext2> <ciphertext2>" << endl;
		return 1;
	}

	//Variables for inputs
	string input_command(argv[1]);
	vector<uberzahl> result(2, (largeType) 0);
	vector<uberzahl> key(2, (largeType) 0);
	vector<uberzahl> text(2, (largeType) 0);

	//Handle inputs
	if ( input_command == "keygen" ) {
		result = keygen();
	} else if ( argc == 6 ){
		key[0] = argv[2];
		key[1] = argv[3];
		text[0] = argv[4];
		text[1] = argv[5];
		if ( input_command == "encrypt" ) {
			result = encrypt( key, text );
		} else if( input_command ==  "decrypt" ) {
			result = decrypt( key, text );
		} else {
			cout << "Error: unrecognized command" << endl;
			return 1;
		}
	} else {
		cout << "Error: invalid argument count" << endl;
		return 1;
	}

	cout << result[0] << " " << result[1] << endl;
}

// generate and expand keys, adapted from algorithm in specification
void keyexpand() {
    for (int i = 0; i <= T-2; i++) {
        l[i+m-1] = ((k[i] + (l[i].rotateRight(a, 0, 63))) % two64) ^ i;
        k[i+1] = (k[i].rotateLeft(b, 0, 63)) ^ l[i+m-1];
    }
}

// generate 64 bit random numbers
vector<uberzahl> keygen(){
	vector<uberzahl> key(2, (largeType) 0);
	// seed from /dev/urandom
	ifstream random_file ("/dev/urandom", std::ifstream::in);
	char seed[4];
	random_file.read(seed, 4);
	random_file.close();
	srand((int)*seed);

	// use uberzahl random number generation, create a 65 bit
	// number and clear the top bit
	key[0].random(n + 1);
	key[0].clearBit(n);
	key[1].random(n + 1);
	key[1].clearBit(n);
	return key;
}

// encryption, first expand provided keys
// the vector contains the values x and y specified in the Speck specification
vector<uberzahl> encrypt(vector<uberzahl> key,vector<uberzahl> plaintext) {
	vector<uberzahl> c(2, (largeType) 0);
	c[0] = plaintext[0]; //x
	c[1] = plaintext[1]; //y
	l[0] = key[0];
	k[0] = key[1];
	keyexpand();
	for( int i = 0; i <= T-1; i++ ){
		c[0] = (((c[0].rotateRight(a, 0, 63)) + c[1]) % two64) ^ k[i];
		c[1] = (c[1].rotateLeft(b, 0, 63)) ^ c[0];
	}
	return c;
}

// encryption, first expand provided keys
// the vector contains the values x and y specified in the Speck specification
vector<uberzahl> decrypt(vector<uberzahl> key,vector<uberzahl> ciphertext) {
	vector<uberzahl> c(2, (largeType) 0);
	c[0] = ciphertext[0]; //x
	c[1] = ciphertext[1]; //y
	l[0] = key[0];
	k[0] = key[1];
	keyexpand();
	for( int i = T-1; i >= 0; i-- ){
		c[1] = (c[0] ^ c[1]).rotateRight(b, 0, 63);
		c[0] = ((((c[0] ^ k[i]) + two64) - c[1]) % two64 ).rotateLeft(a, 0, 63);
	}
	return c;
}
