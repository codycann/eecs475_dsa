#include "speck.h"
#include <iostream>
#include <cstdlib>
#include <string>
#include <ctime>
int main(int argc, char* argv[])
{
	Speck_128_256 obj;
	uberzahl key, plaintext;
	if(argc == 3)
	{
		key = argv[2];
		plaintext = argv[1];
	}
	else
	{
		srand(time(NULL));
		key = uberzahl().random(256);
		plaintext = uberzahl().random(128);
	}
	std::cout<<"initial plaintext: "<<plaintext<<"\n";
	std::cout<<"key: "<<key<<"\n";
	uberzahl ciphertext = obj.encryption(plaintext, key);
	std::cout<<"ciphertext: "<<ciphertext<<"\n";
	std::cout<<"-----------------------\n";
	ciphertext = obj.decryption(ciphertext, key);
	std::cout<<"decrypted text: "<<ciphertext<<"\n";
	std::cout<<"is plaintext same after encryption and decryption?: "<<(plaintext == ciphertext)<<"\n";
	std::cout<<"<END OF OUTPUT>"<<"\n";
	return 0;
}