#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>
#include <numeric>
#include <map>
#include <time.h>
#include "uberzahl.cpp"
#include <cstdlib>

using namespace std;

class elGamal{
	uberzahl prime;
	uberzahl minPrime;
	uberzahl maxPrime;
	uberzahl generator;
	uberzahl x;
	uberzahl y;
	
	uberzahl gen(uberzahl p)
	{
		//Function to find a generator of a given prime
		srand(time(NULL));
		uberzahl g;
		while(1)
		{
			uberzahl low = 1;
			uberzahl high = p - 1;
			g = random(low,high);
			if(g.expm((p-1)/2, p) != 1 && g.expm( 2, p) != 1)
			{
				return g;
			}
		}
	}
	
	uberzahl getPrime(uberzahl min, uberzahl max)
	{
		//Function to find a safe prime
		while(1)
		{
			uberzahl p;
			p = random(min, max);
			smallType k = 2;
			uberzahl p2 = p + p + 1;
			if(rabinmiller(p,k) && rabinmiller(p2,k));
			{
				return p2;
			}
		}
	}
	
	public:
	
	elGamal()
	{
		//Initializes all of the required values using a 128-bit prime
		srand(time(NULL));
		uberzahl temp = 2;
		minPrime = temp.exp(126);
		temp = 2;
		maxPrime = temp.exp(127);
		prime = getPrime(minPrime, maxPrime);
		uberzahl low = 2;
		uberzahl high = prime - 2;
		x = random(low, high);
		generator = gen(prime);
		y = generator.expm(x, prime);
	}

	void encrypt(const uberzahl &m, uberzahl &r, uberzahl &s)
	{
		//Encrypts m,
		//Modifies r, s to the outputs of the encryption
		uberzahl low = 2;
		uberzahl high = prime - 2;
		uberzahl k = random(low, high);
		r = generator.expm(k, prime);
		s = m * y.expm(k,prime) % prime;	
	}
	
	uberzahl decrypt(const uberzahl &r, const uberzahl &s, uberzahl &m)
	{
		//Takes in r, s and decrypts them
		//modifies m to the decrypted ciphertext value
		uberzahl rx = r.expm(x, prime);
		m = s * rx.inverse(prime) % prime;
		
		return m;
	}
	
	void publicKey()
	{
		//Outputs the public key values
		cout << "g = " << generator << endl;
		cout << "p = " << prime << endl;
		cout << "y = " << y << endl;
	}
};
