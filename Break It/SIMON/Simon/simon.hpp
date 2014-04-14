#ifndef SIMON_HPP_
#define SIMON_HPP_

#include <gmpxx.h>
#include <vector>
#include <iostream>
#include <exception>

#include "parameters.hpp"

class Simon : private Parameters {
    /**
     * @var Stores the value 2^n - 1 for use in calculations.
     */
    mpz_class two_n_minus_one;

    /**
     * @brief Left circular shift of x by i bits.
     * @param value The number to perform the shift on.
     * @param shift The number of bits to shift.
     */
    inline mpz_class S(const mpz_class& value, int shift) const noexcept;

    /**
     * @brief Performs the f(x) function defined on page 8, used in encryption
     * and decryption.
     * @param x An n bit number.
     */
    inline mpz_class f(const mpz_class& x) const noexcept;

    /**
     * @brief Expands the input key into a list of keys.
     * @details First it clears all existing round keys, then it breaks the
     * input key into its m words of n bits.  Then it generates the rest of the
     * keys (i+m to T-1) according to the key expansion function on page 11.
     * @param key The input key, it must be at most m*n bits long.
     * @returns The list of T round key.
     */
    std::vector<mpz_class> expand_key(const mpz_class& key) const;
public:
    /**
     * @brief Constructor to set two_n_minus_one and pass sizes to superclass.
     */
    Simon(unsigned block_size, unsigned key_size) 
            : Parameters(block_size, key_size) {
        two_n_minus_one = 1;
        two_n_minus_one = (two_n_minus_one << this->n) - 1;
    }

    /**
     * @brief Encrypts the plaintext using key which must be a 2*n bit number.
     * @details First break the plaintext up into 2 n-bit integers, then apply
     * the round function T times and then combine x and y back into a 2*n-bit
     * integer.
     * @param plaintext A 2*n-bit integer to encrypt.
     */
    mpz_class encrypt(const mpz_class& key, const mpz_class& plaintext) const;

    /**
     * @brief Decrypts the ciphertext using key which must be a 2*n bit number.
     * @details First break the ciphertext into 2 n-bit integers, then apply
     * the inverse round function (round function called with x and y switched)
     * T times and then combine x and y back into a 2*n-bit integer.
     * @param ciphertext a 2*n-bit integer to decrypt.
     */
    mpz_class decrypt(const mpz_class& key, const mpz_class& ciphertext) const;

    /**
     * @brief Returns the block size.
     */
    unsigned get_block_size() const noexcept { return this->block_size; }
};

mpz_class Simon::encrypt(const mpz_class& key, const mpz_class& plaintext) const {
    // Check that the input isn't too large.
    mpz_class largest = 1;
    largest = largest << (2*this->n);
    if (plaintext >= largest) {
        throw std::invalid_argument("The plaintext must not be longer than BlockSize (2n).");
    }

    std::vector<mpz_class> k = this->expand_key(key);

    // Split the plaintext into two blocks of size n.
    mpz_class x = (plaintext >> this->n) & this->two_n_minus_one;
    mpz_class y = plaintext & this->two_n_minus_one;

    // See section 3.1 (page 8)
    for (unsigned i = 0; i < this->T; ++i) {
        mpz_class tmp = x;
        x = y ^ this->f(x) ^ k[i];
        y = tmp;
    }

    return (x << this->n) | y;
}

mpz_class Simon::decrypt(const mpz_class& key, const mpz_class& ciphertext) const {
    // Check that the input isn't too large.
    mpz_class largest = 1;
    largest = largest << (this->block_size);
    if (ciphertext >= largest) {
        throw std::invalid_argument("The ciphertext must not be longer than BlockSize (2n).");
    }

    std::vector<mpz_class> k = this->expand_key(key);

    // Split the plaintext into two blocks of size n.
    mpz_class x = (ciphertext >> this->n) & this->two_n_minus_one;
    mpz_class y = ciphertext & this->two_n_minus_one;

    // See section 3.1 (page 8)
    for (int i = this->T-1; i >= 0; --i) {
        mpz_class tmp = y;
        y = x ^ this->f(y) ^ k[i];
        x = tmp;
    }

    return (x << this->n) | y;
}

std::vector<mpz_class> Simon::expand_key(const mpz_class& key) const {
    // Check that the input isn't too large.
    mpz_class largest = 1;
    largest = largest << (this->key_size);
    if (key >= largest) {
        throw std::invalid_argument("The key must not be longer than KeySize (m*n).");
    }

    std::vector<mpz_class> k;

    // Break the input key into its m parts of size n.
    for (unsigned i = 0; i < this->m; ++i) {
        k.push_back((key >> (i * this->n)) & this->two_n_minus_one);
    }

    // Implement k_{i+m} function on page 11 to generate the rest of the keys.
    mpz_class c = 1;
    c = (c << this->n) - 4;
    for (unsigned i = 0; i < (this->T - this->m); ++i) {
        mpz_class tmp;
        switch (this->m) {
            case 2:
                tmp = this->S(k[i+1], -3);
                break;
            case 3:
                tmp = this->S(k[i+2], -3);
                break;
            case 4:
                tmp = this->S(k[i+3], -3) ^ k[i+1];
                break;
            default:
                std::cerr << "Bad m value, this should be impossible." << std::endl;
                exit(1);

        }
        k.push_back(c ^ this->z[this->seq][i % 62] ^ k[i] ^ tmp ^ this->S(tmp, -1));
    }
    return k;
}

mpz_class Simon::S(const mpz_class& value, int shift) const noexcept {
    if (shift == 0) {
        return value;
    } else if (shift > 0) {
        return ((value << shift) | (value >> (this->n - shift))) & this->two_n_minus_one;
    } else {
        shift = -1 * shift;
        return ((value >> shift) | (value << (this->n - shift))) & this->two_n_minus_one;
    }
}

mpz_class Simon::f(const mpz_class& x) const noexcept {
    return (this->S(x, 1) & this->S(x, 8)) ^ this->S(x, 2);
}

#endif // SIMON_HPP_
