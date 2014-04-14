#ifndef SPECK_H
#define SPECK_H

#include <map>
#include <vector>
#include <tuple>

#include "uberzahl.h"

class speck {

public:
    // Initialize a speck cipher
    speck(const short& blockSize, const short& keySize, const uberzahl& key);
    speck(const short& blockSize, const short& keySize)
        : speck(blockSize, keySize, genkey(keySize)) {}

    static uberzahl genkey(short keySize);
    // Encrypt/decrypt a list of blocks each of which must be within the specified
    // block size. Returns a list of blocks each of which is the encrypted or
    // decrypted version of the corresponding plain/cipher text blocks.
    std::vector<uberzahl> encrypt(const std::vector<uberzahl>& blocks) const;
    std::vector<uberzahl> decrypt(const std::vector<uberzahl>& blocks) const;

    // Convenience methods to encrypt/decrypt single blocks
    uberzahl encrypt_block(const uberzahl& plaintext) const;
    uberzahl decrypt_block(const uberzahl& ciphertext) const;

private:
    static const std::map<std::pair<short, short>, short> kNumRounds;

    short wordSize_;
    short blockSize_;
    short keySize_;
    short keyWords_;
    short T_;
    short alpha_;
    short beta_;
    uberzahl mod_;
    std::vector<uberzahl> roundKey_;

    uberzahl rotate_left(const uberzahl& value, unsigned shift) const;
    uberzahl rotate_right(const uberzahl& value, unsigned shift) const;

    // Forward feistel functions (for encryption)
    std::tuple<uberzahl, uberzahl> feistel_add(const uberzahl& x, const uberzahl& y) const;
    std::tuple<uberzahl, uberzahl> feistel_xor(const uberzahl& x, const uberzahl& y) const;

    // Backward feistel functions (for decryption)
    std::tuple<uberzahl, uberzahl> feistel_add_inv(const uberzahl& x, const uberzahl& y) const;
    std::tuple<uberzahl, uberzahl> feistel_xor_inv(const uberzahl& x, const uberzahl& y) const;

    // Expand the master key into the complete round key.
    std::vector<uberzahl> expand_key(uberzahl key) const;
};

#endif
