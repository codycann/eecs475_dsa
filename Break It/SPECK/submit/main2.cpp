#include "speck.h"
#include <iostream>
#include <cstdlib>
#include <string>
#include <ctime>
int main(int argc, char* argv[])
{
	Speck_128_256 obj;
	uberzahl key;
	std::string mode;
	srand(time(NULL));
	if(argc != 4)
	{
		exit(1);
	}
	mode = argv[1];
	if(mode != "encrypt" && mode != "decrypt")
	{
		exit(1);
	}
	if(mode == "encrypt")
	{
		int keysize = atoi(argv[3]);
		int textsize = atoi(argv[2]);
		uberzahl key = uberzahl().random(keysize);
		uberzahl plaintext = uberzahl().random(textsize);
		std::cout<<"initial plaintext: "<<plaintext<<"\n";
		std::cout<<"key: "<<key<<"\n";
		std::cout<<"key size: "<<keysize<<"\n";
		std::cout<<"plaintext size: "<<textsize<<"\n";
		std::cout<<"ciphertext: "<<obj.encryption(plaintext,key)<<"\n";
	}
	else
	{
		int keysize = atoi(argv[3]);
		int textsize = atoi(argv[2]);
		uberzahl key = uberzahl().random(keysize);
		uberzahl ciphertext = uberzahl().random(textsize);
		std::cout<<"initial ciphertext: "<<ciphertext<<"\n";
		std::cout<<"key: "<<key<<"\n";
		std::cout<<"key size: "<<keysize<<"\n";
		std::cout<<"ciphertext size: "<<textsize<<"\n";
		std::cout<<"plaintext: "<<obj.decryption(ciphertext,key)<<"\n";
	}
	std::cout<<"<END OF OUTPUT>"<<"\n";
	return 0;
}