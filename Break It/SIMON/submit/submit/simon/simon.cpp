// SIMON with n=64 and m=4
#include <iostream>
#include <cstdlib>
#include "uberzahl.h"
#include "simon.h"
#include <cassert>
#include <iomanip>
#include <vector>
#include <string.h>
#include <time.h>
using namespace std;

extern int z[5][62];

int n = 64;
int m = 4;

int T = 72;
int j = 4;

uberzahl key;

const uberzahl shift = 0xffffffffffffffff; 
const uberzahl take_128_bit = (shift << n) | shift;

int main(int argc, char **argv){
     srand(time(0));
    //uberzahl new_key = generate();
    vector<uberzahl> k;
    vector<uberzahl> plaintext_vector;
    vector<uberzahl> ciphertext_vector;
    uberzahl ciphertext;
    uberzahl plaintext;
    uberzahl zero = "0";
    if(strcmp(argv[1],"encrypt") == 0)
    {
        key = argv[2];
        if(key.bitLength() > 256)
        {
            cout << "key bit length is more than 256 bit, please see README file." << endl;
            return 0;
        }
        plaintext = argv[3];
        //For test the result
        // k.push_back(0x1f1e1d1c1b1a1918);
        // k.push_back(0x1716151413121110);
        // k.push_back(0x0f0e0d0c0b0a0908);
        // k.push_back(0x0706050403020100);
        // key = (k[0] << (3*n)) | (k[1] << (2*n)) | (k[2] << (n)) | k[3];
        // plaintext = 0x74206e69206d6f6f;
        // plaintext = (plaintext << n) | 0x6d69732061207369;
        int num_block = 0;
        uberzahl result;
        uberzahl temp;
        while(plaintext != zero)
        {
            temp = plaintext & take_128_bit;
            plaintext = plaintext >> (2*n);
            result = encrypt(temp, key);
            ciphertext = ciphertext | (result << num_block*(2*n));
            num_block++;
        }
        cout << "The ciphertext is: " << endl;
        cout << ciphertext << endl;
    }
    else if(strcmp(argv[1],"decrypt") == 0)
    {
        key = argv[2];
        if(key.bitLength() > 256)
        {
            cout << "key bit length is more than 256 bit, please see README file." << endl;
            return 0;
        }
        ciphertext = argv[3];
        int num_block = 0;
        uberzahl result;
        uberzahl temp;
        while(ciphertext != zero)
        {
            temp = ciphertext & take_128_bit;
            ciphertext = ciphertext >> (2*n);
            result = decrypt(temp, key);
            plaintext = plaintext | (result << num_block*(2*n));
            num_block++;
        }
        cout << "The plaintext is: " << endl; 
        cout << plaintext << endl;
    }
    else
    {
        cout << "Input '" << argv[1] << "'" << " is invalid, please see README file." << endl;
        return 0;
    }

    // uberzahl c = 0x8d2b5579afc8a3a0;
    // c = (c << n) | 0x3bf72a87efe7b868;
    // cout << "ans= " << c <<endl;
    //assert(enc == ciphertext);//encryption works
    //assert(enc == plaintext);//decryption works
    return 0;
}


//////////////////////////////////////////////////////
////////////////HELPER FUNCTIONS//////////////////////
//////////////////////////////////////////////////////

uberzahl toggleNBit(uberzahl x, unsigned int size)
{
    for (int i = 0; i < size; ++i)
    {
        x.toggleBit(i);
    }
return x;
}



uberzahl f(uberzahl x)
{
    return (x.rotateLeft(1,0,n-1) & x.rotateLeft(8,0,n-1));
}

/*----------------KEY EXPANSION-----------------*/
void key_expansion( uberzahl k[])
{

     for (int i = m; i < T; ++i)
	{	
		uberzahl tmp = k[i-1].rotateRight(3, 0, n-1);
		if( m == 4)
			tmp = tmp^k[i-3];
		tmp = tmp ^ tmp.rotateRight(1, 0, n-1);
		k[i] = tmp ^ z[j][(i-m)%62] ^3;
		//Need to xor the ~k[i-m]
		k[i] = k[i] ^ toggleNBit(k[i-m], n);

		
	}
     
   
     
return;
}

/*----------------ENCRYPTION----------------------*/
uberzahl encrypt( uberzahl plaintext, uberzahl key)
{
    uberzahl cipher_text;
    //break up the plaintext into two 64 bit words
    uberzahl plain_x = (plaintext >> n) & shift;
    uberzahl plain_y = plaintext & shift;
    //break up the key into four 64 bit words 
    uberzahl k[T];
    k[0] = key & shift;
    k[1] = (key >> n) & shift;
    k[2] = (key >> (2*n)) & shift;
    k[3] = (key >> (3*n)) & shift;
    
    key_expansion(k);
 
    for (int i = 0; i < T; ++i)
    {
        uberzahl tmp = plain_x;
        plain_x = plain_y ^ k[i] ^ plain_x.rotateLeft(2,0,n-1) ^ f(plain_x);
        plain_y = tmp;
    }

    cipher_text = (plain_x << n) | plain_y;

    return cipher_text;
}

/*----------------DECRYPTION----------------------*/
uberzahl decrypt( uberzahl ciphertext, uberzahl key)
{
    
   //break up the plaintext into two 64 bit words
    uberzahl cipher_x = (ciphertext >> n) & shift;  
    uberzahl cipher_y = ciphertext & shift;
     
    //break up the key into four 64 bit words 
    uberzahl k[T];
    k[0] = key & shift;
    k[1] = (key >> n) & shift;
    k[2] = (key >> (2*n)) & shift;
    k[3] = (key >> (3*n)) & shift;
    
    key_expansion(k);
    
    for (int i = 0; i < T; ++i)
    {
        uberzahl tmp = cipher_y;
        cipher_y = cipher_x ^ k[T-i-1] ^ cipher_y.rotateLeft(2,0,n-1) ^ f(cipher_y);
        cipher_x = tmp;
    }

uberzahl plain_text = (cipher_x << n) | cipher_y;

return plain_text;
}


/*--------------RANDOM KEY GENERATOR---------------*/
uberzahl generate(){
uberzahl a = "0";
uberzahl b = "115792089237316195423570985008687907853269984665640564039457584007913129639936";
uberzahl new_key = random(a, b);


return new_key;
}






