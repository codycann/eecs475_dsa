#ifndef SPECK2_H_
#define SPECK2_H_

#include <stdlib.h>
#include <time.h>
#include <iostream>
#include "uberzahl.h"

using namespace std;

#define n 64		//word size
#define m 2			//number of key words
#define T 32		//number of rounds
#define alpha 8		//alpha
#define beta 3		//beta
static const uberzahl mod = 0xffffffffffffffff;	// 2^64 - 1

class Speck2 {

	private:
		uberzahl halfMod;
		std::vector<uberzahl> l;
		std::vector<uberzahl> k;

	public:
		Speck2(uberzahl x, uberzahl y);

		uberzahl ct_x, ct_y;

//		void keyGen();
		void keyExpand();
		void encrypt();
		void decrypt();
		void setKeys(uberzahl k1, uberzahl k0);
		void cmdLineParse(int argc, char **argv);
};

Speck2::Speck2(uberzahl x, uberzahl y) {

	l.reserve(T);
	k.reserve(T);

	this->ct_x = x;
	this->ct_y = y;

	halfMod = 1;
	halfMod = halfMod << n;
}

void keyGen()
{
	srand(time(nullptr));

	mediumType bits = 64;
	uberzahl randomNumber;

	randomNumber.random(bits);
	cout << randomNumber << ' ';

	randomNumber.random(bits);
	cout << randomNumber << '\n';
}

void Speck2::keyExpand() {
	for(unsigned i = 0; i < (T - 1); ++i) {
		this->l[i+m-1] =
		( (this->k[i] + this->l[i].rotateRight(alpha, 0, n - 1) ) & mod) ^ i;	//use mod to keep 64bits

		this->k[i+1] =
		this->k[i].rotateLeft(beta, 0, n - 1) ^ this->l[i+m-1];
	}
}

void Speck2::encrypt() {
	for(unsigned i = 0; i < T; ++i) {
		this->ct_x =
			( (this->ct_x.rotateRight(alpha, 0, n - 1)
				+ this->ct_y) & mod) ^ this->k[i];	//use mod to keep 64bits
		this->ct_y =
			this->ct_y.rotateLeft(beta, 0, n - 1) ^ this->ct_x;
	}
}

void Speck2::decrypt() {
	for(int i = T - 1; i >= 0; i--) {
		uberzahl temp;

		temp = this->ct_x;
		this->ct_x = (this->ct_x ^ this->ct_y).rotateRight(beta, 0, n-1);
		this->ct_y = temp;

		this->ct_y = this->ct_y ^ k[i];

		temp = this->ct_x;
		while(this->ct_y < this->ct_x)
		{
			this->ct_y = this->ct_y + halfMod;
		}
		this->ct_x =
			((this->ct_y - this->ct_x) % halfMod).rotateLeft(alpha, 0, n-1);
		this->ct_y = temp;
	}
}

void Speck2::setKeys(uberzahl k1, uberzahl k0) {
	l[0] = k1;
	k[0] = k0;
}

#endif /* SPECK2_H_ */
