Group 4: Dinker Ambe (dambe), Angus Lo (anguslo), Aaron Podell (ajpodell)
SPECK 128/128 ECB mode

Key generation, encryption, and decryption are member functions of class Speck. To use them, create an instance of Speck and call its functions.

uberzahl genKey() // Returns randomly generated 128-bit key
uberzahl encrypt(uberzahl key, uberzahl plaintext) // returns ciphertext
uberzahl decrypt(uberzahl key, uberzahl ciphertext) // returns plaintext



Example code:

Speck speck;
uberzahl key = speck.genKey();
uberzahl ciphertext = speck.encrypt(key, plaintext);
uberzahl deciphered = speck.decrypt(key, ciphertext);



Test vectors:

Key:        20011376718272490338853433276725592320
Plaintext:  144062678019685919772856771483193666848
Ciphertext: 221137820289473687857657110085594713368

Key:        0
Plaintext:  144062678019685919772856771483193666848
Ciphertext: 1000444146172915417172513437297852742

Key:        1
Plaintext:  152037107215241021449068138290853736037
Ciphertext: 197518563100430107023787069588039329421

Key:        1442304682729575737805390361686
Plaintext:  111111111111111111111111111111111
Ciphertext: 32096542000650209713025123876611961071

Key:        12345678901234567890123456
Plaintext:  111111111111111111111111111111111
Ciphertext: 215825222094735503735129327493762389553

Key:        5176156079219501789779
Plaintext:  1189998819991197253
Ciphertext: 84750170069882828649038465849818186684

Key:        1189998819991197253
Plaintext:  1189998819991197253
Ciphertext: 132247081460443199909686876827324754061