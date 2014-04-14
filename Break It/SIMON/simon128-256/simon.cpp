//simon round function

#include <iostream>
#include <getopt.h>
#include "uberzahl.h"

using namespace std;

//GLOBALS
unsigned int m = 4;		//get (T, j) and m from definitions 
unsigned int j = 4;
unsigned int T = 72;
int WORD_SIZE = 64;
uberzahl THREE = 3;

//magic z array derived from magic math
unsigned long long z[5][62] = {
	{1,1,1,1,1,0,1,0,0,0,1,0,0,1,0,1,0,1,1,0,0,0,0,1,1,1,0,0,1,1,0,1,1,1,1,1,0,
		1,0,0,0,1,0,0,1,0,1,0,1,1,0,0,0,0,1,1,1,0,0,1,1,0},	
	{1,0,0,0,1,1,1,0,1,1,1,1,1,0,0,1,0,0,1,1,0,0,0,0,1,0,1,1,0,1,0,1,0,0,0,1,1,
		1,0,1,1,1,1,1,0,0,1,0,0,1,1,0,0,0,0,1,0,1,1,0,1,0},
	{1,0,1,0,1,1,1,1,0,1,1,1,0,0,0,0,0,0,1,1,0,1,0,0,1,0,0,1,1,0,0,0,1,0,1,0,0,
		0,0,1,0,0,0,1,1,1,1,1,1,0,0,1,0,1,1,0,1,1,0,0,1,1},
	{1,1,0,1,1,0,1,1,1,0,1,0,1,1,0,0,0,1,1,0,0,1,0,1,1,1,1,0,0,0,0,0,0,1,0,0,1,
		0,0,0,1,0,1,0,0,1,1,1,0,0,1,1,0,1,0,0,0,0,1,1,1,1},
	{1,1,0,1,0,0,0,1,1,1,1,0,0,1,1,0,1,0,1,1,0,1,1,0,0,0,1,0,0,0,0,0,0,1,0,1,1,
		1,0,0,0,0,1,1,0,0,1,0,1,0,0,1,0,0,1,1,1,0,1,1,1,1}
};


uberzahl logicalNot(uberzahl &x){
	//logical not (~) for uberzahl numbers
	uberzahl num = 1;
	num = (num << WORD_SIZE) - 1;
	return x ^ num;
}

uberzahl feistel(uberzahl x){
	//all that left shift'n in the feistel function
	uberzahl a, b, c, d;
	a = x.rotateLeft(1, 0, WORD_SIZE - 1);
	b = x.rotateLeft(8, 0, WORD_SIZE - 1);
	c = a & b;
	d = x.rotateLeft(2, 0, WORD_SIZE - 1);
	return (c ^ d);
}

pair<uberzahl, uberzahl> simonEnc(uberzahl x, uberzahl y, int T, uberzahl k[]){
	//returns a pair of uberzahls that when appended, is the encrypted plaintext
	uberzahl tmp;
	for(int i = 0; i < T; i++){
		tmp = x;
		x = y ^ feistel(x) ^ k[i];
		y = tmp;
	}
	pair<uberzahl, uberzahl> ans;
	ans = make_pair(y, x);
	return ans;
}

pair<uberzahl, uberzahl> simonDec(uberzahl x, uberzahl y, int T, uberzahl k[]){
	//returns a pair of uberzahls that when appended, is the decrypted ciphertext
	for (int i = T-1; i >= 0; i--) {
		uberzahl tmp = x;
		x = y ^ feistel(x) ^ k[i];
		y = tmp;
	}
	pair<uberzahl, uberzahl> ans;
	ans = make_pair(x, y);
	return ans;
}



int main(int argc, char **argv){

	//get commands
	int cflag = 0; int kflag = 0; int pflag = 0;
	
	uberzahl pt, ct, key;
	
	int c; int options_index;
	static struct option long_options[] = {
            {"cipher",required_argument,0,'c'},
            {"plain",required_argument,0,'p'},
            {"key",required_argument,0,'k'},
            {0,0,0,0}
    };

    //read flags
    while( (c=getopt_long(argc,argv,"c:p:k:", long_options,&options_index))!=-1 ) {
		switch(c) {
			case 'c':
				cflag=1;
				ct = optarg;
				break;
			case 'p':
				pflag=1;
				pt = optarg;
				break;
			case 'k':
				kflag=1;
				key = optarg;
				break;
			default:
				cout << "--- Manual Page --- \n\nUsage: -k key { -c ciphertext | -p plaintext }\n"; 
				exit(0);
		}
	}
	
	// getting m size key
	uberzahl k[T];
	k[3] = key>>192;
	k[2] = (key>>128) & ((1<<64)-1);
	k[1] = (key>>64) & ((1<<64)-1);
	k[0] = key & ((1<<64)-1);

	//getting px, py
	uberzahl px,py;	
	px = pt>>64;
	py = pt & ((1<<64)-1);
	
	//getting cx, cy
	uberzahl cx,cy;	
	cy = ct>>64;
	cx = ct & ((1<<64)-1);
	
	if (pflag && cflag) {
		cerr << "Don't put in both ciphertext and plaintext.\n";
		exit(1);
	}


//===========================  Key Expansion  ==================================

	//create T keys using key expansion on input key which is split into four
	for(unsigned int i = m; i < T; i++){
		uberzahl tmp = k[i - 1].rotateRight(3, 0, WORD_SIZE -1);
		if(m == 4){
			tmp = tmp ^ k[i - 3];
		}
		tmp = tmp ^ tmp.rotateRight(1, 0, WORD_SIZE - 1);
		k[i] = logicalNot((k[i-m]));
		k[i] = k[i]	^ tmp;
		k[i] = k[i]	^ z[j][(i-m) % 62];
		k[i] = k[i]	^ THREE;
	}

//===========================  Cipher  ========================================

	if (pflag) {
		//create ciphertext from plaintext
		pair<uberzahl, uberzahl> ciphertext = simonEnc(px, py, T, k);
		uberzahl result = (ciphertext.second << 64) + ciphertext.first;
		cout << "\n\nCipheretext is " << result << "\n\n\n"; 
	} else if (cflag) {
		//create plaintext from ciphertext
		pair<uberzahl, uberzahl> plaintext = simonDec(cx, cy, T, k);
		uberzahl result = (plaintext.second << 64) + plaintext.first;
		cout << "\n\nPlaintext is " << result << "\n\n\n";
	} else {
		cerr << "Usage: -k key { -c ciphertext | -p plaintext }\n";
		exit(1);
	}
	
	return 0;
}

