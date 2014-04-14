//Speck encryption algorithm with 128 bit key and plaintext
//EECS 475 group 15 - Anonymous

#include <string>
#include <vector>

#define LCS _lrotl 							//left circular shift
#define RCS _lrotr 							//right circular shift
#define u64 unsigned long long
#define MAXKEYWORDS 4
#define MAXROUNDS 34

enum modes{ ENC, DEC };

class SpeckClass{
private:
	int n;											//word size 
	int m;											//number of words in key
	uint64_t T;										//number of rounds
	int alpha, beta;								//amts to circular shift
	std::vector<uint64_t> x_orig, y_orig;			//plaintext words (2)
	std::vector<uint64_t> x_final, y_final;			//ciphertext words (2)

private:
	uint64_t l[MAXROUNDS+MAXKEYWORDS-1];			//all words but 1 word of key. Little endian. 
	uint64_t key[MAXROUNDS+1]; 						//last word of key. Little endian. 
	bool tableFilled=0;		
	bool Mode;

	void expandKey();								//private function that creates the table of keys
														//for each round
	void encrypt(uint64_t round);					//private function that expands the plaintext 
														//to the next "round's" plaintext
	
	void decrypt(uint64_t round);					//private function that expands the ciphertext 
														//to the previous "round's" ciphertext

public:
	std::vector<uint64_t> x, y;						//plain/ciphertext word-vectors: each odd word
														//is in x; each even word is in y

	void encryption();								//encrypts x_orig, y_orig into x,y
	std::string decryption();						//decrypts x,y and returns a string of the resulting plaintext
	void changeMode(bool mode);						//changes mode between ENC and DEC
	void printStuff(bool mode);						//prints useful information 
									
													//constructor: see README
	SpeckClass(bool mode, int N, int t, std::vector<uint64_t> pt, std::vector<uint64_t> K);
};


void keyGenerator(std::vector<uint64_t>& key);

void createPlaintext(int mode, std::vector<uint64_t>& pt);

