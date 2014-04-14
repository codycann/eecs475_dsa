//Speck encryption algorithm with 128 bit key and plaintext
//EECS 475 group 15 - Anonymous

#include <iostream>
#include <iomanip>
#include <sstream>
#include <stdio.h>
// #include <chrono> //for testing time
#include <stdint.h>

#include <x86intrin.h>
#include <openssl/rand.h>

#include "speck_openssl.h"

using namespace std;

//Speck 128/256
#define CHOSENKEYWORDSIZE 4 
#define CORRESPONDINGROUNDS 34 
#define WORDSIZE 64


void SpeckClass::expandKey(){
	//create table of keys with the initial key
	if(tableFilled) return;
	
	for(uint64_t i=0; i<T-1; i++){
		int index = i+m-1;
		l[index] = (RCS(l[i], alpha) + key[i])  ^ i;
		key[i+1] = LCS(key[i], beta) ^ l[index];
	}
	tableFilled = 1;
}

void SpeckClass::encrypt(uint64_t round){
	for(uint64_t i=0; i<x.size(); i++){
		x[i] = (RCS(x[i], alpha) + y[i]) ^ key[round];
		y[i] = LCS(y[i], beta) ^ x[i];
	}
}

void SpeckClass::decrypt(uint64_t round){
	for(uint64_t i=0; i<x.size(); i++){
		y[i] = RCS((x[i] ^ y[i]), beta);
		x[i] = LCS((x[i] ^ key[round]) - y[i], alpha);
	}
}

void SpeckClass::encryption(){
	if(Mode == DEC && !tableFilled) {
		cerr << "Wrong mode; DEC was inputted\n";
		return;
	}
	// auto start = chrono::steady_clock::now();

	expandKey();
	for(uint64_t i=0; i<T; i++)
		encrypt(i);

	// auto current = chrono::steady_clock::now();
	// auto elapsed = chrono::duration_cast<chrono::duration<double>>(current-start);
	// double chrono_time = elapsed.count();
	// cerr << "\tEncryption time: " << chrono_time << "\n";
}

string SpeckClass::decryption(){
	if(Mode == ENC && !tableFilled) {
		cerr << "Wrong mode; ENC was inputted\n";
		return "";
	}
	// auto start = chrono::steady_clock::now();

	expandKey();
	for(uint64_t i=0; i<T; i++)
		decrypt(T-1-i);

	stringstream plaintext;

	uint64_t messageLengthBytes = y.back();
	int count = 0;

	while(messageLengthBytes >= 16){
		// cout << "bytes: " << messageLengthBytes << "\n";
		plaintext << hex << setw(16) << setfill('0') << x[count];
		plaintext << hex << setw(16) << setfill('0') << y[count];
		count++;
		messageLengthBytes -= 16;
	}

	if(messageLengthBytes==0);
	else if(messageLengthBytes <= 8){
		int width = messageLengthBytes*2;
		plaintext << hex << setw(width) << setfill('0') << ((x[count] >> 4*(16-width)));
	}
	else{
		plaintext << hex << setw(16) << setfill('0') << x[count];
		messageLengthBytes -= 8;
		int width = messageLengthBytes*2;
		plaintext << hex << setw(width) << setfill('0') << ((y[count] >> 4*(16-width)));
	}

	// auto current = chrono::steady_clock::now();
	// auto elapsed = chrono::duration_cast<chrono::duration<double>>(current-start);
	// double chrono_time = elapsed.count();
	// cerr << "\tDecryption time: " << chrono_time << "\n";

	//Test plaintext padding output
	//cout << "output without padding:\n" << plaintext.str() << "\n";

	return plaintext.str();

}

void SpeckClass::printStuff(bool mode){
	printf("key: \t\t0x");
	for(int i=0; i<m-1; i++)
		printf("%016llx ", (u64) l[i]);
	printf("%016llx\n", (u64) key[0]);

	if(mode == ENC){
		printf("plaintext: \t0x");
	} else {
		printf("ciphertext: \t0x");
	}
	
	for(uint64_t i=0; i<x_orig.size(); i++)
		printf("%016llx %016llx ", (u64) x_orig[i], (u64) y_orig[i]);
	printf("\n");

	if(mode == ENC){
		printf("ciphertext: \t0x");
	} else {
		printf("plaintext: \t0x");
	}

	for(uint64_t i=0; i<x.size(); i++)
		printf("%016llx %016llx ", (u64) x[i], (u64) y[i]);
	printf("\n");
	
}

SpeckClass::SpeckClass(bool mode, int N, int t, vector<uint64_t> pt, vector<uint64_t> K){
	n = N; m = K.size(); T = (uint64_t) t;
	
	alpha = 8, beta = 3;
	if(n == 16) alpha=7, beta=2;

	for(int i=0; i<m-1; i++)
		l[(m-2)-i] = K[i];
	key[0] = K[m-1];

	Mode = mode;

	for(uint64_t i=0; i<pt.size(); i++){
		uint64_t temp = 0;
		temp = pt[i];
		if(i%2==0)
			x_orig.push_back(temp);
		else
			y_orig.push_back(temp);
	}
	x = x_orig;
	y = y_orig;
}

//Generates a random key using the openssl rand library
void keyGenerator(vector<uint64_t>& key){
	int result;
	unsigned char buf[8];
	
	stringstream ss;
	for(int i=0; i<CHOSENKEYWORDSIZE; i++){
		ss.str("");

		result = RAND_bytes(buf, 8);
		
		if(result){ //good
			ss << "0x";
			for(int k=0; k<8; k++)
				ss << hex << setw(2) << setfill('0') << (int) buf[k];

			key.push_back(stoull(ss.str().c_str(), 0, 0));

		}
		else{ //bad
			i--;
		}
	}
}


void createPlaintext(int mode, vector<uint64_t>& pt){
	//modifies empty vector
	//uses cin; piped input file into cin

	string word;

	string input;
	cin >> input;
	if(mode == ENC){
		if(input.size() % 2){
			string temp = "0";
			temp += input;
			input = temp;
		}
		int inputSize = input.size();

		int messageLengthBytes = inputSize / 2;

		//pad

		if(messageLengthBytes % 16 == 0){
			input += "1000000000000000";
			stringstream ss;
			ss << hex << setw(16) << setfill('0') << messageLengthBytes;
			input += ss.str();
		}
		else if (messageLengthBytes % 16 < 8){

			input += "10";
			while(input.size() % 16 != 0){
				input += "00";
			}

			stringstream ss;
			ss << hex << setw(16) << setfill('0') << messageLengthBytes;
			input += ss.str();

		}
		else{
			input += "10";
			while(input.size() % 16 != 0){
				input += "00";
			}
			input += "0000000000000000";

			stringstream ss;
			ss << hex << setw(16) << setfill('0') << messageLengthBytes;
			input += ss.str();
		}
	}

	//read 128-bit parts of the plaintext
	for(uint64_t i=0; i<input.size()/16; i++){
			word = "0x";		
			word += input.substr(i*16, 16);
			pt.push_back(stoull(word.c_str(), 0, 0));

	}


}

int main(int argc, char** argv){
    
	vector<uint64_t> pt, K;
	//Speck 128/128 test vector
	// 6c617669757165207469206564616d20
	// K.push_back(0x0f0e0d0c0b0a0908);
	// K.push_back(0x0706050403020100);


	createPlaintext(ENC, pt);

    keyGenerator(K);

    //Speck 128/256 test vector
     //65736f6874206e49202e72656e6f6f70
     //K.push_back(0xAAAAAAAAAAAAAAAA);
     //K.push_back(0xAAAAAAAAAAAAAAAA);
     //K.push_back(0xAAAAAAAAAAAAAAAA);
     //K.push_back(0xAAAAAAAAAAAAAAAA);

    
    //specky(encryption/decryption, word size, # rounds, plaintext vector, key)
	SpeckClass specky(ENC, WORDSIZE, CORRESPONDINGROUNDS, pt, K);
	specky.encryption();
	//Test output
	specky.printStuff(ENC);
	specky.decryption();

	if(argc > 1 && argv[1][1] == 'h'){
		cerr << "usage: ./speck_openssl.out < infile\n";
		return 0;
	}


	return 0;
}