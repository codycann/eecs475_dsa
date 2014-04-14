#include <iostream>
#include "uberzahl.h"
#include <stdlib.h>     /* srand, rand */
#include <time.h>
using namespace std;

#define NUMKEYWORDS 2 	// m
#define NUMROUNDS 32 		// T
#define WORDSIZE 64 		// n
#define BLOCKSIZE 128
#define KEYSIZE 128
#define ALPHA 8
#define BETA 3

uberzahl trimmedNum(uberzahl num, int bits);

class Speck {
  public:
    Speck();
    uberzahl genKey();
    void setKey(uberzahl userKey);
    uberzahl encrypt(uberzahl plaintext);
    uberzahl decrypt(uberzahl ciphertext);
    uberzahl encrypt(uberzahl key, uberzahl plaintext);
    uberzahl decrypt(uberzahl key, uberzahl ciphertext);
  private:
    void expand(uberzahl &x, uberzahl &y, uberzahl k);
    void contract(uberzahl &x, uberzahl &y, uberzahl k);
    uberzahl key;
    uberzahl rounds[NUMROUNDS];
};

