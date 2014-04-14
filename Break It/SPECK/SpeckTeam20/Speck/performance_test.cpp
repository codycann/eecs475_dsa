#include <algorithm>
#include <getopt.h>
#include <ctime>
#include <cassert>
#include "Speck.h"


using namespace std;

uberzahl hex2dec(const char *text_arg);
//converts text_arg in hexadecimal into a decimal number

string dec2hex(uberzahl value);
//converts value in decimal into a hexadecimal string


int main(){
		clock_t encStart, encEnd, decStart, decEnd;
		float encTime = 0, decTime = 0;
    //code goes here
		uberzahl key;
		uberzahl plaintext, ciphertext;
		for(int bitsize = 16; bitsize <= 128; bitsize += 16){
			encTime = 0;
			decTime = 0;
			for(int i = 0; i < 1000; ++i){
					key = keygen();
					plaintext = plaintext.random(bitsize);
					//cout << plaintext << endl;
					encStart = clock();
					ciphertext = encrypt(key, plaintext);
					//cout << ciphertext << endl;
					encEnd = clock();
					encTime += (float) encEnd - (float) encStart;
					
					decStart = clock();
					assert(plaintext == decrypt(key, ciphertext)); 
					decEnd = clock();
					decTime += (float) decEnd - (float) decStart;
			}
			cout <<  bitsize << "\t" << encTime / 1000.0 << "\t" << decTime / 1000.0 << endl;
		}
    return 0;

}

