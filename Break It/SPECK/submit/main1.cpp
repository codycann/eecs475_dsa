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
	if(argc != 4)
		exit(1);
	mode = argv[1];
	if(mode != "encrypt" && mode != "decrypt")
		exit(1);
	key = argv[3];
	if(mode == "encrypt")
	{
		uberzahl plaintext = argv[2];
		std::cout<<"initial plaintext: "<<plaintext<<"\n";
		std::cout<<"key: "<<key<<"\n";
		std::cout<<"ciphertext: "<<obj.encryption(plaintext, key)<<"\n";
	}
	if(mode == "decrypt")
	{
		uberzahl ciphertext = argv[2];
		std::cout<<"initial ciphertext: "<<ciphertext<<"\n";
		std::cout<<"key: "<<key<<"\n";
		std::cout<<"plaintext: "<<obj.decryption(ciphertext, key)<<"\n";
	}
	std::cout<<"<END OF OUTPUT>"<<"\n";
	return 0;
}