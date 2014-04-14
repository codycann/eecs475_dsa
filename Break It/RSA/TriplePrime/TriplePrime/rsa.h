#ifndef RSA_H
#define RSA_H

#include "uberzahl.h"
#include <vector>

#define PRIME_COUNT 3 //triple prime RSA, can be changed to any value greater than 1

uberzahl rsa_encrypt ( const uberzahl&, const uberzahl&, const uberzahl& );
uberzahl rsa_decrypt ( const uberzahl&, const uberzahl&, const std::vector<uberzahl>& );
void rsa_key_gen ( unsigned int );

uberzahl RED ( const uberzahl& ); // this function uses global variables to cache
uberzahl montgomery_add ( const uberzahl&, const uberzahl&, const uberzahl& );
uberzahl montgomery_mult ( const uberzahl&, const uberzahl&, const uberzahl& );
uberzahl montgomery_expm ( const uberzahl&, const uberzahl&,  const uberzahl& );

uberzahl crt_piece ( const uberzahl&, const uberzahl&, const uberzahl& );
void montgomeryModulus ( const uberzahl& );

#endif
