/**
 * For now, just implement everything in this header file.
 * Through EECS482, I have discovered that this is one of the fastest ways
 * to prototype a piece of code, without the complexity of separating
 * implementation from declaration, and allows rapid restructuring.
 */

#pragma once
#include <cstdint>
#include <vector>

namespace crypto {
    /**
     * The Speck Block Cipher. This is a template class allowing changing the
     * underlying word data type. This can be useful when using the algorithm
     * on resource constrained devices (that also support C++ compilers) like
     * the Arduino. To use the cipher, choose from one of Speck32, Speck64, and
     * Speck128 (typedefs) at the bottom of the file.
     *
     * Speck32: 32-bit maximum block size Speck
     * Speck64: 64-bit maximum block size Speck, including 48-bit and 32-bit.
     * Speck128: 128-bit maximum block size Speck, including all valid block
     * sizes.
     *
     * On a 64-bit machine, Speck128 has the best performance.
     *
     * Implemented according to https://eprint.iacr.org/2013/404.pdf
     */
    template <class word_t>
    class SpeckGeneric {
        // Internal functions
        inline word_t mask(word_t w) const;
        inline word_t rotateLeft(word_t w, int by) const;
        inline word_t rotateRight(word_t w, int by) const;
        inline word_t bytesToWord(const uint8_t* bytes) const;
        inline void wordToBytes(word_t word, uint8_t* bytes) const;

        static word_t generateWordMask(int n);

     public:
        const int n; // bits to a word
        const word_t wordMask;
        const int wordBytes; // number of bytes in a word
        const int m; // words to a key
        const int T; // rounds
        const int alpha;
        const int beta;

        std::vector<word_t> k;

     public:
        /**
         * Construct a new Speck cipher with given block and key sizes (in bits)
         */
        SpeckGeneric(int blockBitSize, int keyBitSize);

        /**
         * Set key.
         * @param key  the key in a big-endian byte array. of "m" words.
         * @remarks    the caller is responsible for filling in enough bytes
         *             into this array. The minimal size can be obtained through
         *             getKeySize()
         */
        void setKey(const uint8_t* key);

        /**
         * Encrypt a single block of plaintext into ciphertext.
         * @param plaintext  the plaintext in a big-endian byte array of two
         *                   words. X precedes Y in the array. (See the
         *                   specifications)
         * @param ciphertext ..
         * @remarks          the caller is responsible for allocating enough
         *                   space for both plaintext and ciphertext. The
         *                   minimal size can be obtained through getBlockSize()
         */
        void encryptBlock(const uint8_t* plaintext, uint8_t* ciphertext) const;

        /**
         * Decrypt a single block of ciphertext into plaintext.
         * @param ciphertext See encryptBlock
         * @param plaintext  ..
         * @remarks          the caller is responsible for allocating enough
         *                   space for both plaintext and ciphertext. The
         *                   minimal size can be obtained through getBlockSize()
         */
        void decryptBlock(const uint8_t* ciphertext, uint8_t* plaintext) const;

        /**
         * Get block size in bits
         */
        int getBlockBitSize() const {
            return n * 2;
        }

        /**
         * Get block size in bytes
         */
        int getBlockSize() const {
            return getBlockBitSize() / 8;
        }

        /**
         * Get key size in bits
         */
        int getKeyBitSize() const {
            return n * m;
        }

        /**
         * Get key size in bytes
         */
        int getKeySize() const {
            return getKeyBitSize() / 8;
        }
    };

    typedef SpeckGeneric<uint64_t> Speck;
    typedef SpeckGeneric<uint16_t> Speck32;
    typedef SpeckGeneric<uint32_t> Speck64;
    typedef Speck Speck128;
};