SPECK 128/256
=============

Team: 12 [Crypto Chevaliers]
Uniqname: nndalal, thnghia, mutalisk

*Note: our program assumes uberzahl has been cloned into a subdirectory called uberzahl.

Instructions:
	- `make normal` to compile
	- run with `./a.out [mode] [key] [text]`
	- [mode] can be either 'e' or 'd' standing for encrypt/decrypt
	- [key] can be any 256 bit number
	- [text] can be any 128 bit number (or less)

Examples:
	`./a.out e 14074904626401341155369551180448584754667373453244490859944217516317499064576 134851401336142382734221733825187114864`
	`./a.out d 14074904626401341155369551180448584754667373453244490859944217516317499064576 119614175882224391918291739847996153346`

Test Vectors:
You can run `make validity && ./a.out 1000` to run 1000 random test vectors of varying bit length.

key: 14074904626401341155369551180448584754667373453244490859944217516317499064576
plaintext: 134851401336142382734221733825187114864
ciphertext: 119614175882224391918291739847996153346

key: 14074904626401341155369551180448584754667373453244490859944217516317499064576
plaintext: 7
ciphertext: 285639349852887954561256787020905888857

Performance:
Please refer to the attached excel spreadsheet. We notice that for both encryption and decryption, we get a horizontal line at around 2-3 ms indicating that the runtimes of the Speck algorithms for encryption and decryption are independent of the size of the plaintext and ciphertext, respectively.

You can run `make performance && ./a.out [mode]` to reproduce the performance tests found in the attached excel spreadsheet. [mode] can be either 'e' or 'd' standing for encrypt/decrypt.
