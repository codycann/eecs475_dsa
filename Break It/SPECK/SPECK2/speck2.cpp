#include "speck2.h"

using namespace std;

int main (int argc, char **argv) {
	string option1 = "keygen";
	string option2 = "encrypt";
	string option3 = "decrypt";

	if(argc == 1) {
		cout << "One does not simply forget the arguments!\n";
		exit(1);
	}
	if(argv[1] == option1) {	//keygen
		keyGen();
	}
	else if(argv[1] == option2) {	//encrypt 2 pt
		Speck2 speck(argv[4], argv[5]);
		speck.setKeys(argv[2], argv[3]);
		speck.keyExpand();
		speck.encrypt();
		cout << speck.ct_x << ' ' << speck.ct_y << '\n';
	}
	else if(argv[1] == option3) {	//decrypt 2 ct
		Speck2 speck(argv[4], argv[5]);
		speck.setKeys(argv[2], argv[3]);
		speck.keyExpand();
		speck.decrypt();
		cout << speck.ct_x << ' ' << speck.ct_y << '\n';
	}
	else {
		cout << "Invalid option!\n";
		exit(1);
	}

	return 0;
}
