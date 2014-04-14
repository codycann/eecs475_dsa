#ifndef __SHA1_H__
#define __SHA1_H__

#include "uberzahl.h"
#include <string>

using std::string;

unsigned int rotate(unsigned int rot, int index);

uberzahl SHA1(string message);

#endif /* __SHA1_H__ */
