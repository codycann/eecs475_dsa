#ifndef _ENCRYPT_H_
#define _ENCRYPT_H_

#include "uberzahl.h"

struct CiphText_AuthTag
{
	uberzahl ciphertext;
	uberzahl authtag;
};

CiphText_AuthTag encrypt_decrypt(uberzahl key, uberzahl IV, uberzahl A, uberzahl text);

#endif //_ENCRYPT_H_