#include "speck.h"
#include <iostream>
#include <cstdlib>

int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cout << "Please read the readme.txt\n";
        exit(0);
    }

    char flag = *argv[1];
    uberzahl key = uberzahl(argv[2]);
    uberzahl text = uberzahl(argv[3]);
    speck s;

    if (flag == 'e') {
        std::cout << s.encrypt(key, text) << "\n";
    } else if (flag == 'd') {
        std::cout << s.decrypt(key, text) << "\n";
    } else {
        std::cout << "Please read the readme.txt\n";
    }

    return 0;
}
