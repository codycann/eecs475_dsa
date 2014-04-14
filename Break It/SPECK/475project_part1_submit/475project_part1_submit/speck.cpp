#include <iostream>
#include <stdlib.h>
#include <time.h>
#include "uberzahl.h"
//#include "uberzahl.cpp"

using namespace std;

// choose key 128bit key size and 128 block size
const int WORDSIZE = 64;
const int KEYNUM = 2;
const int ALPHA = 8;
const int BETA = 3;
const int ROUNDNUM = 32;
uberzahl sizeMod = "18446744073709551615"; // 2^WORDSIZE - 1

// structure to define key 
struct SpeckKey
{
	uberzahl k0;
	uberzahl l0;
};

// generate a random key
SpeckKey keyGen(){
	SpeckKey key;
	srand(time(0));
	key.k0.random(WORDSIZE);
	key.l0.random(WORDSIZE);
	return key;
}

//expand the entire key given initial k[0] and l[0]
void keyExpan(uberzahl k[], uberzahl l[], SpeckKey &key){

	k[0] = key.k0;
	l[0] = key.l0;
	for (int i = 0; i <= ROUNDNUM - 2; i++){
		l[i + KEYNUM - 1] = ((k[i] + l[i].rotateRight(ALPHA, 0, WORDSIZE - 1)) & sizeMod) ^ i;
		k[i + 1] = k[i].rotateLeft(BETA, 0, WORDSIZE - 1) ^ l[i + KEYNUM - 1];
	}

}

//break text into blocks
void textBreak(uberzahl &plaintext, uberzahl &x, uberzahl &y){

	y = plaintext & sizeMod;
	x = plaintext >> WORDSIZE;
}

// recombine blcoks into text
uberzahl textCombine(uberzahl &x, uberzahl &y){

	return (x << WORDSIZE) | y;
}


//given plaintext x and y, encryt them
uberzahl encrypt(SpeckKey &key, uberzahl &plaintext){

	uberzahl k[ROUNDNUM];
	uberzahl l[ROUNDNUM];
	keyExpan(k, l, key);

	uberzahl x, y;
	textBreak(plaintext, x, y);
	for (int i = 0; i < ROUNDNUM; i++){
		x = ((x.rotateRight(ALPHA, 0, WORDSIZE - 1) + y) & sizeMod) ^ k[i];
		y = y.rotateLeft(BETA, 0, WORDSIZE - 1) ^ x;
	}

	return textCombine(x, y);
}

// return decrypting message
uberzahl decrypt(SpeckKey &key, uberzahl &ciphertext){

	uberzahl k[ROUNDNUM];
	uberzahl l[ROUNDNUM];
	keyExpan(k, l, key);

	uberzahl x, y;
	textBreak(ciphertext, x, y);

	for (int i = ROUNDNUM - 1; i >= 0; i--){
		y = (x ^ y).rotateRight(BETA, 0, WORDSIZE - 1);
		x = (((x ^ k[i]) + sizeMod + 1 - y) & sizeMod).rotateLeft(ALPHA, 0, WORDSIZE - 1);

	}

	return textCombine(x, y);
}



int main(){
	

	// Commented out codes below is using the sample test vector of 128 bit, 
	// you can use it to check with the sample result in the spec
	// SpeckKey key;
	// key.l0 = "1084818905618843912";
	// key.k0 = "506097522914230528";
	// uberzahl x = "7809653424151160096";
	// uberzahl y = "8388271400802151712";
	// uberzahl plaintext = textCombine(x, y);
	// cout << "x = " << x << " y = " << y << endl;
 
	// Below are codes for time testing, which are commented out 
	// uberzahl abc;
	// srand(time(0));
	// uberzahl plaintext = abc.random(32);


	SpeckKey key = keyGen();
	uberzahl plaintext = "144062678019685919772856771483193666848";
	cout << "plaintext: " << plaintext << endl;
	uberzahl ciphertext;
	//clock_t start = clock();
	//for (int i =0; i<1000; i++)
	ciphertext = encrypt(key, plaintext);
	//clock_t end = clock();
	//cout << "It took " << (end-start)*1000/CLOCKS_PER_SEC << " milliseconds" << endl;

	cout << "ciphertext: " << ciphertext <<endl;
	uberzahl decrypttext;
	//clock_t start2 = clock();

	//for (int i =0; i<1000; i++)
	decrypttext = decrypt(key, ciphertext) ;

	//clock_t end2 = clock();
   	//cout << "It took " << (end2-start2)*1000/CLOCKS_PER_SEC << " milliseconds" << endl;
   
	cout << "decrypted plaintext: " << decrypttext <<endl;
	

	return 0;


}
