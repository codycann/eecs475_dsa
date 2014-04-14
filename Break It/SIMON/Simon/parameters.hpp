#ifndef PARAMETERS_HPP_
#define PARAMETERS_HPP_

#include <exception>


/**
 * @brief Parameter class for Simon.  Variables are named and set according to 
 * "https://eprint.iacr.org/2013/404.pdf" Table 3.1.
 */
struct Parameters {
    Parameters(unsigned block_size_, unsigned key_size_)
            : block_size(block_size_)
            , key_size(key_size_)
            , n(block_size_ / 2)
            , m(2 * key_size_ / block_size_) {
        if (!((block_size == 32 && key_size == 64)
                || (block_size == 48 && (key_size == 72 || key_size == 96))
                || (block_size == 64 && (key_size == 96 || key_size == 128))
                || (block_size == 96 && (key_size == 96 || key_size == 144))
                || (block_size == 128 && (key_size == 128 || key_size == 192 || key_size == 256)))) {
            throw std::invalid_argument("The block size and key size must follow the specifications laid out in 'https://eprint.iacr.org/2013/404.pdf' Table 3.1");
        }
        // Set T and seq based on table 3.1.
        T = (block_size == 32)                      ? 32 :
            (block_size == 48)                      ? 36 :
            (block_size == 64 && key_size == 96)    ? 42 :
            (block_size == 64 && key_size == 128)   ? 44 :
            (block_size == 96 && key_size == 96)    ? 52 :
            (block_size == 96 && key_size == 144)   ? 54 :
            (block_size == 128 && key_size == 128)  ? 68 :
            (block_size == 128 && key_size == 192)  ? 69 :
            (block_size == 128 && key_size == 256)  ? 72 : 0;

        seq =
            (block_size == 38 || (block_size == 48 && key_size == 72)) ? 0 :
            (block_size == 48 && key_size == 96) ? 1 :
            ((block_size == 64 && key_size == 96) || (block_size == 96 && key_size == 96) || (block_size == 128 && key_size == 128)) ? 2 :
            ((block_size == 64 && key_size == 128) || (block_size == 96 && key_size == 144) || (block_size == 128 && key_size == 192)) ? 3 :
            (block_size == 128 && key_size == 256) ? 4 : 0;
    }
protected:
    /// @var block_size The block size used by the Simon cipher (2n).
    const unsigned block_size = block_size;
    /// @var key_size The key size used by the Simon cipher (mn).
    const unsigned key_size = key_size;
    /// @n The variable n used by the Simon cipher.
    const unsigned n = block_size / 2;
    /// @m The variable m used by the Simon cipher.
    const unsigned m = 2 * key_size / block_size;
    /// @T The variable 'T' used by the Simon cipher.
    unsigned T;
    /// @seq Which expansion sequence in z to use.
    unsigned seq;
    /// @var z   Sequence used in key expansion.
    const unsigned z[5][62] = {
        {1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 1, 1, 0, 0, 0, 0, 1, 1, 1, 0, 0, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 1, 1, 0, 0, 0, 0, 1, 1, 1, 0, 0, 1, 1, 0},
        {1, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 1, 0, 1, 1, 0, 1, 0},
        {1, 0, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 1, 1, 0, 1, 1, 0, 0, 1, 1},
        {1, 1, 0, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 1, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 0, 0, 1, 1, 1, 1},
        {1, 1, 0, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 0, 1, 0, 1, 1, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 1, 1, 1, 0, 1, 1, 1, 1}
    };
};

#endif // PARAMETERS_HPP_
