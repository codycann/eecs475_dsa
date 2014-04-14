#include <iostream>
#include <cmath>
#include <string>
#include <cstdlib>
#include "encrypt.h"
#include "uberzahl.h"
#include "increment.h"
#include "multiplication.h"
#include "ghash.h"
#include "gctr.h"
#include "genkey.h"

using namespace std;

int getNextMultipleOf128(int size)
{
	int next_multiple = 0;
	while (next_multiple < size)
			next_multiple += 128;

	return next_multiple;
}

uberzahl concatSize(uberzahl x, uberzahl y, int size)
{
	x = x << size;
	x = x + y;

	return x;
}

uberzahl computePreCounter(uberzahl IV, uberzahl H)
{
	uberzahl J_0 = IV;
	uberzahl IV_bit_length_uber = IV.bitLength();
	int IV_bit_length = IV.bitLength();
	if (IV_bit_length == 96)
	{
		//IV || 0^31 || 1
		J_0 = J_0 << 32;
		J_0.setBit(0);
	}
	else
	{
		int next_multiple = getNextMultipleOf128(IV.bitLength());
		int bit_length_diff = next_multiple - IV.bitLength();
		int s = 128 * ceil((double) IV_bit_length/128) - IV_bit_length;
		J_0 = J_0 << (s + 64);
		J_0 = concatSize(J_0, IV_bit_length_uber, 64);
		J_0 = ghash(J_0, H, bit_length_diff);
	}

	return J_0;
}

CiphText_AuthTag encrypt_decrypt(uberzahl key, uberzahl IV, uberzahl A, uberzahl text, int bit_length_diff, bool encrypt_flag) {
	uberzahl zero;
	uberzahl H = ciph_k(key, zero);
	uberzahl J_0 = computePreCounter(IV, H);
	uberzahl C = gctr(key, text, increment(J_0), bit_length_diff);

	int A_length = A.bitLength();
	int T_length = text.bitLength();
	int C_length = C.bitLength();
	int u = 128 * ceil((double) C_length/128) - C_length;
	int v = 128 * ceil((double) A_length/128) - A_length;

	uberzahl A_bit_length = A.bitLength();
	uberzahl concat = A;
	concat = concat << v;
	if(encrypt_flag) {
		concat = concatSize(concat, C, C.bitLength());
		if (A == zero)
		{
			A_bit_length = "0";
			concat = C;
		}
	}
	else {
		concat = concatSize(concat, text, text.bitLength());
		if (A == zero)
		{
			A_bit_length = "0";
		}
	}


	int next_multiple_C = getNextMultipleOf128(C.bitLength());
	if(!encrypt_flag) {
		u += C_length - T_length ;
		// A_bit_length = A_bit_length - (C_length - T_length);
	}
	concat = concat << u;


	concat = concatSize(concat, A_bit_length, 64);

	if(!encrypt_flag) {
		concat = concatSize(concat, text.bitLength()+1, 64);
	}
	else {
		concat = concatSize(concat, C.bitLength()+1, 64);
	}

	int next_multiple = getNextMultipleOf128(concat.bitLength());
	uberzahl S = ghash(concat, H, next_multiple - concat.bitLength() + 1);

	next_multiple = getNextMultipleOf128(S.bitLength());
	uberzahl T = gctr(key, S, J_0, next_multiple - S.bitLength());
	if (T.bitLength() > 128)
		T = betterExtract(T, T.bitLength() - 128, T.bitLength() - 1);
	T = T^ciph_k(key, J_0);
	CiphText_AuthTag CA;
	CA.ciphertext = C;
	CA.authtag = T;
  	return CA;
}

bool checkUpperBoundSize(uberzahl val, uberzahl max_len, string val_name)
{
	if ((uberzahl) val.bitLength() > max_len)
	{
		cout << val_name << " is too long" << endl;
		cout << "Maximum bit length is: " << max_len << endl;
		return 1;
	}

	return 0;
}

CiphText_AuthTag encrypt(uberzahl key, uberzahl IV, uberzahl A, uberzahl text, int bit_length_diff) {
	CiphText_AuthTag CAR = encrypt_decrypt(key, IV, A, text, bit_length_diff, true);
	return CAR;
}

uberzahl decrypt(uberzahl key, uberzahl IV, uberzahl A, CiphText_AuthTag CA, int bit_length_diff) {
	CiphText_AuthTag CAR = encrypt_decrypt(key, IV, A, CA.ciphertext, bit_length_diff, false);
	cout << "Passed: " << CA.authtag << endl;
	cout << "Calculated: " << CAR.authtag << endl;
	if(CAR.authtag != CA.authtag) {
		cout << "Authentication failed. Authtags do not match.";
		exit(1);
	}
	return CAR.ciphertext;
}

int main(int argc, char *argv[])
{
	string option(argv[1]);
	if(option == "-keygen") {
		genkey();
		return 0;
	}

	if (!(argc >= 6)){
		cout << "usage: ./encrypt [-encrypt/-decrypt/-keygen] [128-bit AES key] [96-bit IV] [Additional Authenticated Data (must be same for encryption and decryption)] [plaintext/ciphertext] [Auth Tag (decryption only)]" << endl;
		return 1;
	}

	uberzahl key = argv[2];
	uberzahl IV = argv[3];
	uberzahl A = argv[4];
	uberzahl plaintext = argv[5];

	//Error checking
	uberzahl max_P_len = 2;
	max_P_len = max_P_len.exp(39);
	max_P_len = max_P_len - 256;
	if (checkUpperBoundSize(plaintext, max_P_len, "Plaintext"))
		return 1;

	uberzahl max_A_and_IV_len = 2;
	max_A_and_IV_len = max_A_and_IV_len.exp(64) - 1;
	if (checkUpperBoundSize(A, max_A_and_IV_len, "Additional Authentication Data"))
		return 1;

	if (checkUpperBoundSize(IV, max_A_and_IV_len, "IV"))
		return 1;

	cout << IV.bitLength() << endl;
	if (IV.bitLength() <= 96)
	{
		cout << "IV not at least 96 bits" << endl;
		cout << "For IVs, it is recommended that implementations restrict support to the length of 96 bits, to promote interoperability, efficiency, and simplicity of design." << endl;
		return 1;
	}

	uberzahl ctext;
	int next_multiple = 0;
	int bit_length_diff = 0;
	if(option == "-encrypt") {
		next_multiple = getNextMultipleOf128(plaintext.bitLength());
		bit_length_diff = next_multiple - plaintext.bitLength();
		CiphText_AuthTag ret = encrypt(key, IV, A, plaintext, bit_length_diff);
		ctext = ret.ciphertext;
		cout << "Auth Tag: " << ret.authtag << endl;
		cout << "Encryption: " << ctext <<endl;
	}
	else if(option == "-decrypt") {
		uberzahl tag = argv[6];
		next_multiple = getNextMultipleOf128(plaintext.bitLength());
		bit_length_diff = next_multiple - plaintext.bitLength();
		CiphText_AuthTag CA;
		CA.ciphertext = plaintext;
		CA.authtag = tag;
		ctext = decrypt(key, IV, A, CA, bit_length_diff);
		cout << "Decryption: " << ctext << endl;;
	}

	return 0;
}
