README

Isaiah Hines - ihines@umich.edu
Anuraag Veturi - anuraagv@umich.edu
Jagdishwar Singh - jnsingh@umich.edu


See main.cpp for examples of usage.

See speck.h for interface functions.



Key Generation:
	Speck128GenerateKey()

Sequence Encryption Functions:
	Speck128EncryptSequence()
	Speck128DecryptSequence()



Padding Scheme:
	The paper does not specify how to pad plaintext messages that are not block alligned. As such our padding scheme generates a block of random values that are seeded by the key. Decryption reverses the process such that the padding is removed. There are many more details, but this is just a basic explanation.
