This is an implementation of El-Gamal encryption done by Team 43. tbrad, erichen, dquesada

to compile,
It can be included using 
#include "elGamal.cpp"

the only requirement for compiling is that it is compiled on a CAEN computer using g++.

To Encrypt:

create a new instance of ElGamal:
for example:

elGamel el;

to encrypt:

el.encrypt(m, r, s);

m is your plaintext message.
r and s are your outputs from your encryption and will be written over.
all are of the uberzahl type.

to decrypt:

el.decrypt(r, s, m);

m is your decrypted message and will be written over.
r and s are the outputs from the encryption.

to see your public keys:

el.publicKey();

outputs your generator, prime and y values.