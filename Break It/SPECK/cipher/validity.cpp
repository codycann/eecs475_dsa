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
    int iter = atoi(argv[1]);

    uberzahl key, text;
    speck s;

    for (int i = 0; i < iter; ++i) {
        key.random(256);
        text.random(rand() % 128 + 1);
        if (text != s.decrypt(key, s.encrypt(key, text))) {
            std::cout << "x != dec(enc(x))\n";
            exit(0);
        }
    }

    std::cout << "Testing successful: for " << iter << " iterations, x == dec(enc(x))\n";

    return 0;
}
