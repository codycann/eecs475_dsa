#include <time.h>
#include "uberzahl.h"
#include <openssl/rand.h>
#include <iostream>
#include <iomanip>

using namespace std;

void genkey(){
	srand(time(0));
	unsigned char key[16];
	RAND_bytes(key, sizeof(key));
	unsigned char iv[12];
	RAND_bytes(iv, sizeof(iv));
	cout<<"key: ";
	for(int i=0;i<16;i++){
		cout<<(int)key[i];
	}
	cout<<endl;
	cout<<"iv: ";
	for(int i=0;i<12;i++){
		cout<<(int)iv[i];
	}
	cout<<endl;
}
