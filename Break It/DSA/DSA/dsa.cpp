#include<string>
#include<time.h>
#include<iostream>
#include<sstream>
#include<string>
#include<stdlib.h>
#include<cmath>
#include<string>
#include<vector>
#include"uberzahl.h"
#include"uberzahl.cpp"
#include"sha256.h"
#include"sha256.cpp"
//#include"dsa.h"

using namespace std;

// Original Modular Exponentiation function from HW 4
uberzahl modExpOrig(uberzahl x, uberzahl c, uberzahl n)
{
uberzahl z = 1;
unsigned int l = c.bitLength();
uberzahl remainder;

for(int i = l-1; i >= 0; i = i - 1)
{
z = (z*z)%n;
if((c >> i & 0x1) == 1)
{
z = (x*z)%n;
}
}

return z;
}

// Function converts a string to a Uberzahl
uberzahl convertstrings(string x)
{
	uberzahl h(x.c_str());
	return h;
}

//used to get p and q in parameter gen
// Gets a random Uberzahl with given bitlength
uberzahl getRandPrime(long long bitlength)
{
	long long two = 2;
	uberzahl uberBitLower(bitlength-1);
	uberzahl uberBitHigher(bitlength);
	uberzahl a(two);
	uberzahl b(two);
	a = a.exp(uberBitLower);
	b = b.exp(uberBitHigher);
	uberzahl c = random(a,b);
	unsigned long long prob = 50;
	uberzahl d = nextprime(c,prob);
	while(d >= b)
	{
		uberzahl c = random(a,b);
		uberzahl d = nextprime(c,prob);
	}
	return d;
}

//used to get x and k in parameter gen
// Generates a random Uberzahl from 1 to q-1 inclusive
uberzahl genSecretNumber(uberzahl q)
{
	long long zero = 0;
	uberzahl uberZero(zero);
	uberzahl randomNum = random(zero,q);
	while(randomNum == uberZero || randomNum == q)
	{
		randomNum = random(zero,q);
	}

	return randomNum;
}

// Generate a Public Key with using y = g^x mod(p)
uberzahl genPublicKey(uberzahl g, uberzahl x, uberzahl p)
{
	return g.expm(x,p);
}

// Supplies Validation or Generator q
bool partialValidation(uberzahl p, uberzahl q, uberzahl g)
{
	long long two = 2;
	uberzahl ubertwo(two);
	long long one = 1;
	uberzahl uberone(one);
	if(g < ubertwo || g > p-uberone)
	{
		return false;
	}
	if(g.expm(q,p) == uberone)
	{
		return true;
	}
	return false;
}

//generates g
// Supplies an Uberzahl Generator
uberzahl unverifGenGenerator(uberzahl p, uberzahl q)
{
	long long longOne = 1;
	uberzahl one(longOne);
	uberzahl e = (p-one)/q;
	uberzahl g;
	for(uberzahl h = one+one; h < (p-one); h= h+one)
	{
		g = h.expm(e,p);
		if(g != one)
		{
			if(partialValidation(p,q,g))
			{
				return g;
			}
		}
	}
	cout << "No valid generator found" << endl;
	exit(1);
}

//Specifically generates q such that (p-1)%q == 0
uberzahl genQ(uberzahl p, long long N, long long L)
{
	long long one = 1;
	uberzahl uberOne(one);
	uberzahl q = getRandPrime(N);
	long long zero = 0;
	uberzahl uberZero(zero);
	long long counter = 0;
	while((p-uberOne)%q != uberZero && counter <= L)
	{
		q = getRandPrime(N);
		counter++;
	}
	if(counter == L+1)
	{
		cout << "Probably no q to match p" << endl;
		exit(1);
	}

	return q;
}

// Computes an appropriate Z given the output length, hash and N
string computeZ(long long N, long long outlen, string hash)
{
	//get every bit in the bitlengh of min(n,outlen) to be 1
	long long tempBits = 1;
	if(N < outlen)
	{
		while(N != 0)
		{
			tempBits = tempBits << 1;
			N = N >> 1;
		}

	}
	else
	{
		while(outlen != 0)
		{
			tempBits = tempBits << 1;
			outlen = outlen >> 1;
		}
	}
	//tempBits = tempBits << 1;	//this may or may not be necessary but I can't count
	tempBits -= 1;

	uberzahl uberBits(tempBits);
	uberzahl hashhash(hash.c_str());
	return (uberBits & hashhash).convert_to_string();
}

// Calculate r, s, z and returning r and s while recalculating k if needed.
uberzahl signGen(uberzahl & r, uberzahl g, uberzahl & k, uberzahl p, uberzahl q,
		uberzahl x, long long N, long long outlen, string hash, string & z)
{
	r = modExpOrig(g,k,p) % q;

	string hashhash = sha256(hash);
	z = computeZ(N, outlen, hashhash);
	uberzahl zstar(z.c_str());
	uberzahl s = (q.inverse(k)*(zstar+x*r) ) % q;	//note that this doesn't use uberzahl at all

	long long zero = 0;
	uberzahl uberzero(zero);

	if(r == uberzero || s == uberzero)
	{
		//randomizeK
		while(r == uberzero || s == uberzero)
		{
			//find new k
			k = genSecretNumber(q);
			r = modExpOrig(g,k,p) % q;
			z = computeZ(N, outlen, hashhash);
			s = (q.inverse(k)*(zstar+x*r)) % q;	//note that this doesn't use uberzahl at all
		}

	}

	return s;
}

// Written in spec. Returns true if v = rprime and false otherwise (given certain conditions);
bool sigVerif(string Mp, uberzahl rp, uberzahl sp, uberzahl p, uberzahl q,
		uberzahl g, uberzahl x, uberzahl y, long long N, long long outlen, string & zp)
{
	long long zero = 0;
	uberzahl uberzero(zero);
	//step 1
	if(rp <= uberzero || rp >= q || sp <= uberzero || sp >= q)
	{
		return false;
	}

	//step2
	uberzahl w = q.inverse(sp);//invert(sp,q);
	uberzahl zpstar(zp.c_str());
	//HASH Mprime hash(Mp)
	string hashhash = sha256(Mp);

	zp = computeZ(N, outlen, hashhash);//should be hashMp and converted to integer
	uberzahl u1 = (zpstar*w)%q;
	uberzahl u2 = (rp*w)%q;
	uberzahl gu1 = modExpOrig(g,u1,p);
	uberzahl yu2 = modExpOrig(y,u2,p);
	uberzahl v = (gu1*yu2)%q;
	if(v == rp)
	{
		return true;
	}
	return false;
}

// Verifies that L and N are valid bitlength pairs
bool verifyLN(long long L, long long N)
{
	if(L == 1024 && N == 160)
	{
		return true;
	}
	if(L == 2048 && N == 224)
	{
		return true;
	}
	if(L == 2048 && N == 256)
	{
		return true;
	}
	if(L == 3072 && N == 256)
	{
		return true;
	}
	cout << "Invalid key pair length" << endl;
	exit(1);
}

