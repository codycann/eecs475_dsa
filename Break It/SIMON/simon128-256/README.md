Simon128/256 
	- A lightweight block cipher
	- Takes in 256 bit block key and 128 bit block input and outputs 128 bit block
		output. All argument input should be decimal. Simon128/256 uses the
		uberzahl library to handle Arbitrary-precision arithmetic.
	
 -compile using c++11 (ex. "g++ simon.cpp uberzahl.cpp -std=c++11 -o simon")
	Ignore the warnings about left bitshifting numbers that your compiler may
	give you.

 -simon has three flags
	-k [key] : key is a 256 bit decimal number. The key is required.

	-p [plaintext] : plaintext is a 128 bit block of plaintext. Simon will 
		output a 128 block of ciphertext.

	-c [ciphertext] : ciphertext is a 128 bit block of ciphertext. Simon 
		will output a 128 block of plaintext.

	(Note: our implementation does not support using both -p and -c)

 -Input to simon128/256 is a 256 bit key and 128 bit block of plaintext or 128 
	block of ciphertext. The output will be a 128 bit block of ciphertext 
	resulting from the input key and input plaintext or a 128 bit block of plaintext
	resulting from the input key and input ciphertext.

	Simon128/256 splits the 256 bit key into four 64 bit keys and then uses those
	four keys to build 68 additional keys. Then the 128 bit plaintext is split into
	two 64 bit blocks of text which is put into the rounds with each of the 72 keys.
	
	The process is reversed using the same keys to get the plaintext from ciphertext.

	If a key is smaller than 198 bits then one of the first four keys will be zero
	and if it is smaller than 128 bits then two of the first four keys will be zero
	and if it is smaller than 64 bits then three of the first four keys will be zero.
	Don't choose a small key. Please.

 -Examples:

	"./simon -k 14074904626401341155369551180448584754667373453244490859944217516317499064576 -p 154358840404572640889229863092020605801"
	Has output which is Enc(x): 187646149816416544869629532715157534824

	"./simon -k 14074904626401341155369551180448584754667373453244490859944217516317499064576 -c 187646149816416544869629532715157534824"
	Has output which is Dec(x): 154358840404572640889229863092020605801

 
