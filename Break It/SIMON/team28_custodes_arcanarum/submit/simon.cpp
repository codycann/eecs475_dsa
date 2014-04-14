#include "uberzahl.h"
#include <iostream>
#include <cstdlib>
#include <getopt.h>
#include <stdio.h>
#include <cassert>
#include <chrono>

using namespace std;

uberzahl generate_random();
void split_key();
void key_expansion();
uberzahl encrypt(uberzahl plaintext);
uberzahl decrypt(uberzahl ciphertext);
uberzahl encrypt_block(uberzahl block);
uberzahl decrypt_block(uberzahl block);
void printHelp();
bool checkInput(uberzahl input);
void testCorrectness();

//define constants
const int n = 64; //word size in bits
const int m = 4; //# of key words
const int T = 72; //# rounds
const int j = 4; //z number

//global keywords
uberzahl kwords[T - 1]; //# key words

/*
Creates a random key for the user to use for encryption
or decryption on a later run of the algorithm
*/
uberzahl generate_random(int bits)
{
	uberzahl k("0");
	for (int i = 0; i < bits; ++i)
	{
		k = k << 1;
		k = k + rand() % 2; 
	}

	return k;
}

/*
Forms first m keywords
*/
void split_key(uberzahl key)
{
	kwords[0] = key.extract(0,63);
	kwords[1] = key.extract(64, 127) >> 64;
	kwords[2] = key.extract(128,191) >> 128;
	kwords[3] = key.extract(192,255) >> 192;

	return;
}

/*
Forms keys m to T-1
*/
void key_expansion()
{
	int z[5][62] = 
		{
			{1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 1, 1, 0, 0,
			 0, 0, 1, 1, 1, 0, 0, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 1,
			 0, 0, 1, 0, 1, 0, 1, 1, 0, 0, 0, 0, 1, 1, 1, 0, 0, 1, 1, 0},
			{1, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 1, 1, 0,
			 0, 0, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1,
			 1, 1, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 1, 0, 1, 1, 0, 1, 0},
			{1, 0, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0,
			 1, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0,
			 0, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 1, 1, 0, 1, 1, 0, 0, 1, 1},
			{1, 1, 0, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0,
			 1, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0,
			 1, 0, 0, 1, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 0, 0, 1, 1, 1, 1},
			{1, 1, 0, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 0, 1, 0, 1, 1, 0,
			 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 0, 0, 0, 0,
			 1, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 1, 1, 1, 0, 1, 1, 1, 1},
		};//2d array so we can get individual digits
	uberzahl tmp;
	uberzahl c = 2;
	c = c.exp(n) - 4;

	//from page 11 for m = 4
	for(int i = 0; i < T-m; ++i)
	{
		uberzahl rot3 = kwords[i + 3].rotateRight(3, 0, n-1);
		uberzahl rot3xor = rot3 ^ kwords[i+1];
		uberzahl rot1 = rot3xor.rotateRight(1, 0, n-1);
		//k[i+m] = c XOR Z[j][i mod 62] XOR k[i] XOR (I XOR S-1)(S-3 k[i+3] XOR k[i+1])
		kwords[i + m] = c ^ z[j][i % 62] ^ kwords[i] ^ rot3xor ^ rot1;
	}

	return;
}

/*
Encrpyts all plaintext
*/
uberzahl encrypt(uberzahl plaintext)
{
	int current_bit = 0;
	uberzahl current_block("0");
	uberzahl ciphertext("0");
	while (current_bit < plaintext.bitLength())
	{
		//shift ciphertext over
		ciphertext = ciphertext << 128;
		//extract a block and encrypt
		current_block = plaintext.extract(current_bit, current_bit+127);
		//add block to the ciphertext
		ciphertext = ciphertext + encrypt_block(current_block);
		current_bit += 128;
	}

	return ciphertext;
}

/*
Decrypts all plaintext
*/
uberzahl decrypt(uberzahl ciphertext)
{
	int current_bit = 0;
	uberzahl current_block("0");
	uberzahl plaintext("0");
	while (current_bit < ciphertext.bitLength())
	{
		//shift plaintext over
		plaintext = plaintext << 128;
		//extract a block and decrypt
		current_block = ciphertext.extract(current_bit, current_bit+127);
		//add block to the plaintext
		plaintext = plaintext + decrypt_block(current_block);
		current_bit += 128;
	}

	return plaintext;
}

/*
Encrypts plaintext one block at a time
*/
uberzahl encrypt_block(uberzahl block)
{
	uberzahl x = block.extract(64,127) >> 64;
	uberzahl y = block.extract(0,63);

	//for i = 0..T-1
	for (int i = 0; i < T; ++i)
	{
		//tmp <- x
		uberzahl tmpword = x;
		//x <- y XOR (Sx & S^(8)x) XOR S^2x XOR k[i]
		x = y ^ (x.rotateLeft(1, 0, n-1) & x.rotateLeft(8, 0, n-1)) ^ x.rotateLeft(2, 0, n-1) ^ kwords[i];
		//y <- tmp
		y = tmpword;
	}

	//concatenate x and y
	uberzahl result = x;
	result = result << n;
	result = result + y;
	return result;

}

/*
Decrypts plaintext one block at a time
*/
uberzahl decrypt_block(uberzahl block){
	uberzahl x = block.extract(64,127) >> 64;
	uberzahl y = block.extract(0,63);

	for (int i = 0; i < T; ++i)
	{
		uberzahl tmpword = y;
		y = x ^ (y.rotateLeft(1, 0, n-1) & y.rotateLeft(8, 0, n-1)) ^ y.rotateLeft(2, 0, n-1) ^ kwords[T-i-1];
		x = tmpword;
	}

	//concatenate x and y
	uberzahl result = x;
	result = result << n;
	result = result + y;
	return result;
}

/*
Tests that when a plaintext is encrpyted it is decrypted 
to the original plaintext correctly. Also tests that if
a cipher text is decrypted renencrypting it produces the
same ciphertext. This is assuming the same key for
encryption and decryption.
*/
void testCorrectness()
{
	auto start = std::chrono::high_resolution_clock::now();
	auto duration = chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now()-start).count();
	cout << "Key, Key Length, Plaintext, Plaintext Length, Ciphertext, Ciphertext Length, Time, Correct" << endl;
	cout << "-------------------------------------------------------------------------------" << endl;

	for(int i = 32; i < 2048; ++i)
	{
		start = std::chrono::high_resolution_clock::now();

		uberzahl key = generate_random(i/8);
		split_key(key);
		key_expansion();
		
		uberzahl plaintext1 = generate_random(i/16);
		uberzahl encrypted1 = encrypt(plaintext1);
		uberzahl decrypted1 = decrypt(encrypted1);

		uberzahl ciphertext2 = generate_random(i/16);
		uberzahl decrypted2 = decrypt(ciphertext2);
		uberzahl encrypted2 = encrypt(decrypted2);

		duration = chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now()-start).count();

		cout << key << "," << key.bitLength() << "," << plaintext1 << "," << plaintext1.bitLength() << "," << ciphertext2 << "," << ciphertext2.bitLength() << "," << duration << ",";
		//if incorrect
		if(plaintext1 != decrypted1 || ciphertext2 != encrypted2)
			cout << "False" << endl;
		else
			cout << "True" << endl;
	}
}

void printHelp()
{
	cout << 	"Usage: ./simon <args>\n" <<
				"All arguments must be given in base 10\n\n" <<
				"Arguments list\n" <<
				"-h\tprints this output\n" <<
				"\tUsage: ./simon -h\n\n" <<
				"-g\tgenerates a key\n" <<
				"\tUsage: ./simon -g\n\n" <<
				"-k\tspecifies the key to use\n" <<
				"\tUsage: ./simon -[p|c] <message> -k <key>\n\n" <<
				"-p\tencrypts specified plaintext\n" <<
				"\tUsage: ./simon -[p] <message> -k <key>\n\n" <<
				"-c\tdecrypts specified ciphertext\n" <<
				"\tUsage: ./simon -[c] <message> -k <key>\n\n" << 
				"-t\ttimes and tests random inputs\n" << 
				"\tUsage: ./simon -t" << endl;
}

bool checkInput(uberzahl input)
{
	if (input == uberzahl("0"))
		return false;
	else
		return true;
}

int main (int argc, char* argv[])
{
	uberzahl key("0");
	bool encryption = true;
	bool keygen = false;
	string filename = "";;

	uberzahl plaintext("0");
	uberzahl ciphertext("0");

	int c;

	srand(time(NULL));

	//get command line parameters
	while ((c = getopt(argc, argv, "k:p:c:ght")) != -1)
	{
		switch(c)
	    {
	        case 'k':
	            key = uberzahl(optarg);
	            break;
		    case 'p':
		    	plaintext = uberzahl(optarg);
		    	encryption = true;
		    	break;
		    case 'g':
		    	keygen = true;
		    	break;
		    case 'c':
		    	ciphertext = uberzahl(optarg);
		    	encryption = false;
		    	break;
		    case 'h':
		   		printHelp();
		   		return 0;
		   	case 't':
		   		testCorrectness();
		   		return 0;
		   	default:
		   		printHelp();
		   		return 0;


	    }
	}

	if (keygen)
	{
		key = generate_random(256);
		//cout << "key bitlength: " << key.bitLength() << endl;
		cout << "key: " << key << endl;
		return 0;
	}

	split_key(key);
	key_expansion();
	
	if (encryption)
	{
		if (!checkInput(plaintext))
		{
			cerr << "No plaintext given" << endl;
			return 1;
		}
		if (!checkInput(key))
		{
			cerr << "No key given" << endl;
			return 1;
		}
		ciphertext = encrypt(plaintext);
		cout << "ciphertext: " << ciphertext << endl;
	}
	else
	{
		if (!checkInput(ciphertext))
		{
			cerr << "No ciphertext given" << endl;
			return 1;
		}
		if (!checkInput(key))
		{
			cerr << "No key given" << endl;
			return 1;
		}
		plaintext = decrypt(ciphertext);
		cout << "plaintext: " << plaintext << endl;
	}

	return 0;
}
