#pragma once
#include <cstdint>
#include <cstddef>
#include <cassert>
#include <vector>
#include <stdexcept>

namespace crypto {
    enum class ECBMode { Encrypt, Decrypt };

    /**
     * ECB Mode Stream Wrapper
     */
    template <class stream_t, class cipher_t>
    class ECB {
     public:
     private:
        stream_t& stream;
        cipher_t& cipher;
        ECBMode mode;
        size_t counter;
        std::vector<uint8_t> buffer;
        std::vector<uint8_t> resultBuffer;
     public:
        ECB(stream_t& stream, cipher_t& cipher, ECBMode mode)
        : stream(stream), cipher(cipher), mode(mode), counter(0) {
            buffer.resize(cipher.getBlockSize());
            resultBuffer.resize(cipher.getBlockSize());
        }

        void write(const uint8_t* data, size_t size) {
            while (size > 0) {
                if ((counter + size) >= (size_t) cipher.getBlockSize()) {
                    if (counter > 0) {
                        size_t toCopy = cipher.getBlockSize() - counter;
                        std::copy(data, data + toCopy,
                            buffer.begin() + counter);
                        data += toCopy;
                        size -= toCopy;

                        if (mode == ECBMode::Encrypt) {
                            cipher.encryptBlock(buffer.data(),
                                resultBuffer.data());
                        } else {
                            cipher.decryptBlock(buffer.data(),
                                resultBuffer.data());
                        }
                        counter = 0;
                    } else {
                        size_t toCopy = cipher.getBlockSize();

                        if (mode == ECBMode::Encrypt) {
                            cipher.encryptBlock(data, resultBuffer.data());
                        } else {
                            cipher.decryptBlock(data, resultBuffer.data());
                        }

                        data += toCopy;
                        size -= toCopy;
                    }
                    stream.write(resultBuffer.data(), cipher.getBlockSize());
                } else {
                    std::copy(data, data + size, buffer.begin() + counter);
                    counter += size;
                    data += size;
                    size = 0;
                }
            }
        }
        void close() {
            if (counter != 0) {
                throw std::runtime_error(
                    "incomplete block at end of stream;"
                    " block will be discarded.");
            }
            stream.close();
        }
    };

}