#ifndef SPECK_H
#define SPECK_H

#include <array>
#include "uberzahl.h"

uberzahl keygen();
//generates a random key

uberzahl encrypt(uberzahl key, uberzahl plaintext);
//encrypts "plaintext" using "key"

uberzahl decrypt(uberzahl key, uberzahl ciphertext);
//decrypts "ciphertext" using "key"

#endif
