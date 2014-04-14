#include <math.h>
#include "uberzahl.h"
#include <stdlib.h> 
#include "SHA1.h"
#include "SignAndVerify.h"

using namespace std;




	pubkey::pubkey (uberzahl pin, uberzahl qin, uberzahl gin, uberzahl yin)
	{
		p = pin;
		q = qin;
		g = gin;
		y = yin;
	}

	 void pubkey::print()
	{
		cout<<"p is "<<p<<endl;
		cout<<"q is "<<q<<endl;
		cout<<"g is "<<g<<endl;
		cout<<"y is "<<y<<endl;
	}


	sig::sig(uberzahl rin, uberzahl sin)
	{
		r = rin; s = sin;
	}

	void sig::print()
	{
		cout<<"r is "<<r<<endl;
		cout<<"s is "<<s<<endl;
	}


sig sign(string m, uberzahl x, pubkey key)
{
	uberzahl zero("0");
	uberzahl s = zero; uberzahl r = zero;
	bool done = false;
	while (!done)
	{
		uberzahl k = (rand());
		k = ( k % (key.q - 1)) + 1;
		r = (key.g.expm(k, key.p)) % key.q;
		s = k.inverse(key.q)*(SHA1(m) + x*r);
		s = s % key.q;
		if(r != zero && s != zero)
			done = true;
	}
	return sig(r, s);
}

bool verify(sig in, pubkey key, string m)
{
	uberzahl zero("0");
	if(in.r >= key.q || in.r <= zero)
		return false;
	if(in.s >= key.q || in.s <= zero)
		return false;
	uberzahl w = in.s.inverse( key.q);
	uberzahl u1 = (SHA1(m) * w) % key.q;
	uberzahl u2 = (in.r * w) % key.q;
	uberzahl v = ((key.g.expm(u1,key.p)* key.y.expm(u2,key.p))%key.p) % key.q;
	return (v == in.r); 
}




