#include <iostream>
#include <cstdlib>
#include "uberzahl.h"
#include "hash.h"
#include <fstream>
using namespace std;

int main(void){
	uberzahl m = 139762420523; //Test string in decimal
	//string test = "6162630d0a"; //Test string in hex
	//uberzahl m = uberzahl(stoul(test, 0, 16));
	uberzahl output = hash(m);
	cout << output << "\n";
	unsigned int bl = output.bitLength();
	cout << "Bit Length = " << bl << "\n";
	return 0;
}
