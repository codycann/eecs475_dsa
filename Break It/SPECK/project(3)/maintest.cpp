#include <iostream>
#include <cstdint>
#include <cstdlib>
#include <stdexcept>
#include <string>
#include <sstream>
#include <vector>
#include "speck.h"

using namespace std;

word_t* getKey(const string &in) {
	word_t* key = new word_t[4]();
	stringstream sstream;
	char garbage;
	sstream << in;

	for(int i = 0; i < 4; i++) {
		sstream >> hex >> key[i];
		sstream >> garbage;	
	}
	sstream.clear();

	return key;
}

void checkParamters(int argc, char *argv[]) {
	if(argc != 6 && argc != 4) {
		throw std::invalid_argument("ERROR: Incorrect number of parameters!\n");
	}

	string cmd = string(argv[1]);
	if(cmd != "keygen" && cmd != "encrypt" && cmd!= "decrypt" && cmd!= "encrypt-long" && cmd!="decrypt-long") {
		throw std::invalid_argument("ERROR: Invalid command: '" + cmd + "'\n");
	}
}

int main(int argc, char *argv[]) {
	checkParamters(argc, argv);

	size_t word_size = unsigned(atoi(argv[2]));
	size_t key_size = unsigned(atoi(argv[3]));

	cout << "Wordsize:" << word_size << " Keysize:" << key_size << endl;
	speck Result(word_size, key_size);

	if(string(argv[1]) == "keygen") {
		word_t* keygen = Result.gen_key();
		cout << "\nOutput Key:\n----------\n";
		cout << hex << keygen[0] << ',' << keygen[1] << ',' << keygen[2] << ',' << keygen[3] << endl;
	
	} else if (string(argv[1]) == "encrypt") {
		word_t* key = getKey(string(argv[4]));
		stringstream sstream;
		char garbage;

		cout << "\nInput Key:\n---------\n" << hex << key[0] << ',' << key[1] << ',' << key[2] << ',' << key[3] << endl;
		Result.set_key(key);

		vector<pair<word_t, word_t>> ciphertext;

		sstream << string(argv[5]);
		unsigned long p1;
		unsigned long p2;


		while(sstream.good()) {
			sstream >> hex >> p1;
			sstream >> garbage;
			sstream >> hex >> p2;
			sstream >> garbage;

			ciphertext.push_back(Result.encrypt(p1, p2));
		}

		cout << "\nCiphertext:\n----------\n";
		for(auto i = ciphertext.begin(); i != ciphertext.end(); i++) {
			cout << hex << i->first << ',' << i->second << ',';
		}
		cout << "\b " << endl;
	
	} else if (string(argv[1]) == "decrypt") {
		word_t* key = new word_t[4]();
		stringstream sstream;
		char garbage;
		sstream << string(argv[4]);

		for(int i = 0; i < 4; i++) {
			sstream >> hex >> key[i];
			sstream >> garbage;	
		}
		sstream.clear();

		cout << "\nInput Key:\n---------\n" << hex << key[0] << ',' << key[1] << ',' << key[2] << ',' << key[3] << endl;
		Result.set_key(key);

		vector<pair<word_t, word_t>> plaintext;

		sstream << string(argv[5]);
		unsigned long p1;
		unsigned long p2;

		while(sstream.good()) {
			sstream >> hex >> p1;
			sstream >> garbage;
			sstream >> hex >> p2;
			sstream >> garbage;

			plaintext.push_back(Result.decrypt(p1, p2));
		}

		cout << "\nPlaintext:\n---------\n";
		for(auto i = plaintext.begin(); i != plaintext.end(); i++) {
			cout << hex << i->first << ',' << i->second << ',';
		}
		cout << "\b " << endl;
	} else if(string(argv[1]) == "encrypt-long") {
		word_t* key = getKey(string(argv[4]));
		stringstream sstream;
		char garbage;

		cout << "\nInput Key:\n---------\n" << hex << key[0] << ',' << key[1] << ',' << key[2] << ',' << key[3] << endl;
		Result.set_key(key);
		
		mpz_t enc_num;
		mpz_t cypher;
		mpz_init(enc_num);
		mpz_init(cypher);
		gmp_sscanf(argv[5], "%Zx", enc_num);
		//cypher = Result.encrypt(mpz_class(enc_num)).get_mpz_t();
		gmp_printf("\nCiphertext:\n---------\n%Zx\n", Result.encrypt(mpz_class(enc_num)).get_mpz_t());
	} else if(string(argv[1]) == "decrypt-long") {
		word_t* key = new word_t[4]();
		stringstream sstream;
		char garbage;
		sstream << string(argv[4]);

		for(int i = 0; i < 4; i++) {
			sstream >> hex >> key[i];
			sstream >> garbage;	
		}
		sstream.clear();

		cout << "\nInput Key:\n---------\n" << hex << key[0] << ',' << key[1] << ',' << key[2] << ',' << key[3] << endl;
		Result.set_key(key);
		
		mpz_t dec_num;
		mpz_t text;
		mpz_init(dec_num);
		mpz_init(text);
		gmp_sscanf(argv[5], "%Zx", dec_num);
		//text = Result.decrypt(mpz_class(dec_num)).get_mpz_t();
		gmp_printf("\nPlaintext:\n---------\n%Zx\n", Result.decrypt(mpz_class(dec_num)).get_mpz_t());
	}
	return 0;
}


// int main()
// {
// 	speck test(16u, 64u);
// 	word_t* keygen = test.gen_key();
// 	cout << hex << keygen[0] << ',' << keygen[1] << ',' << keygen[2] << ',' << keygen[3] << endl;
// 	word_t key[4] = {0x1918ul, 0x1110ul, 0x0908ul, 0x0100ul};
// 	test.set_key(key);
// 	auto result = test.encrypt(0x6574ul, 0x694cul);
// 	cout << hex << result.first << ',' << result.second << endl;
// 	auto re2 = test.decrypt(result.first, result.second);
// 	cout << re2.first << ',' << re2.second << endl;
// 	return 0;
// }
