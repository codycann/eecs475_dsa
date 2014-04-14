Please put simon.cpp, uberzahl.cpp and uberzahl.h in one directory and compile them with the command below:
g++ simon.cpp uberzahl.cpp -o program
You can replace program with any name you want.

Use the command below to generate key:
./program keygen (key_size)
replace (key_size) with a decimal number. Please use a valid key size (listed below). 

Use the command below to encrypt:
./program encrypt (key) (plaintext)
Replace (key) and (plaintext) with decimal numbers.

Use the command below to decrypt:
./program decrypt (key) (ciphertext)
Replace (key) and (ciphertext) with decimal numbers.

The program will choose the block size according to the key size.
Valid key sizes and the corresponding block sizes:
key size	block size
64		32
72		48
96		48
128		64
144		96
192		128
256		128
