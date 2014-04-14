#include "rsa.h"
uberzahl __Mmodulus;
uberzahl __Mprime;
uberzahl __Rbar;
uberzahl __Rmont;
uberzahl ONE = 1;

uberzahl rsa_encrypt ( const uberzahl& message, const uberzahl& e, const uberzahl& n ){
  return montgomery_expm( message, e, n );
}

uberzahl rsa_decrypt ( const uberzahl& cipher, const uberzahl& d, const std::vector<uberzahl>& primes ){
  uberzahl retval;
  uberzahl N = 1;
  std::vector<uberzahl> crt;

  for ( size_t i=0; i < primes.size(); ++i )
    N = N * primes[i];
  // work montgomery modulus prime
  for ( size_t i=0; i < primes.size(); ++i )
    crt.push_back( montgomery_expm( cipher, d, primes[i] ) );
  // work montgomery modulus N
  for ( size_t i=0; i < primes.size(); ++i )
    crt[i] = crt_piece( crt[i], primes[i], N / primes[i] );
  for ( size_t i=0; i < crt.size(); ++i )
    retval = montgomery_add( retval, crt[i], N );

  return retval;
}

void rsa_key_gen( unsigned int bitLength ){
  std::vector<uberzahl> primes;
  uberzahl prime;
  uberzahl N = 1;
  uberzahl phi = 1;

  do {
    prime.random( bitLength / PRIME_COUNT );
    prime = nextprime( prime, 100 );

    bool newPrime = true;
    for ( size_t i=0; i<primes.size(); ++i )
      if ( prime == primes[i] ){
        newPrime = false;
        break;
      }

    if ( newPrime ){
      primes.push_back( prime );
      N = N*prime;
      phi = phi*(prime-1);
    }
  }
  while ( primes.size() < PRIME_COUNT );

  uberzahl e;
  do{
    e.random( bitLength + 1 );
    e.clearBit( bitLength );
  }
  while ( e < 2 || ( e == (ONE << bitLength) - 1 ) || e.gcd(phi) > 1 );

  std::cout << "Public Information : " << std::endl;
  std::cout << "e = " << e << std::endl;
  std::cout << "N = " << N << std::endl << std::endl;

  std::cout << "Private Information : " << std::endl;
  std::cout << "d = " << e.inverse(phi) << std::endl;
  std::cout << "Primes : " << std::endl;
  for ( size_t i=0; i<primes.size(); ++i )
    std::cout << primes[i] << std::endl;

}

uberzahl RED ( const uberzahl& T ){
  uberzahl m = ( __Mprime * T ) & ( __Rmont - 1 );
  uberzahl retval = ( T + m*__Mmodulus ) >> ( __Rmont.bitLength() - 1 );
  return ( retval >= __Mmodulus ) ? retval - __Mmodulus : retval;
}

uberzahl montgomery_add ( const uberzahl& a, const uberzahl& b, const uberzahl& modulus ){
  if ( a > modulus )
    return montgomery_add( a%modulus, b, modulus );
  if ( b > modulus )
    return montgomery_add( a, b%modulus, modulus );
  
  montgomeryModulus( modulus );
  uberzahl Abar = RED(a * __Rbar);
  uberzahl Bbar = RED(b * __Rbar);

  return RED( Abar + Bbar );
}

uberzahl montgomery_mult ( const uberzahl& a, const uberzahl& b, const uberzahl& modulus ){
  if ( a > b ) // allow for arbitrary multiplication
    return montgomery_mult( b, a, modulus );
  if ( a > modulus ) // easy checks are a > modulus, b > R
    return montgomery_mult( a%modulus, b, modulus );
  montgomeryModulus( modulus ); // reassign the montgomery modulus
  if ( b >= __Rmont ) // now ab < MR
    return montgomery_mult( a, b%modulus, modulus );

  uberzahl Abar = RED ( a * __Rbar );
  return RED( Abar * b );
}

uberzahl montgomery_expm ( const uberzahl& base, const uberzahl& exponent, const uberzahl& modulus ){
  uberzahl retval = base;
  for ( size_t i = exponent.bitLength()-1; i > 0; --i ){
    retval = montgomery_mult( retval, retval, modulus );
    if ( exponent.bit(i-1) )
      retval = montgomery_mult( base, retval, modulus );
  }
  return retval;
}

uberzahl crt_piece ( const uberzahl& a, const uberzahl& m, const uberzahl& M ){
  // return a*M*M' where M' is M^(-1) mod m
  uberzahl retval = montgomery_mult(a, M, M*m);
  retval = montgomery_mult(retval, M.inverse(m), M*m);
  return retval;
}

void montgomeryModulus( const uberzahl& modulus ){
  if ( __Mmodulus != modulus ){ // we need to recalculate R_bar
    __Mmodulus = modulus;
    __Rmont = ONE << __Mmodulus.bitLength();
    __Mprime = __Rmont - modulus.inverse( __Rmont );
    __Rbar = ( __Rmont * __Rmont ) % modulus;
  }
}
