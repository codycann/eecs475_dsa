#ifndef _GCTR_H_
#define _GCTR_H_

#include "uberzahl.h"
#include <string.h>

uberzahl ciph_k(uberzahl u_key, uberzahl u_text);
uberzahl gctr(uberzahl key, uberzahl x, uberzahl ICB, int bit_length_diff);
std::string uber2hex(uberzahl d);
uberzahl betterExtract(uberzahl d, int start, int end);


#endif //_GCTR_H_