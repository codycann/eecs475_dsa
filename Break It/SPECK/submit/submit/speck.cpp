
#include "speck.h"

#include "test.h"

#include <ctime>
#include <cassert>

using namespace std;

const map<pair<short, short>, short> speck::kNumRounds = {
    {{32, 64},   22},
    {{48, 72},   22},
    {{48, 96},   23},
    {{64, 96},   26},
    {{64, 128},  27},
    {{96, 96},   28},
    {{96, 144},  29},
    {{128, 128}, 32},
    {{128, 192}, 33},
    {{128, 256}, 34}
};

speck::speck(const short& blockSize, const short& keySize, const uberzahl& key)
      : blockSize_(blockSize),
        wordSize_(blockSize / 2),
        keySize_(keySize),
        keyWords_(keySize_ / wordSize_),
        T_(kNumRounds.at(make_pair(blockSize, keySize))),
        mod_(uberzahl("1") << wordSize_) {
    if(blockSize_ == 32 && keySize_ == 64) {
        alpha_ = 7;
        beta_ = 2;
    } else {
        alpha_ = 8;
        beta_ = 3;
    }
    roundKey_ = expand_key(key);
}

uberzahl speck::rotate_left(const uberzahl& value, unsigned shift) const {
    uberzahl upper = ((value << shift) % mod_);
    uberzahl lower = (value >> (wordSize_ - shift));
    return upper | lower;
}

uberzahl speck::rotate_right(const uberzahl& value, unsigned shift) const {
    uberzahl upper = ((value << (wordSize_ - shift)) % mod_);
    uberzahl lower = value >> shift;
    return upper | lower;
}

tuple<uberzahl, uberzahl> speck::feistel_add(const uberzahl& x, const uberzahl& y) const {
    return make_tuple(y, (rotate_right(x, alpha_) + y) % mod_);
}

tuple<uberzahl, uberzahl> speck::feistel_xor(const uberzahl& x, const uberzahl& y) const {
    return make_tuple(y, rotate_left(x, beta_) ^ y);
}

tuple<uberzahl, uberzahl> speck::feistel_add_inv(const uberzahl& x, const uberzahl& y) const {
    uberzahl value;
    if(x > y) {
        value = mod_ - (x - y) % mod_;
    } else {
        value = (y - x) % mod_;
    }
    return make_tuple(rotate_left(value, alpha_), x);
}

tuple<uberzahl, uberzahl> speck::feistel_xor_inv(const uberzahl& x, const uberzahl& y) const {
    return make_tuple(rotate_right(x ^ y, beta_), x);
}

vector<uberzahl> speck::expand_key(uberzahl key) const {
    uberzahl k0 = key % mod_;
    vector<uberzahl> roundKey = { k0 };
    vector<uberzahl> l;

    key = key >> wordSize_;
    for(int i = 0; i < keySize_ / wordSize_ - 1; i++) {
        l.push_back(key % mod_);
        key = key >> wordSize_;
    }

    for(int i = 0; i < T_ - 1; i++) {
        uberzahl a, b;
        tie(a, b) = feistel_add(l[i], roundKey[i]);
        b = b ^ i;
        tie(a, b) = feistel_xor(a, b);
        l.push_back(a);
        roundKey.push_back(b);
    }
    return roundKey;
}

uberzahl speck::encrypt_block(const uberzahl& plaintext) const {
    uberzahl x = plaintext >> wordSize_;
    uberzahl y = plaintext % mod_;
    for(int i = 0; i < T_; i++) {
        tie(x, y) = feistel_add(x, y);
        y = y ^ roundKey_[i];
        tie(x, y) = feistel_xor(x, y);
    }
    return (x << wordSize_) | y;
}

uberzahl speck::decrypt_block(const uberzahl& ciphertext) const {
    uberzahl x = ciphertext >> wordSize_;
    uberzahl y = ciphertext % mod_;
    for(int i = T_ - 1; i >= 0; i--) {
        tie(x, y) = feistel_xor_inv(x, y);
        y = y ^ roundKey_[i];
        tie(x, y) = feistel_add_inv(x, y);
    }
    return (x << wordSize_) | y;
}

uberzahl speck::genkey(short keySize) {
    static bool seeded = false;
    if(!seeded) {
        srand(time(NULL));
        seeded = true;
    }
    return uberzahl("0").random(keySize);
}

vector<uberzahl> speck::encrypt(const vector<uberzahl>& blocks) const {
    vector<uberzahl> result;
    for(const uberzahl& block : blocks) {
        result.push_back(encrypt_block(block));
    }
    return result;
}

vector<uberzahl> speck::decrypt(const vector<uberzahl>& blocks) const {
    vector<uberzahl> result;
    for(const uberzahl& block : blocks) {
        result.push_back(encrypt_block(block));
    }
    return result;
}
