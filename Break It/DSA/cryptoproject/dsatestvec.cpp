/*
	This tests the sign and verify functions using the first example from:
		http://csrc.nist.gov/groups/ST/toolkit/documents/Examples/DSA2_All.pdf
	k and z are hardcoded in since our randomness and hash differ
	
	input when testing sign is in "signinput.txt"
	correct output is in "correctsign.txt"

	input when testing verify is in "verifyinput.txt"
	correct output is in "correctverify.txt"
*/


#include "uberzahl.h"
#include <openssl/sha.h>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <time.h>
#include <iomanip>
#include <sstream>
#include <string>

using namespace std;
/*
	Using libgmp now. Be sure to compile with -lgmpxx and -lgmp
	For openssl compile with -lssl
*/

#define P_BITS 1024
#define Q_BITS 160

struct parameters
{
	uberzahl p;
	uberzahl q;
	uberzahl g;
	uberzahl y;
	uberzahl x;
};

struct signature
{
	uberzahl r;
	uberzahl s;
};

uberzahl modexp(uberzahl, uberzahl, uberzahl);
uberzahl findG(uberzahl &, uberzahl &);
void init(parameters &);
parameters keygen();
signature sign(parameters &, uberzahl &);
bool verify(parameters &, signature &, uberzahl);
string sha256(const string);

int main(int argc, char **argv)
{
	if (argc == 1)
	{
		cerr << argv[0] << " [mode]" << endl;
		return 1;
	}
	string mode = argv[1];
	
	//outputs public key (p, q, g, y) followed by private key x
	if (mode == "keygen")
	{
		parameters key = keygen();
		cout << key.p << endl;
		cout << key.q << endl; 
		cout << key.g << endl; 
		cout << key.y << endl;
		cout << key.x << endl;
	}
	//input: p, q, g, y, x, message
	//output: signature (r, s)
	else if (mode == "sign")
	{
		parameters key;
		string pIn, qIn, gIn, yIn, xIn, mIn;
		cin >> pIn;
		cin >> qIn;
		cin >> gIn;
		cin >> yIn;
		cin >> xIn;
		key.p = pIn.c_str();
		key.q = qIn.c_str();
		key.g = gIn.c_str();
		key.y = yIn.c_str(); //doesn't use y
		key.x = xIn.c_str();
		
		cin >> mIn;
		uberzahl m = mIn.c_str();
		signature sig = sign(key, m);
		
		cout << sig.r << endl;
		cout << sig.s << endl;
	}
	//input: p, q, g, y, x, message, r, s
	//output: true or false if valid signature
	else if (mode == "verify")
	{
		parameters key;
		string pIn, qIn, gIn, yIn, xIn, rIn, sIn, mIn;
		cin >> pIn;
		cin >> qIn;
		cin >> gIn;
		cin >> yIn;
		cin >> xIn;
		key.p = pIn.c_str();
		key.q = qIn.c_str();
		key.g = gIn.c_str();
		key.y = yIn.c_str();
		key.x = xIn.c_str(); //doesn't use x
		
		cin >> mIn;
		uberzahl m = mIn.c_str();
		
		signature sig;
		cin >> rIn;
		cin >> sIn;
		sig.r = rIn.c_str();
		sig.s = sIn.c_str();		
		
		bool verified = verify(key, sig, m);
		cout << verified << endl;	
	}
	else
	{
		cerr << "Invalid mode; use keygen, sign, or verify" << endl;
		return 1;	
	}
	
	return 0;
}

//Computes x^c mod n (from previous homework)
uberzahl modexp(uberzahl x, uberzahl c, uberzahl n)
{
	uberzahl z(1);	
	int l = c.bitLength();	
	
	for (int i = l-1; i >= 0; --i)
	{
		z = (z * z) % n;
		if (c.bit(i) == 1)
		{
			z = (z * x) % n;
		}
	}
	
	return z;
}

//Input: the generated primes p, q
//Output: generator g
uberzahl findG(uberzahl &p, uberzahl &q)
{
	uberzahl one = 1;
	uberzahl two = 2;
	uberzahl pMinus = p - 1;
	uberzahl e = (p-1)/q;
	uberzahl h, g;

	do 
	{
		h = random(one, pMinus);
		g = modexp(h, e, p);
	}
	while (g < two || g > pMinus || modexp(g, q, p) != one);
	return g;
}

//initialize primes p and q, generator g
void init(parameters &param)
{
	srand(time(0));

	mpz_class mpz_p;
	mpz_class mpz_q;

	gmp_randclass gmp_rand (gmp_randinit_default);
	
	gmp_rand.seed(time(0));

	start:
	do
	{
		mpz_q = gmp_rand.get_z_bits(Q_BITS);	
	}
	while (!(mpz_probab_prime_p(mpz_q.get_mpz_t(), 50)));

	mpz_p = mpz_q;
	
	do
	{
		mpz_p = mpz_p * 2;
	}
	while (mpz_p.get_str(2).length() < P_BITS);

	mpz_p = mpz_p + 1;
	
	if (!(mpz_probab_prime_p(mpz_p.get_mpz_t(), 75))) goto start;
	
	//convert mpz_class to uberzahl
	string p_str = mpz_p.get_str(10);
	string q_str = mpz_q.get_str(10);
	uberzahl p(p_str.c_str());
	uberzahl q(q_str.c_str());

	param.p = p;
	param.q = q;
	param.g = findG(p, q); //find generator
	return;
}

//Generates public key (p, q, g, y) and private key (x)
parameters keygen()
{
	parameters key;	
	init(key);
	
	//find keys x, y
	key.x = random("1", key.q - 1);
	key.y = modexp(key.g, key.x, key.p);
	
	return key;
}

//Signs a message with two values (r, s)
signature sign(parameters &key, uberzahl &plaintext)
{
	signature sig;
	srand(time(0));
	
	do
	{
		//generate secret k and k inverse
		unsigned long long security_strength = 512; //for 3072 = L
		uberzahl c = "1";
		c.random(security_strength);
		c = c.extract(0, (Q_BITS +64));
		uberzahl k = (c % (key.q - 1)) + 1;
		//hardcoded in because randomness
		k = "300353875094850383992357513808803338939680923765";
		uberzahl k_inv = k.inverse(key.q);
		
		//hash message
		string text = plaintext.convert_to_string();
		string hashTxt = sha256(text);
		uberzahl z = hashTxt.c_str();
		z = z.extract(0, Q_BITS);
		//hardcoded in due to different hash functions
		z = "968236873715988614170569073515315707566766479517";
		
		//calculate signature
		sig.r = (modexp(key.g, k, key.p)) % key.q; 	
		sig.s = (k_inv*(z + key.x * sig.r)) % key.q;
	}
	while (sig.r == uberzahl("0") || sig.s == uberzahl("0"));
	
	return sig;
}	

//Verifies (r, s)
bool verify(parameters &key, signature &sign, uberzahl message)
{
	uberzahl s = sign.s;
	uberzahl w = s.inverse(key.q);
	cout << "w: " << w << "\n";
	
	string text = message.convert_to_string();
	string hashTxt = sha256(text);
	uberzahl z = hashTxt.c_str(); 
	z = z.extract(0, Q_BITS);
	//hardcoded in due to different hash functions
	z = "968236873715988614170569073515315707566766479517";
	
	uberzahl u1 = (z * w) % key.q;
	cout << "u1: " << u1 << "\n";
	uberzahl u2 = (sign.r * w) % key.q;
	cout << "u2: " << u2 << "\n";
	uberzahl v = ((modexp(key.g, u1, key.p) * modexp(key.y, u2, key.p)) % key.p) % key.q;
	cout << "v: " << v << "\n";
	
	if (v == sign.r) return true; //valid signature
	return false;
}

//Source: http://stackoverflow.com/a/10632725
string sha256(const string str)
{
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, str.c_str(), str.size());
    SHA256_Final(hash, &sha256);
    stringstream ss;
    for(int i = 0; i < SHA256_DIGEST_LENGTH; i++)
    {
        ss << dec << setw(2) << setfill('0') << (int)hash[i];
    }
    return ss.str();
}