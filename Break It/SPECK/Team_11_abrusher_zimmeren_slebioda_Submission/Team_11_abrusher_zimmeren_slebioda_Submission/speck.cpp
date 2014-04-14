#include <iostream>
#include <cstdlib>
#include <vector>

#include "uberzahl.h"

using namespace std;

class Speck {
	int n; 												//word size = block size / 2
	int m;												//number of key words = key size / word size
	int T;												//number of rounds
	uberzahl two;										//Need uberzahl of value 2 in several places (uberzahl didn't work on literals for certain operations)
	uberzahl one;										//Need uberzahl of value 1 in several places (uberzahl didn't work on literals for certain operations)
	uberzahl modulus; 									//2^n - 1 (used to perform modular operations over modulo 2^n)
	pair<int, int> rotationConstants;					//(alpha, beta)
	vector<uberzahl> l;									//l keywords
	vector<uberzahl> k;									//k keywords
	uberzahl moduloAdd(uberzahl a, uberzahl b);			//returns (a + b) mod 2^n
	uberzahl moduloSubtract(uberzahl a, uberzahl b); 	//returns (a - b) mod 2^n
	void keyExpansion(vector<uberzahl> keys);			//Generates a key schedule k
	
public:
	Speck(int blockSize, int keySize);
	vector<uberzahl> generateKeys();						//Returns vector of m keys used as a parameter for key expansion, and stores expanded key vector in private data k
	vector<uberzahl> encrypt(vector<uberzahl> plaintext);	//Returns vector of encrypted plaintext
	vector<uberzahl> decrypt(vector<uberzahl> plaintext);	//Returns vector of decrypted ciphertext
};

uberzahl Speck::moduloAdd(uberzahl a, uberzahl b){
	return ((a + b) & modulus);
}

uberzahl Speck::moduloSubtract(uberzahl a, uberzahl b){
	return ((a + modulus + 1 - b) & modulus);
}

void Speck::keyExpansion(vector<uberzahl> keys){
	//Initialize l vector
	for(int i = m - 2; i >= 0; i--){
		l.push_back(keys[i]);
	}
	
	//Initialize k vector
	k.push_back(keys.back());
	
	//Perform expansion
	uberzahl i_u = "0";
	for(int i = 0; i < T-1; i++){
		l.push_back(moduloAdd(k[i], l[i].rotateRight(rotationConstants.first, 0, n - 1)) ^ i_u);
		k.push_back(k[i].rotateLeft(rotationConstants.second, 0, n - 1) ^ l[i + m - 1]);
		i_u = i_u + one;
	}
}

Speck::Speck(int blockSize, int keySize){
	//Initialize n, T, m, and rotationConstants as described in paper
	n = blockSize / 2;
	m = keySize / n;
	
	if(n == 16) T = 22;
	
	else if(n == 24){
		if(m == 3) T = 22;
		else if(m == 4) T = 23;
	}
	
	else if(n == 32){
		if(m == 3) T = 26;
		else if(m == 4) T = 27;
	}
	
	else if(n == 48){
		if(m == 2) T = 28;
		else if(m == 3) T = 29;
	}
	
	else if(n == 64){
		if(m == 2) T = 32;
		else if(m == 3) T = 33;
		else if(m == 4) T = 34;
	}
	
	if(n == 16) rotationConstants = make_pair(7, 2);
	else rotationConstants = make_pair(8, 3);
	
	two = "2";
	one = "1";
	
	modulus = two.exp(n) - one;
	
	srand(time(0));	
}


vector<uberzahl> Speck::generateKeys(){
	vector<uberzahl> keySeed;
	
	uberzahl lowerBound = two.exp(n - 1) - one;
	uberzahl upperBound = two.exp(n + 1) - one;
	
	for(int i = 0; i < m; i++){
		keySeed.push_back(random(lowerBound, upperBound) & modulus);
	}
	
	keyExpansion(keySeed);
	
	return keySeed;
}

vector<uberzahl> Speck::encrypt(vector<uberzahl> plaintext){
	uberzahl x = plaintext[0];
	uberzahl y = plaintext[1];
	
	for(int i = 0; i < T; i++){
		x = x.rotateRight(rotationConstants.first, 0, n - 1);
		x = (moduloAdd(x, y) ^ k[i]);
		y = (y.rotateLeft(rotationConstants.second, 0, n - 1) ^ x);
	}
	
	vector<uberzahl> retvec;
	retvec.push_back(x); retvec.push_back(y);
	
	return retvec;
}

vector<uberzahl> Speck::decrypt(vector<uberzahl> ciphertext){
	uberzahl x_enc = ciphertext[0];
	uberzahl y_enc = ciphertext[1];
	
	for(int i = T - 1; i >= 0; i--){
		uberzahl temp;
		
		y_enc = (x_enc ^ y_enc);
		y_enc = y_enc.rotateRight(rotationConstants.second, 0, n - 1);

		temp = moduloSubtract((x_enc ^ k[i]), y_enc);
		x_enc = temp.rotateLeft(rotationConstants.first, 0, n - 1);
	}
	
	vector<uberzahl> retvec;
	retvec.push_back(x_enc); retvec.push_back(y_enc);
	
	return retvec;
}

int main(){
	cout << "Example usage:" << endl;
	Speck s(128, 256);
	vector<uberzahl> keys = s.generateKeys();
	
	cout << "Keys: \t";
	for(unsigned int i = 0; i < keys.size(); i++){
		cout << keys[i] << "\t";
	}
	cout << endl;
	
	vector<uberzahl> plaintext;
	plaintext.push_back("7310309114568011337"); plaintext.push_back("2318916638112444272");
	
	cout << "Plaintext: \t";
	cout << plaintext[0] << "\t" << plaintext[1] << endl;
	
	vector<uberzahl> ciphertext = s.encrypt(plaintext);
	cout << "Ciphertext: \t";
	cout << ciphertext[0] << "\t" << ciphertext[1] << endl;
	
	plaintext = s.decrypt(ciphertext);
	cout << "Decrypted ciphertext: \t";
	cout << plaintext[0] << "\t" << plaintext[1] << endl;
}





