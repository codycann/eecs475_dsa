#include <iostream>
#include "uberzahl.h"

using namespace std;

main() {
	uberzahl a = "27";
	uberzahl b = "12";
	uberzahl c = a ^ b;
	uberzahl d = ror(a, 2);	
	uberzahl e = rol(a, 2);
	cout << "ror(a = '11', 2): " << d << endl;
	cout << "rol(a = '11', 2): " << e << endl;
	
	return 0;
}
