eecs 475 W14 Project team # 11

abrusher Alexander Brusher
slebioda Steven Lebioda
zimmeren Austin Zimmer



Speck implementation

To Use:

NOTE: Takes decimal input for all inputs 


First you must initiate the Speck class with the correct block and key sizes

Speck(int blockSize, int keySize)

ex. Speck s(32, 64);

must be one of the following bit combinations [blockSize/keySize]:
										32/64
										48/72
										48/96
										64/96
										64/128
										96/96
										96/144
										128/128
										128/192
										128/256



Next you must run the key expansion 

void Speck::keyExpansion(vector<uberzahl> keys)

ex. s.keyExpansion(keys);

each key must be less than then or equal to 1/4 bit size  of the keySize

NOTE: keyExpansion must be run before using encrypt or decrypt 

—————————————————————————————————————————

To encrypt

vector<uberzahl> Speck::encrypt(vector<uberzahl> plaintext)

s.encrypt(plaintext);

the vector must be in the order of [plaintext1 , plaintext2]

each plaintext must be less than or equal to 1/2 bit size of the blockSize

—————————————————————————————————————————

To decrypt

vector<uberzahl> Speck::decrypt(vector<uberzahl> ciphertext)

s.decrypt(ciphertext);

the vector must be in the order of [cyphertext1 , cyphertext2]

each cyphertext must be less than or equal to 1/2 bit size of the blockSize
