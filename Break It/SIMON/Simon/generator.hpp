#ifndef GENERATOR_HPP_
#define GENERATOR_HPP_

#include <random>
#include <string>

/**
 * @brief Class used for key generation for the Simon cipher.
 */
class KeyGenerator {
    /**
     * @var The random number generator using Mersenne Twister.
     */
    std::mt19937 generator;
    /**
     * @var The uniform distribution to use for generating 0s and 1s.
     */
    std::uniform_int_distribution<char> distr;
    /**
     * @var The number of bits to make the generated key.
     */
    unsigned key_size;
public:
    /**
     * @brief Constructs the KeyGenerator class from key size and a seed.
     */
    KeyGenerator(unsigned size, unsigned seed) 
            : key_size(size), generator(seed), distr('0', '1') {}

    /**
     * @brief Returns a pseudorandomly generated key of KeySize bits.
     */
    mpz_class operator() () {
        std::string binary_string;
        for (unsigned i = 0; i < key_size; ++i) {
            binary_string.push_back(distr(generator));
        }
        mpz_class result;
        result.set_str(binary_string, 2);
        return result;
    }
};

#endif // GENERATOR_HPP_
