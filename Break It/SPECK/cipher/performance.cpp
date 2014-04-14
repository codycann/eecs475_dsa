#include "speck.h"
#include <iostream>
#include <cstdlib>
#include <time.h>

int main(int argc, char* argv[]) {
    srand(time(NULL));

    if (argc != 2) {
        std::cout << "Please read the readme.txt\n";
        exit(0);
    }
    char flag = *argv[1];

    clock_t start, end;
    uberzahl key, text, res;
    speck s;

    for (int i = 1; i <= 128; ++i) {
        key.random(256);
        text.random(i);
        start = clock();
	    if (flag == 'e') {
        	res = s.encrypt(key, text);
	    } else if (flag == 'd') {
        	res = s.decrypt(key, text);
	    } else {
	        std::cout << "Please read the readme.txt\n";
	    }
        end = clock();
        std::cout << i << "\t" << (end-start)*1000/CLOCKS_PER_SEC << "\n";
    }

    return 0;
}
