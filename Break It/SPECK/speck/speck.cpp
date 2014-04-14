#include <cassert>
#include <stdexcept>
#include <limits>
#include "./speck.h"

struct cipherConfig {
    int blockBitSize;
    int keyBitSize;
    int T;
};

// Speck specs p. 15
static const cipherConfig cipherConfigs[] = {
    { 32,  64, 22},
    { 48,  72, 22},
    { 48,  96, 23},
    { 64,  96, 26},
    { 64, 128, 27},
    { 96,  96, 28},
    { 96, 144, 29},
    {128, 128, 32},
    {128, 192, 33},
    {128, 256, 34},
};

static int lookupT(int blockBitSize, int keyBitSize) {
    for (auto& config : cipherConfigs) {
        if (config.blockBitSize == blockBitSize &&
            config.keyBitSize == keyBitSize) {
            return config.T;
        }
    }
    // Fall through, no valid bit size found
    throw std::runtime_error("invalid block size or key size");
}

static int lookupAlpha(int blockBitSize) {
    if (blockBitSize == 32) {
        return 7;
    } else {
        return 8;
    }
}

static int lookupBeta(int blockBitSize) {
    if (blockBitSize == 32) {
        return 2;
    } else {
        return 3;
    }
}

namespace crypto {
    template <class word_t>
    word_t SpeckGeneric<word_t>::generateWordMask(int n) {
        if ((size_t) n == sizeof(word_t) * 8) {
            return std::numeric_limits<word_t>::max();
        } else {
            return (1ULL << n) - 1;
        }
    }

    /**
     * Ensure only the right bits of the word w are non-zero
     */
    template <class word_t>
    word_t SpeckGeneric<word_t>::mask(word_t w) const {
        return w & wordMask;
    }

    /**
     * Note: rotate = circular shift. bits is the wrap around point
     */
    template <class word_t>
    word_t SpeckGeneric<word_t>::rotateLeft(word_t w, int by) const {
        assert(mask(w) == w);
        word_t rotation = w<<by;
        return mask(rotation | (w>>(n-by)));
    }

    template <class word_t>
    word_t SpeckGeneric<word_t>::rotateRight(word_t w, int by) const {
        assert(mask(w) == w);
        word_t rotation = w>>by;
        return mask(rotation | (w<<(n-by)));
    }

    /**
     * Convert bits_arg bits at the beginning of the byte array bytes into a word_t,
     * assuming big-endian. The first bits_arg / 8 bytes in bytes are read
     */
    template <class word_t>
    word_t SpeckGeneric<word_t>::bytesToWord(const uint8_t* bytes) const {
        assert(n % 8 == 0);
        int bits = n;
        word_t result = 0;
        while (bits > 0) {
            result <<= 8;
            result |= *(bytes++);
            bits -= 8;
        }
        assert(mask(result) == result);
        return result;
    }

    /**
     * Convert the word into a bits-bit number into the beginning of the array
     * bytes, assuming big-endian. The first bits / 8 bytes in the bytes array are
     * modified
     */
    template <class word_t>
    void SpeckGeneric<word_t>::wordToBytes(word_t word, uint8_t* bytes) const {
        assert(mask(word) == word);
        assert(n % 8 == 0);
        int bits = n;
        while (bits > 0) {
            *(bytes++) = word >> (bits - 8);
            bits -= 8;
        }
    }

    // @remarks: the initialization blocks are ugly, but they should allow
    // more compilier optimizations on rotate with the use of const.
    template <class word_t>
    SpeckGeneric<word_t>::SpeckGeneric(int blockBitSize, int keyBitSize)
    : n(blockBitSize / 2), wordMask(generateWordMask(n)), wordBytes(n / 8),
      m(keyBitSize / n), T(lookupT(blockBitSize, keyBitSize)),
      alpha(lookupAlpha(blockBitSize)), beta(lookupBeta(blockBitSize)) {
        if ((size_t) n > sizeof(word_t) * 8) {
            throw std::runtime_error("word size too big for this Speck instance");
        }
        k.resize(T);
    }

    template <class word_t>
    void SpeckGeneric<word_t>::setKey(const uint8_t* key) {
        // = l[], size = T - 2 + m
        std::vector<word_t> l(T - 2 + m);
        //copy key into l[m-2]..l[0], k[0]
        for(int i = 0;i<=m-2;i++){
            l[i] = bytesToWord(key + wordBytes * (m - 2 - i));
        }
        k[0] = bytesToWord(key + wordBytes * (m - 1));
        // generate k through the key expansion algorithm
        for(int i = 0; i <= T - 2; i++){
            int index_l = i + m - 1;
            l[index_l] = mask(k[i] + rotateRight(l[i], alpha)) ^ i;
            word_t key_inc = rotateLeft(k[i], beta) ^ l[index_l];
            k[i + 1] = key_inc;
        }
    }

    template <class word_t>
    void SpeckGeneric<word_t>::encryptBlock(const uint8_t* plaintext,
        uint8_t* ciphertext) const {
        word_t x, y;
        x = bytesToWord(plaintext);
        y = bytesToWord(plaintext + wordBytes);
        for(int i = 0; i <= T - 1; i++){
            x = mask(rotateRight(x, alpha) + y) ^ k[i];
            y = rotateLeft(y, beta) ^ x;
        }
        wordToBytes(x, ciphertext);
        wordToBytes(y, ciphertext + wordBytes);
    }

    template <class word_t>
    void SpeckGeneric<word_t>::decryptBlock(const uint8_t* ciphertext,
        uint8_t* plaintext) const {
        word_t x, y;
        x = bytesToWord(ciphertext);
        y = bytesToWord(ciphertext + wordBytes);
        for(int i = T - 1; i >= 0; i--){
            y = rotateRight(x ^ y,beta);
            x = rotateLeft(mask((x ^ k[i]) - y),alpha);
        }
        wordToBytes(x, plaintext);
        wordToBytes(y, plaintext + wordBytes);
    }

    template class SpeckGeneric<uint16_t>;
    template class SpeckGeneric<uint32_t>;
    template class SpeckGeneric<uint64_t>;
}
