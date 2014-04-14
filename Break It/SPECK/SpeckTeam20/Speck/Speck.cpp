#include "Speck.h"

const unsigned int BLOCKSIZE = 128;
const unsigned int KEYSIZE = 256;
const unsigned int WORDSIZE = 64;
const unsigned int WORDS = 4;
const unsigned int ALPHA = 8;
const unsigned int BETA = 3;
const unsigned int ROUNDS = 34;
const uberzahl ZERO = "0";
const uberzahl MAXKEY = "115792089237316195423570985008687907853269984665640564039457584007913129639935";
const uberzahl MOD = "18446744073709551616";

std::array<uberzahl, ROUNDS> keyExpansion(uberzahl key);

uberzahl blockEncrypt(std::array<uberzahl, ROUNDS> &k, uberzahl block);

uberzahl blockDecrypt(std::array<uberzahl, ROUNDS> &k, uberzahl block);

uberzahl keygen() {
    srand(time(NULL));
    return random(ZERO, MAXKEY);
}

std::array<uberzahl, ROUNDS> keyExpansion(uberzahl key) {
    std::array<uberzahl, ROUNDS + WORDS - 2> l;
    std::array<uberzahl, ROUNDS> k;
    k[0] = key.extract(0, WORDSIZE - 1);
    for (unsigned int i = 0; i < WORDS - 1; ++i) {
        key = key >> WORDSIZE;
        l[i] = key.extract(0, WORDSIZE - 1);
    }
    for (unsigned int i = 0; i < ROUNDS - 1; ++i) {
        l[i + WORDS - 1] = (k[i] + l[i].rotateRight(ALPHA, 0, WORDSIZE - 1)).extract(0, WORDSIZE - 1) ^ i;
        k[i + 1] = k[i].rotateLeft(BETA, 0, WORDSIZE - 1) ^ l[i + WORDS - 1];
    }
    return k;
}

uberzahl blockEncrypt(std::array<uberzahl, ROUNDS> &k, uberzahl block) {
    uberzahl x, y;
    y = block.extract(0, WORDSIZE - 1);
    block = block >> WORDSIZE;
    x = block.extract(0, WORDSIZE - 1);
    for (unsigned int i = 0; i < ROUNDS; ++i) {
        x = (x.rotateRight(ALPHA, 0, WORDSIZE - 1) + y).extract(0, WORDSIZE - 1) ^ k[i];
        y = y.rotateLeft(BETA, 0, WORDSIZE - 1) ^ x;
    }
    block = x;
    block = block << WORDSIZE;
    block = block + y;
    return block;
}

uberzahl blockDecrypt(std::array<uberzahl, ROUNDS> &k, uberzahl block) {
    uberzahl x, y;
    y = block.extract(0, WORDSIZE - 1);
    block = block >> WORDSIZE;
    x = block.extract(0, WORDSIZE - 1);
    for (unsigned int i = 0; i < ROUNDS; ++i) {
        y = (x ^ y).rotateRight(BETA, 0, WORDSIZE - 1);
        x = ((x ^ k[ROUNDS - 1 - i]) + MOD - y).extract(0, WORDSIZE - 1).rotateLeft(ALPHA, 0, WORDSIZE - 1);
    }
    block = x;
    block = block << WORDSIZE;
    block = block + y;
    return block;
}

uberzahl encrypt(uberzahl key, uberzahl plaintext) {
    std::array<uberzahl, ROUNDS> k(keyExpansion(key));
    uberzahl block, ciphertext;
    do {
        block = plaintext.extract(0, BLOCKSIZE - 1);
        plaintext = plaintext >> BLOCKSIZE;
        ciphertext = ciphertext << BLOCKSIZE;
        ciphertext = ciphertext + blockEncrypt(k, block);
    } while (plaintext > ZERO);
    return ciphertext;
}

uberzahl decrypt(uberzahl key, uberzahl ciphertext) {
    std::array<uberzahl, ROUNDS> k(keyExpansion(key));
    uberzahl block, plaintext;
    do {
        block = ciphertext.extract(0, BLOCKSIZE - 1);
        ciphertext = ciphertext >> BLOCKSIZE;
        plaintext = plaintext << BLOCKSIZE;
        plaintext = plaintext + blockDecrypt(k, block);
    } while (ciphertext > ZERO);
    return plaintext;
}
