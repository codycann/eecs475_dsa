#   Simon Cipher

Implementation the [SIMON](https://eprint.iacr.org/2013/404.pdf) cipher in C++.

##  How To Use

### CLI Mode

First, make sure you run `make` in the project directory to make the programs.  If you are in the CAEN environment this should correctly compile the programs.  Then you can interface with the SIMON cipher in the following ways.

#### Key Generation

Run the `keygen` command with the key size as an argument to generate a random key of at most that size.  The key will be output in **decimal**.

    ./simon keygen <key-size>

#### Encryption

Run the `encrypt` command to encrypt a message.  It requires 4 arguments, the block size, the key size, the key, and the plaintext.  The block size and key size must follow the constraints set out in table 3.1 (page 10) of the [Simon paper](https://eprint.iacr.org/2013/404.pdf).  The key and plaintext should be in **decimal**.  The ciphertext will also be output in **decimal**.

    ./simon encrypt <block size> <key size> <key> <plaintext>

#### Decryption

Run the `decrypt` command to decrypt a message.  It requires 4 arguments, the block size, the key size, the key, and the ciphertext.  The block size and key size must follow the constraints set out in table 3.1 (page 10) of the [Simon paper](https://eprint.iacr.org/2013/404.pdf).  The key and ciphertext should be in **decimal**.  The plaintext will also be output in **decimal**.

    ./simon decrypt <block size> <key size> <key> <ciphertext>

### Programmatically

First create the Simon object with the desired block size and key size.  Then make a call to `expand_key` to expand your key.  Then you can call `encrypt` and `decrypt` to encrypt and decrypt messages.  All numbers passed to functions must be of type mpz_class.  The `KeyGenerator` class may be used to generate pseudorandom keys of the correct key size.

    Simonsimon(<block-size>, <key-size>);
    KeyGenerator gen(<key-size>, <seed>);
    mpz_class key = gen(); /* A mpz_class number of KeySize bits */
    mpz_class ciphertext = simon.encrypt(key, plaintext /* A mpz_class number of at most block-size bits */);
    mpz_class plaintext = simon.decrypt(key, ciphertext /* A mpz_class number of at most block-size bits */);

#   Makefile

The Makefile is set up to compile the tests in the CAEN environment by default.  Simply enter `make` to make the tests in the CAEN environment.

#   TODO

*   Check for other implementation requirements that haven't been met.
*   Check for edge case mistakes.
*   Check for mistakes in the CLI.
