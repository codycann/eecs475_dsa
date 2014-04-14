SPECK README


COMPILATION
Compile with:
g++ speck.cpp -o speck
with "uberzahl.h" and "uberzahl.cpp" in the same directory

GENERATING KEYS
To generate keys, compile and run:
./speck -k
Three 64-bit keys will be printed to command line.

ENCRYPTION
To encrypt, compile and run:
./speck -e <PLAINTEXT> <KEY1> <KEY2> <KEY3>

DECRYPTION
To decrypt, compile and run:
./speck -d <CIPHERTEXT> <KEY1> <KEY2> <KEY3>

All inputs and outputs are in base 10 decimal.

