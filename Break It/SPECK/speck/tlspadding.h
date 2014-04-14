#pragma once
#include <cstdint>
#include <cstddef>
#include <cassert>
#include <vector>
#include <stdexcept>

namespace crypto {

    /**
     * TLS Padding stream wrapper
     *
     * Wrap this outside a cipher.
     *
     * See http://www.hit.bme.hu/~buttyan/courses/BMEVIHIM132/2010/04_block_enc_modes.pdf ,
     * slide 9 for the padding scheme used here. MAC is not implemented.
     */
    template <class stream_t, class cipher_t>
    class TLSPadding {
        stream_t& stream;
        cipher_t& cipher;
        size_t counter;
     public:
        TLSPadding(stream_t& stream, cipher_t& cipher)
        : stream(stream), cipher(cipher), counter(0) {}

        void write(const uint8_t* buffer, size_t size) {
            stream.write(buffer, size);
            counter = (counter + size) % cipher.getBlockSize();
        }
        void close() {
            size_t padding = cipher.getBlockSize() - counter;
            assert(padding <= 255);
            std::vector<uint8_t> paddingBuffer(padding, (uint8_t) padding);
            stream.write(paddingBuffer.data(), padding);
            stream.close();
        }
    };

    /**
     * TLS Padding removal stream wrapper
     *
     * Wrap the cipher outsidet this (since there is only write)
     *
     * 
     * See http://www.hit.bme.hu/~buttyan/courses/BMEVIHIM132/2010/04_block_enc_modes.pdf ,
     * slide 9 for the padding scheme used here. MAC is not implemented.
     */
    template <class stream_t, class cipher_t>
    class TLSDepadding {
        stream_t& stream;
        cipher_t& cipher;
        size_t counter;
        std::vector<uint8_t> buffer;
     public:
        TLSDepadding(stream_t& stream, cipher_t& cipher)
        : stream(stream), cipher(cipher), counter(0) {
            buffer.resize(cipher.getBlockSize());
        }

        void write(const uint8_t* data, size_t size) {
            assert(counter <= cipher.getBlockSize());
            while (size > 0) {
                if ((counter + size) > (size_t) cipher.getBlockSize()) {
                    size_t toCopy = cipher.getBlockSize() - counter;
                    if (counter > 0) {
                        stream.write(buffer.data(), counter);
                        counter = 0;
                    }
                    if (toCopy > 0) {
                        stream.write(data, toCopy);
                        data += toCopy;
                        size -= toCopy;
                    }
                } else {
                    std::copy(data, data + size, buffer.begin() + counter);
                    counter += size;
                    data += size;
                    size = 0;
                }
            }
        }

        void close() {
            if (counter == 0) {
                stream.close();
                return;
            }
            if (counter != (size_t) cipher.getBlockSize()) {
                throw std::runtime_error("invalid TLS padded stream length");
            }

            uint8_t paddingCount = buffer[cipher.getBlockSize() - 1];
            if (paddingCount > cipher.getBlockSize()) {
                throw std::runtime_error("invalid TLS pad length byte");
            }

            for (int i = cipher.getBlockSize() - paddingCount;
                i < cipher.getBlockSize(); i++) {
                if (buffer[i] != paddingCount) {
                    throw std::runtime_error("invalid TLS pad bytes");
                }
            }

            if (cipher.getBlockSize() - paddingCount > 0) {
                stream.write(buffer.data(),
                    cipher.getBlockSize() - paddingCount);
            }
            stream.close();
            counter = 0;
        }
    };
}