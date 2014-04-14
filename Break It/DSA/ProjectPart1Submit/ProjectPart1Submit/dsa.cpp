#include <iostream>
#include <cstdlib>
#include <string>
#include <gmpxx.h>
#include <algorithm>
#include <random>
#include "sha256.h"
#include "dsa.h"

using namespace std;

//GLOBALS
mpz_class P, Q, G, Y;

void printPublic()
{
//EFFECTS: prints public keys
	cout << "Printing public keys" << endl;
	cout << "P: " << P << endl;
	cout << "Q: " << Q << endl;
	cout << "G: " << G << endl;
	cout << "Y: " << Y << endl;
}


pair<mpz_class, mpz_class> encrypt(mpz_class key, mpz_class plaintext)
{
//Encryption Function
//
//EFFECTS: Encrypts the plaintext using the key. Outputs r and s
//         as a pair of mpz_classes. Ensures r and s are greater
//         than one. 

	mpz_class r, s, k, kinverse;
	
	gmp_randclass rr(gmp_randinit_default);
	rr.seed(time(NULL));
	
	do
	{
		k = rr.get_z_range (Q-1)+1;
		mpz_powm(r.get_mpz_t(), G.get_mpz_t(), k.get_mpz_t(), P.get_mpz_t());
		r = r % Q;
		mpz_invert(kinverse.get_mpz_t(), k.get_mpz_t(), Q.get_mpz_t());		
		string message = plaintext.get_str();
		s = ( (hash_message(message) + key * r) * kinverse) % Q;

	}
	while (r == 0 || s == 0 || r==1 || s==1);
	
	pair<mpz_class, mpz_class> encryption = make_pair(r,s);

	return encryption;
}


bool verify(pair<mpz_class, mpz_class> signature, mpz_class plaintext) 
{
//Verification Function
//
//EFFECTS: Uses the public keys Q, P, and Y to verify that the 
//         values of r and s contained in "signature" was correctly
//         encrypted with "plaintext". Outputs true if it was correctly
//         encrypted, and false if it wasn't.

	mpz_class z, w, u1, u2, v, v1, v2;

	if ((signature.first<=0) || (signature.first>=Q) || 
		(signature.second<=0) || (signature.second>=Q)) 
	{
		return false;
	}

	mpz_invert(w.get_mpz_t(), signature.second.get_mpz_t(), Q.get_mpz_t());
	z = hash_message(plaintext.get_str());

	u1 = (z*w) % Q;
	u2 = (signature.first*w) % Q;

	mpz_powm(v1.get_mpz_t(), G.get_mpz_t(), u1.get_mpz_t(), P.get_mpz_t());
	mpz_powm(v2.get_mpz_t(), Y.get_mpz_t(), u2.get_mpz_t(), P.get_mpz_t());
	v = ((v1*v2) % P);
	v = v % Q;
	
	return (v==signature.first);
}


mpz_class keyGeneration(int pbitLength, int qbitLength) 
{
//Key generation function
//
//REQUIRES: P, Q, G, Y global variables unchanged after signature  
//          set input mpz_class plaintext (unhashed version).
//          Pair must be order: (r, s)
//EFFECTS: Generates a prime P of bitlength "pbitlength",
//         and a prime Q of bitlength "qbitlength". P-1 is a 
//         multiple of Q. Also sets G to be a generator of 
//         of order Q mod P. Returns a random private key
//         "x", and sets Y to its appropriate value.

	gmp_randclass rr(gmp_randinit_default);
	rr.seed(time(NULL));

	//Set P and Q
	setPandQ(pbitLength, qbitLength);
	
	//Find g
	mpz_class g = 1;
	mpz_class div = (P - 1) / Q;
	mpz_class h;
	
	while (g == 1) 
	{
		h = rr.get_z_range (P-1)+1;
		mpz_powm(g.get_mpz_t(), h.get_mpz_t(), div.get_mpz_t(), P.get_mpz_t());
	}
	
	G = g;

	//Generate x
	mpz_class x;
	x = rr.get_z_range (Q-1) + 1;
	
	//Compute y -> Note: Adjusted to give y = g^x mod P
	mpz_class y;
	mpz_powm(y.get_mpz_t(), g.get_mpz_t(), x.get_mpz_t(), P.get_mpz_t());
	Y = y;
	
	return x;
}


void setPandQ(int pbitLength, int qbitLength) 
{
//Find P and Q
//
//EFFECTS: Generates P and Q of propper bit lengths

	string maxQ(qbitLength, '1');
	mpz_class maxq(maxQ, 2);
	
	string maxP(pbitLength, '1');
	mpz_class maxp(maxP, 2);
	
	while (true) 
	{
		string a = createNumber(qbitLength);
		mpz_class num(a, 2);
		
		mpz_class trialq;
		mpz_nextprime(trialq.get_mpz_t(), num.get_mpz_t());
		
		//Make sure next prime isn't bigger than max allowed value
		if (trialq > maxq) 
		{
			continue;
		}

		mpz_class trialp, one;
		one = "1";
		trialp = trialq << (pbitLength - qbitLength);
		trialp = trialp + one;
		
		//Test if q is prime
		while (trialp < maxp)
		{
			int probability = mpz_probab_prime_p(trialp.get_mpz_t(), 25);
		
			if (probability >= 1)
			{
				P = trialp;
				Q = trialq;
				return;
			} 
			
			trialp += trialq;
		}
	}
}

mpz_class hash_message(const std::string &message) 
{
//Hash message
//
//EFFECTS: takes as input a string "message", and returns an 
//         mpz_class of the hash of "message" using sha256
	
	unsigned int length = mpz_sizeinbase(Q.get_mpz_t(), 2);
	
	if (length > 256) 
	{
		length = 256;
	}
	
	length = length / 4;
	SHA256 sha256;
	std::string temp;
	
	char arr[length+1];
	const char *ptr = arr;

	// Hashing message
	sha256(message);
	temp = sha256.getHash();
	for (unsigned int i=0; i < length; i++) 
	{
		arr[i] = temp[i];
	}

	arr[length] = '\0';
	mpz_class z(ptr, 16);
	return z;
}

string createNumber(int n) 
{
//Create Number
//
//EFFECTS: returns a random "n" bit number 
			
	string myString;
	myString.push_back('1');
	
	for (int i = 1; i < n; ++i) 
	{
		int x = rand() % 2;
		char c = (char)(((int)'0')+x);
		myString.push_back(c);
	}
	return myString;
}


























