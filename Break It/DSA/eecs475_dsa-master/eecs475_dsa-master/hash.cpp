#include "hash.h"
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <cassert>
using namespace std;
//POSSIBLE LOGICAL ERROR ON LINES 65-68

//all addition must be mod 2^32
static uberzahl add(uberzahl x){
	uberzahl mod = uberzahl(4294967296);
	return (x % mod);
}

uberzahl sha256(uberzahl m, mediumType N){
//Initialize hash values:
	uberzahl h0 = uberzahl(stoul("0x6a09e667", 0, 16));
	uberzahl h1 = uberzahl(stoul("0xbb67ae85", 0, 16));
	uberzahl h2 = uberzahl(stoul("0x3c6ef372", 0, 16));
	uberzahl h3 = uberzahl(stoul("0xa54ff53a", 0, 16));
	uberzahl h4 = uberzahl(stoul("0x510e527f", 0, 16));
	uberzahl h5 = uberzahl(stoul("0x9b05688c", 0, 16));
	uberzahl h6 = uberzahl(stoul("0x1f83d9ab", 0, 16));
	uberzahl h7 = uberzahl(stoul("0x5be0cd19", 0, 16));

//Initialize array of round constants
	vector<string> hexValues =    
	{"0x428a2f98", "0x71374491", "0xb5c0fbcf", "0xe9b5dba5", "0x3956c25b", "0x59f111f1", "0x923f82a4", "0xab1c5ed5",
   	"0xd807aa98", "0x12835b01", "0x243185be", "0x550c7dc3", "0x72be5d74", "0x80deb1fe", "0x9bdc06a7", "0xc19bf174",
   	"0xe49b69c1", "0xefbe4786", "0x0fc19dc6", "0x240ca1cc", "0x2de92c6f", "0x4a7484aa", "0x5cb0a9dc", "0x76f988da",
   	"0x983e5152", "0xa831c66d", "0xb00327c8", "0xbf597fc7", "0xc6e00bf3", "0xd5a79147", "0x06ca6351", "0x14292967",
   	"0x27b70a85", "0x2e1b2138", "0x4d2c6dfc", "0x53380d13", "0x650a7354", "0x766a0abb", "0x81c2c92e", "0x92722c85",
  	"0xa2bfe8a1", "0xa81a664b", "0xc24b8b70", "0xc76c51a3", "0xd192e819", "0xd6990624", "0xf40e3585", "0x106aa070",
  	"0x19a4c116", "0x1e376c08", "0x2748774c", "0x34b0bcb5", "0x391c0cb3", "0x4ed8aa4a", "0x5b9cca4f", "0x682e6ff3",
  	"0x748f82ee", "0x78a5636f", "0x84c87814", "0x8cc70208", "0x90befffa", "0xa4506ceb", "0xbef9a3f7", "0xc67178f2"};
	uberzahl array[64];
	for(int i = 0; i < 64; i++){
	array[i] = uberzahl(stoul(hexValues[i], 0, 16));
	}
	
//Pre-Processing
	//append a bit '1' to the message
	uberzahl bit = uberzahl(m.bitLength()); //Bit length of m
	int k;
	m = m << 1;
	m.setBit(0);
	//append k "0" bits
	int msqSize = m.bitLength()%512;
	if(msqSize < 448) k = 448 - msqSize;
	else k = 960 - msqSize;
	m = m << k;
	//append message length in 64 bits
	m = m << 64;
	m = m | bit;

//Process the message in successive 512-bit chucks
	int chunks = m.bitLength()/512;
	if(m.bitLength()%512 > 0) chunks++; //shouldn't execute if done correctly
	for(int j = 0; j < chunks; j++){
		uberzahl w[64] = "";
		for(int i = 0; i < 16; i++){
		int start = j*512+i*32;
		uberzahl ext = m.extract(start, start + 32);
		w[i] = ext >> start; 
		//Do the pieces come from the start or end of m?
		//int end = m.bitLength() - (j*512+i*32);
		//uberzahl ext = m.extract(end-32, end);
		//w[i] = ext >> end -32; 
		}

		//Extend the first 16 words into the remaining 48 words of the message array
		for(int i = 16; i <=63; i++){
			uberzahl s0 = w[i-15].rotateRight(7, 0, 31) ^ w[i-15].rotateRight(18, 0, 31) ^ (w[i-15] >> 3);
			uberzahl s1 = w[i-2].rotateRight(17, 0, 31) ^ w[i-2].rotateRight(19, 0, 31) ^ (w[i-2] >> 10);
			w[i] = add(w[i-16] + s0 + w[i-7]+ s1);
		}

		//Initialize working variables to current hash value:
		uberzahl a = h0;
		uberzahl b = h1;
		uberzahl c = h2;
		uberzahl d = h3;
		uberzahl e = h4;
		uberzahl f = h5;
		uberzahl g = h6;
		uberzahl h = h7; 

		//Compression function main loop
		for(int i = 0; i <= 63; i++){
			uberzahl not_e = e; //create a "bitwise NOT e"
			for(int k = 0; k < 32; k++) not_e.toggleBit(k);
			uberzahl s1 =(e.rotateRight(6,0,31)) ^ (e.rotateRight(11, 0 ,31)) ^ (e.rotateRight(25,0,22));
			uberzahl ch = (e & f) ^ ((not_e) & g);
			uberzahl temp1 = add(h + s1 + ch + array[i] + w[i]);
			uberzahl s0 = (a.rotateRight(2,0,31)) ^ (a.rotateRight(13, 0 ,31)) ^ (a.rotateRight(22,0,31));
			uberzahl maj = (a & b) ^ (a & c) ^ (b & c);
			uberzahl temp2 = add(s0 + maj);
			h = g;
			g = f;
			f = e;
			e = add(d + temp1);
			d = c;
			c = b;
			b = a;
			a = add(temp1 + temp2);
		}

		//Add the compressed check to the curent hash value
		h0 = add(h0 + a);
		h1 = add(h1 + b);
		h2 = add(h2 + c);
		h3 = add(h3 + d);
		h4 = add(h4 + e);
		h5 = add(h5 + f);
		h6 = add(h6 + g);
		h7 = add(h7 + h);
	}
//Produce final hash value
	//append h0 through h7 by bit shifting and then appending with a bitwise OR
	uberzahl temp = h0;
	temp = temp << 32;
	temp = temp | h1;
	temp = temp << 32;
	temp = temp | h2;
	temp = temp << 32;
	temp = temp | h3;
	temp = temp << 32;
	temp = temp | h4;
	temp = temp << 32;
	temp = temp | h5;
	temp = temp << 32;
	temp = temp | h6;
	temp = temp << 32;
	temp = temp | h7;

	// TODO what if N is bigger than 256?
	return temp >> (256 - N);
}

