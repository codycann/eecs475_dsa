#include "475Project.h"
uberzahl keygen(int length) {
    uberzahl u;
    u.random(length);
    return u;
}

vector<uberzahl> encrypt(vector<uberzahl> key,vector<uberzahl> plaintext) {
    vector<uberzahl> result;
    for (int i = 0; i < key.size(); i++) {
        Present p(key[i], key[i].bitLength() / 8);
        result.push_back(p.encrypt(plaintext[i]));
    }
    return result;
}

vector<uberzahl> decrypt(vector<uberzahl> key, vector<uberzahl> ciphertext) {
    vector<uberzahl> result;
    for (int i = 0; i < key.size(); i++) {
        Present p(key[i], key[i].bitLength() / 8);
        result.push_back(p.decrypt(ciphertext[i]));
    }
    return result;
}
