#include <iostream>
#include <cstdlib>
#include "uberzahl.h"
#include "hash.h"
#include <fstream>
using namespace std;

int main(void){
	//uberzahl m = 1397624205233; //Test string in decimal
	//string test = "abc"; //Test string in hex
	//uberzahl m = uberzahl(stoul(test, 0, 16));
	uberzahl m = "";
	uberzahl output = hash(m);
	cout << output << "\n";
	unsigned int bl = output.bitLength();
	cout << "Bit Length = " << bl << "\n";
	return 0;
}
