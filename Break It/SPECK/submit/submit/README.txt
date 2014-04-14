Use make to build. Requires C++11 including the <chrono> module.

To use the speck cipher you must first choose a block/key size. The following are
the only valid key/block size values:

    Block Size   Key Size
    32           64
    48           72
    48           96
    64           96
    64           128
    96           96
    96           144
    128          128
    128          192
    128          256

Once selected, create a key of the specified size by calling

    uberzahl masterkey = speck::genkey(keysize);

(this must be of the correct key size). Then to create your cipher, create a
speck object as follows:

    speck cipher(blocksize, keysize, masterkey);

To encrypt/decrypt a block (must be the correct size) use:

    uberzahl ciphertext = cipher.encrypt_block(block); // or
    uberzahl plaintext = cipher.decrypt_block(block);

To encrypt/decrypt a group of blocks put each block as an element of a vector
and call:

    vector<uberzahl> ciphertext = cipher.encrypt(block_vector); // or
    vector<uberzahl> plaintext = cipher.decrypt(block_vector);

The main method currently runs all published vectors testing the encryption and
decryption functions using the given keys, plain and cipher texts. If a test
vector passes it prints

    Test N passes: true

If it fails:

    Test N passes: false
