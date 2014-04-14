#include <iostream>
#include "uberzahl.h"
#include <time.h>
#include <random>
#include <cstdlib>

using namespace std;

const int bitSize = 100;
uberzahl negone = (long long) -1;
uberzahl zero = "0";
uberzahl one = "1";
uberzahl two = "2";
uberzahl three = "3";
uberzahl four = "4";

struct cipherText //contains r and s as two components of ciphertxt
	{
		uberzahl r;
		uberzahl s;
	};


class ElGamal
{
public:
	uberzahl y;
	uberzahl p;
	uberzahl g;
	cipherText c;
	void Setup()
	{
		srand(time(NULL));
		g = "67276968433851008563610060648503964734"; //128 bit prime same prime and generator are used everytime to improve runtime as these parameters are public either way
		p = "305023830679022983343692810412890660703";
		x.random(bitSize+1);
		x = (x % (p - four)) + two; //constraint on x 2 <= x <= p-2
		y = modExp(g, x, p);
	}

	cipherText Encryption(uberzahl M) //encryption mechanism requires message < 128 bits
	{
		srand(time(NULL));
		uberzahl k;
		k.random(bitSize+1); //k is neither public nor private
		
		k = (x % (p - four)) + two;
		c.r = modExp(g, k , p); //struct is used so that both r and s parameters can be passed
		c.s = (M * modExp(y, k ,p)) % p;


		return c;
	}

	uberzahl Decryption(cipherText c) //decryption mechanism, pass cipher text struct from Encryption
	{
		uberzahl M; //original message
		uberzahl rX = modExp(c.r, x, p);
		M = (c.s * rX.inverse(p)) % p;

		return M;
	}

	uberzahl modExp(uberzahl base, uberzahl exponent, uberzahl modulus) //modular exponentiation
	{
		uberzahl result = one;
		while(exponent > zero)
		{
			if (exponent % two == one)
			{
				result = (result*base) % modulus;
			}

		exponent = exponent >> 1;
		base = (base * base ) % modulus;
		}

		return result;
	}

private:
	uberzahl x; //private member that is randomly selected in setup


};

int main()
{
	//complile with -std=c++0x!!!
	srand(time(NULL));
	ElGamal cipher;
	cipherText txt;
 	
	//random bit size can be any number <= 128 otherwise message must be split
	// test cases
	uberzahl message;
	message.random(128);
	cout << message << endl;

	cipher.Setup();
	txt = cipher.Encryption(message);
	cout << "ciphertxt is: " << txt.r << txt.s << endl;

	cout << "original message: " << cipher.Decryption(txt) << endl;
	

	return 0;
}





