#include <vector>
#include "uberzahl/uberzahl.h"
#include "present.h"
using namespace std;

//length should be either 80 or 128 in bits
uberzahl keygen(int length);

//each plaintext should be 64bits long
vector<uberzahl> encrypt(vector<uberzahl> key,vector<uberzahl> plaintext);

//each ciphertext should be 64bits long
vector<uberzahl> decrypt(vector<uberzahl> key,vector<uberzahl> ciphertext);
