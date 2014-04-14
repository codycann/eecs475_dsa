How to use:
1. Compile:
	g++ dsa.cpp uberzahl.cpp -lgmp -lgmpxx -lssl -o program
2. Generate keys, outputs both pub and priv keys:
	./program keygen > key.txt
3. Have message in file such as "msg.txt" followed by a newline
4. Concat keys and message:
	cat msg.txt >> key.txt
5. Compute signature, note that key.txt now has the message appended to the key:
	./program sign < key.txt > sig.txt
6. Concat signature and key/message:
	cat sig.txt >> key.txt
7. Verify valid signature, note that signature is appended to message which is appended to key:
	./program verify < key.txt
8. Output will be either true or false depending on if the signature is valid.


Test vector uses dsatestvec.cpp:
	This tests the sign and verify functions using the first example from:
		http://csrc.nist.gov/groups/ST/toolkit/documents/Examples/DSA2_All.pdf
	k and z are hardcoded in since our randomness and hash differ
	
	input when testing sign is in "signinput.txt"
	correct output is in "correctsign.txt"

	input when testing verify is in "verifyinput.txt"
	correct output is in "correctverify.txt"