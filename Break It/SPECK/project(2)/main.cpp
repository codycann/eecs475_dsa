#include "speck.h"
#include <vector>
#include <iostream>
#include <string>
#include <cstdlib>
#include <sstream>

using namespace std;

// n is desired length
string pad_with_zeroes(string s, int n) {
	string z("");
	for (int i = 0; i < n - s.length(); i++) z.push_back('0');
	return z + s;
}

int main(int argc, char **argv)
{
	if (argc < 2) {
		cout << "Too few arguments. Consult README file for details.\n";
		return 1;
	}
	if (string(argv[1]) == string("keygen")) {
		if (argc != 3) {
			cout << "Too " << (argc<3?"few":"many") << " arguments. " <<
				"Consult README file for details.\n";
			return 1;
		}
		int size = atoi(argv[2]);
		if (size != 128 && size != 192 && size != 256) {
			cout << "Invalid word size. Consult README file for details.\n";
			return 1;
		}
		vector<uint64> k = generate_key<uint64>(size / 64);
		for (int i = k.size() - 1; i >= 0; --i)
			cout << k[i] << ' ';
		cout << '\n';
	} else if (string(argv[1]) == string("encrypt") || string(argv[1]) == string("decrypt")) {
		vector<uint64> k;
		for (int i = argc - 3; i >= 2; --i)
			k.push_back(uint64(string(argv[i])));
		vector<uint64> sk = expandKey<uint64, uint128>(34, k);
		string ms1 = pad_with_zeroes(argv[argc - 2], 16);
		string ms2 = pad_with_zeroes(argv[argc - 1], 16);
		cout << "input:\t" << ms1 << ' ' << ms2 << '\n';
		uint128 m(ms1 + ms2), e;
		stringstream o;
		if (argv[1][0] == 'e')
			e = encipherBlock(m, 31 + (argc - 5), sk);
		else
			e = decipherBlock(m, 31 + (argc - 5), sk);
		o << e;
		string out1 = o.str().substr(0, 16), out2 = o.str().substr(16, 16);
		cout << "output:\t" << out1 << ' ' << out2 << '\n';
	} else {
		cout << "Invalid arguments. Consult README for details.\n";
		return 1;
	}

	return 0;
}
