#ifndef __AUX__H__
#define __AUX__H__
#include "uberzahl/uberzahl.h"

const int shiftNum = 64;

uberzahl shiftLeft64(uberzahl value, int shift, int numbits = 64);
 
uberzahl shiftRight64(uberzahl value, int shift, int numbits = 64);


#endif
