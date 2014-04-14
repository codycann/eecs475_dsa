#ifndef __DSA__
#define __DSA__

using namespace std;


//Sets public keys and returns private key "x"
mpz_class keyGeneration(int pbitLength = 2048, int qbitLength = 256);

//Creates the signature
pair<mpz_class, mpz_class> encrypt(mpz_class key, mpz_class plaintext);

//Returns true if signature is verified, false otherwise
bool verify(pair<mpz_class, mpz_class> signature, mpz_class plaintext);

//Sets P and Q
void setPandQ(int pbitLength, int qbitLength);

//Returns a string with bitlength "n"
string createNumber(int n);

//Returns the hash of "message"
mpz_class hash_message(const std::string &message);


#endif
