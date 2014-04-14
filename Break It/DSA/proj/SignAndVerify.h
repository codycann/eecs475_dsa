#ifndef __SaV__
#define __SaV__


#include <string>
#include "uberzahl.h"


using std::string;

struct pubkey
{
	uberzahl p, q, g, y;
	pubkey (uberzahl pin, uberzahl qin, uberzahl gin, uberzahl yin);
	void print();

};


struct sig
{
	uberzahl r, s;
	sig(uberzahl rin, uberzahl sin);
	void print();

};

bool verify(sig in, pubkey key, string m);
sig sign(string m, uberzahl x, pubkey key);

 
#endif /* __SaV__ */
