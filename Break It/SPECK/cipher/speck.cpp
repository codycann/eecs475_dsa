#include "speck.h"

void speck::keyExpansion(uberzahl K, std::vector<uberzahl> &k, std::vector<uberzahl> &l) {
    k.push_back(K % np); // get k0
    K = K << n;
    l.push_back(K % np); // get l0
    K = K << n;
    l.push_back(K % np); // get l1
    K = K << n;
    l.push_back(K % np); // get l2

    for (int i = 0; i <= T-2; ++i) {
        l.push_back(((k.at(i) + l.at(i).rotateRight(a, 0, n-1)) % np) ^ i);
        k.push_back(k.at(i).rotateLeft(b, 0, n-1) ^ l.at(i+m-1));
    }

    K = uberzahl("0"); // possibly prevents an adversary from stack smashing to get the key
}

uberpair speck::split(uberzahl m) {
    // given a 128 bit input, splits it into two 64 bit numbers and returns a pair
    uberpair ret = std::make_pair(m / np, m % np);
    return ret;
}

uberzahl speck::join(uberzahl x, uberzahl y) {
    // given two 64 bit numbers, constructs and returns a 128 bit uberzahl
    return (x << n) + y;
}

speck::speck() {
    np = 1;
    np = np << n; // 2^n used for addition and subtraction
}

uberzahl speck::encrypt(uberzahl key, uberzahl plaintext) {
    std::vector<uberzahl> k, l;
    keyExpansion(key, k, l);

    uberpair xy = split(plaintext);
    uberzahl x = xy.first, y = xy.second;

    for (int i = 0; i <= T-1; ++i) {
        x = ((x.rotateRight(a, 0, n-1) + y) % np) ^ k.at(i);
        y = y.rotateLeft(b, 0, n-1) ^ x;
    }

    key = uberzahl("0"); k.clear(); l.clear(); // possibly prevents an adversary from stack smashing to get the key and its expansion
    return join(x, y);
}

uberzahl speck::decrypt(uberzahl key, uberzahl ciphertext) {
    std::vector<uberzahl> k, l;
    keyExpansion(key, k, l);

    uberpair xy = split(ciphertext);
    uberzahl x = xy.first, y = xy.second;

    for (int i = T-1; i >= 0; i--) {
        y = (x^y).rotateRight(b, 0, n-1);
        x = (((x^k.at(i)) + np - y) % np).rotateLeft(a, 0, n-1);        
    }

    key = uberzahl("0"); k.clear(); l.clear(); // possibly prevents an adversary from stack smashing to get the key and its expansion
    return join(x, y);
}
