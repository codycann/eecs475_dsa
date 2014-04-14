#include <iostream>
#include <chrono>
#include <ctime>
#include <ratio>

#include "simon.hpp"
#include "generator.hpp"

using std::cout;
using std::endl;

/**
 * @brief Class used for plaintext generation for the Simon cipher.
 */
class BlockGenerator {
    /**
     * @var The random number generator using Mersenne Twister.
     */
    std::mt19937 generator;
    /**
     * @var The uniform distribution to use for generating 0s and 1s.
     */
    std::uniform_int_distribution<char> distr;
    /**
     * @var The number of bits to make the generated block.
     */
    unsigned block_size;
public:
    /**
     * @brief Constructs the BlockGenerator class from a seed.
     */
    BlockGenerator(unsigned size, unsigned seed)
            : block_size(size), generator(seed), distr('0', '1') {}

    /**
     * @brief Returns a pseudorandomly generated key of BlockSize bits.
     */
    mpz_class operator() () {
        std::string binary_string;
        for (unsigned i = 0; i < block_size; ++i) {
            binary_string.push_back(distr(generator));
        }
        mpz_class result;
        result.set_str(binary_string, 2);
        return result;
    }
};

int main() {
    using namespace std::chrono;

    mpz_class plaintext, plaintext_test, ciphertext, ciphertext_test, key;
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();

    Simon s0(128, 256);
    key.set_str("1f1e1d1c1b1a191817161514131211100f0e0d0c0b0a09080706050403020100", 16);
    plaintext.set_str("74206e69206d6f6f6d69732061207369", 16);
    ciphertext_test = s0.encrypt(key, plaintext);
    ciphertext.set_str("8d2b5579afc8a3a03bf72a87efe7b868", 16);
    if (ciphertext != ciphertext_test) {
        cout << "(128, 256) failed to encrypt the correct ciphertext." << endl;
    } else if (plaintext != s0.decrypt(key, ciphertext)) {
        cout << "(128, 256) failed to decrypt the correct plaintext." << endl;
    } else {
        cout << "(128, 256) passed." << endl;
    }  
 
    KeyGenerator keys(256, seed);

    for (unsigned i = 1; i <= 8; i++) {
        double total_encrypt_time = 0.0;
        double total_decrypt_time = 0.0;
        unsigned pt_size = i * 16;
        BlockGenerator plaintexts(pt_size, seed);

        for (unsigned i = 0; i < 10000; ++i) {
            key = keys();
            plaintext = plaintexts();

            //clock_t start_encrypt = clock();
            high_resolution_clock::time_point start_encrypt = high_resolution_clock::now();
            ciphertext = s0.encrypt(key, plaintext);
            //clock_t end_encrypt = clock();
            high_resolution_clock::time_point end_encrypt = high_resolution_clock::now();
            duration<double> encrypt_time = duration_cast<duration<double>>(end_encrypt - start_encrypt);
            //total_encrypt_time += (double)(end_encrypt - start_encrypt) * 1000 / (double)CLOCKS_PER_SEC;
            total_encrypt_time += encrypt_time.count() * 1000;
             
   
            //clock_t start_decrypt = clock();
            high_resolution_clock::time_point start_decrypt = high_resolution_clock::now();
            plaintext_test = s0.decrypt(key, ciphertext);
            //clock_t end_decrypt = clock();
            high_resolution_clock::time_point end_decrypt = high_resolution_clock::now();
            duration<double> decrypt_time = duration_cast<duration<double>>(end_decrypt - start_decrypt);
            //total_decrypt_time += (double)(end_decrypt - start_decrypt) * 1000 / (double)CLOCKS_PER_SEC;
            total_decrypt_time += decrypt_time.count() * 1000;

            if (plaintext != plaintext_test) {
                cout << "Failure." << endl;
                exit(1);
            }

            /*if (i < 5) {
                cout << "Encrypt time = " << encrypt_time.count() * 1000 << endl;
                cout << "Decrypt time = " << decrypt_time.count() * 1000 << endl;
            }*/
        }

        double avg_encrypt_time = total_encrypt_time / 10000;
        double avg_decrypt_time = total_decrypt_time / 10000;

        cout << "For input size = " << pt_size << endl;
        cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
        cout << "Avg encrypt time = " << avg_encrypt_time << endl;
        cout << "Avg decrypt time = " << avg_decrypt_time << endl << endl;
    }

    return 0;
}
