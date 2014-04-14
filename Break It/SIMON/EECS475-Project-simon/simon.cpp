#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <cstring>
#include <iostream>
#include <vector>
#include "uberzahl.h"

using namespace std;

#define zero uberzahl("0")
#define one uberzahl("1")

int n;
int m;
char z[5][63] = {"11111010001001010110000111001101111101000100101011000011100110",
    "10001110111110010011000010110101000111011111001001100001011010",
    "10101111011100000011010010011000101000010001111110010110110011",
    "11011011101011000110010111100000010010001010011100110100001111",
    "11010001111001101011011000100000010111000011001010010011101111"};
int T;
int j;
uberzahl x;
uberzahl y;
vector<uberzahl> k;
int key_sizes[] = {64, 72, 96, 128, 144, 192, 256};
int key_size;

uberzahl negation(uberzahl in) {
    uberzahl ans = in;
    for (int i = 0; i < n; ++i) {
        ans.toggleBit(i);
    }
    return ans;
}

bool is_valid_key_size(int size) {
    return find(key_sizes, key_sizes + 7, size) != key_sizes + 7;
}

void round_key_size() {
    if (key_size <= 64)
        key_size = 64;
    else if (key_size <= 72)
        key_size = 72;
    else if (key_size <= 96)
        key_size = 96;
    else if (key_size <= 128)
        key_size = 128;
    else if (key_size <= 144)
        key_size = 144;
    else if (key_size <= 192)
        key_size = 192;
    else 
        key_size = 256;
}

void key_generation(int size) {
    srand(time(0));
    uberzahl key = random(one << (size-1), (one << size) - one);
    cout << key << endl << key.bitLength() << endl;
}

void key_expansion() {
    for (int i = m; i < T; ++i) {
        uberzahl tmp = k[i-1].rotateRight(3, 0, n-1);
        if (m == 4)
            tmp = tmp ^ k[i-3];

        tmp = tmp ^ tmp.rotateRight(1, 0, n-1);
        k[i] = negation(k[i-m]) ^ tmp ^ uberzahl((largeType) (z[j][(i-m) % 62] - '0')) ^ uberzahl(3);
    }
}

uberzahl f(uberzahl in) {
    return (in.rotateLeft(1, 0, n-1) & in.rotateLeft(8, 0, n-1)) ^ in.rotateLeft(2, 0, n-1);
}

void setup() {
    switch(key_size) {
        case 64:
            n = 16;
            m = 4;
            break;
        case 72:
            n = 24;
            m = 3;
            break;
        case 96:
            n = 24;
            m = 4;
            break;
        case 128:
            n = 32;
            m = 4;
            break;
        case 144:
            n = 48;
            m = 3;
            break;
        case 192:
            n = 64;
            m = 3;
            break;
        case 256:
            n = 64;
            m = 4;
            break;
        default:
            cerr << key_size << "setup fails" << endl;
            exit(1);
    }

    switch(n) {
        case 16:
            T = 32;
            j = 0;
            break;
        case 24:
            T = 36;
            switch(m) {
                case 3:
                    j = 0;
                    break;
                case 4:
                    j = 1;
                    break;
            }
            break;
        case 32:
            switch(m) {
                case 3:
                    T = 42;
                    j = 2;
                    break;
                case 4:
                    T = 44;
                    j = 3;
                    break;
            }
            break;
        case 48:
            switch(m) {
                case 2:
                    T = 52;
                    j = 2;
                    break;
                case 3:
                    T = 54;
                    j = 3;
                    break;
            }
            break;
        case 64:
            switch(m) {
                case 2:
                    T = 68;
                    j = 2;
                    break;
                case 3:
                    T = 69;
                    j = 3;
                    break;
                case 4:
                    T = 72;
                    j = 4;
                    break;
            }
            break;
        default:
            cerr << "setup fails" << endl;
            exit(1);
    }
}

uberzahl encrypt(uberzahl key, uberzahl plaintext) {
    key_size = key.bitLength();
    round_key_size();
    setup();
    k.resize(T);
    for (int i = 0; i < m; ++i) {
        k[i] = key.extract(0, n-1);
        key = key >> n;
    }
    key_expansion();

    uberzahl ciphertext = zero;
    int count = 0;
    while (plaintext != zero) {
        y = plaintext.extract(0, n-1);
        plaintext = plaintext >> n;
        x = plaintext.extract(0, n-1);
        plaintext = plaintext >> n;

        for (int i = 0; i < T; ++i) {
            uberzahl tmp = x;
            x = y ^ f(x) ^ k[i];
            y = tmp;
        }

        ciphertext = ciphertext + (x << (count*2*n+n)) + (y << (count*2*n));
        ++count;

    }
    return ciphertext;
}

uberzahl decrypt(uberzahl key, uberzahl ciphertext) {
    key_size = key.bitLength();
    round_key_size();
    setup();
    k.resize(T);
    for (int i = 0; i < m; ++i) {
        k[i] = key.extract(0, n-1);
        key = key >> n;
    }
    key_expansion();

    uberzahl plaintext = zero;
    int count = 0;
    while (ciphertext != zero) {
        y = ciphertext.extract(0, n-1);
        ciphertext = ciphertext >> n;
        x = ciphertext.extract(0, n-1);
        ciphertext = ciphertext >> n;

        for (int i = 0; i < T; ++i) {
            uberzahl tmp = y;
            y = x ^ f(y) ^ k[T-i-1];
            x = tmp;
        }

        plaintext = plaintext + (x << (count*2*n+n)) + (y << (count*2*n));
        ++count;

    }
    return plaintext;
}

int main(int argc, char* argv[]) {
    if (argc == 3 && strcmp(argv[1], "keygen") == 0) {
        key_size = atoi(argv[2]);
        if (!is_valid_key_size(key_size)) {
            cerr << "Error: invalid key size, must be one of (64, 72, 96, 128, 144, 192, 256)" << endl;
            exit(1);
        }
        key_generation(key_size);
        return 0;
    }
    uberzahl key;
    if (argc == 4 && strcmp(argv[1], "encrypt") == 0) {
        uberzahl plaintext;
        key = uberzahl(argv[2]);
        plaintext = uberzahl(argv[3]);
        cout << encrypt(key, plaintext) << endl;
        return 0;
    }
    if (argc == 4 && strcmp(argv[1], "decrypt") == 0) {
        uberzahl ciphertext;
        key = uberzahl(argv[2]);
        ciphertext = uberzahl(argv[3]);
        cout << decrypt(key, ciphertext) << endl;
        return 0;
    }

    cerr << "Error: incorrect usage. See README.txt" <<endl;
    exit(1);
}

