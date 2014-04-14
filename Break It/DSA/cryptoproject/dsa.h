#ifndef DSA_H
#define DSA_H

#include <vector>
#include "uberzahl.h"

#define L 1024
#define N 160

class dsa
{
	public:
		
		
		//Generates public key (p, q, g, y) and private key (x)
		//Returns a vector of the keys in specified order <p,q,g,y,x>
		vector<uberzahl> keygen();
		
		//Signs a message with two values (r, s)
		vector<uberzahl> sign(vector<uberzahl> key, uberzahl message);
		
		//Verifies (r, s)
		vector<bool> verify(vector<uberzahl> sign, uberzahl message);
		
		
	private:
		uberzahl p;
		uberzahl q;
		uberzahl g;
		uberzahl y;
		uberzahl x;
		vector<uberzahl> k;
		vector<uberzahl> kInv;

}

#endif