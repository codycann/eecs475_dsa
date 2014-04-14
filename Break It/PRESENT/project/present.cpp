/*
 * Implementation of PRESENT cipher 
 * Final project for EECS 475 WN 2014 at the University of Michigan, Ann Arbor
 * Team 18: League of Asians
 * Frank Tan, fstan
 * Joonjae Bang, joonjae
 * Thomas Seah, zhiqian
 *
 * Test vectors
 *    plaintext:              key:                                        ciphertext:
 *    0                       0                                           10870405452058394799
 *    0                       340282366920938463463374607431768211455     1379100327905764824
 *    18446744073709551615    0                                           4350505715235673443      
 *    18446744073709551615    340282366920938463463374607431768211455     7101507822639703476
 *
 * References: 
 *    http://homes.esat.kuleuven.be/~abogdano/papers/present_ches07.pdf
 *    http://www.lightweightcrypto.org/implementations.php
 */
#include <cassert>
#include <getopt.h>
#include <iostream>
#include <stdlib.h>
#include <time.h>

#include "uberzahl.h"

using namespace std;

// Assumes the plaintext/ciphertext is a 64-bit block and key as a 128-bit number
uberzahl encrypt(uberzahl plaintext, uberzahl key);
uberzahl decrypt(uberzahl ciphertext, uberzahl key);

// Generate a 128-bit key
uberzahl generate_key(void);

//Convert uberzahl to int
int uber2int(uberzahl input);

//switch the bits of original with input at position start till end
uberzahl switch_bits(uberzahl original, uberzahl input, int start, int end);
uberzahl encrypt(uberzahl key, uberzahl plaintext);
uberzahl decrypt(uberzahl key, uberzahl ciphertext);

int main(int argc, char *argv[]) {
    /*
     * Input Handling
     */
    bool initialized = false;
    bool encrypting = false;
    bool decrypting = false;
    uberzahl key;
    uberzahl plaintext;
    uberzahl ciphertext;

    struct option long_opts[] = {
        {"keygen", no_argument, NULL, 'g'},
        {"key", required_argument, NULL, 'k'},
        {"encrypt", required_argument, NULL, 'e'},
        {"decrypt", required_argument, NULL, 'd'},
        {"help", no_argument, NULL, 'h'}
    };

    int opt;
    int index;
    while((opt=getopt_long(argc, argv, "gk:e:d:h", long_opts, &index)) != -1) {
        switch(opt) {
            case 'g':
	         {
                // Generate key here
                uberzahl k = generate_key();
					 cout << k << endl;
                return 0;
	         }
            case 'k':
                key = uberzahl(optarg);
                initialized = true;
                break;
            case 'e':
                plaintext = uberzahl(optarg);
                encrypting = true;
                break;
            case 'd':
                ciphertext = uberzahl(optarg);
                decrypting = true;
                break;
            case 'h':
                cout << "This is Team 18's implementation of the PRESENT "
                     << "block cipher\nfor EECS 475 Winter 2014 at the "
                     << "University of Michigan, Ann Arbor\n"
                     << "Run with --keygen or -g to output a 128-bit key.\n"
                     << "Run with --key <key> or -k <key> to set a key to be "
                     << "used\nfor encryption and decryption.\n"
                     << "Run with --encrypt <plaintext> or -e <plaintext> to "
                     << "encrypt.\nMust also run with -k to set a key.\n"
                     << "Run with --decrypt <ciphertext> or -d <ciphertext> to "
                     << "decrypt.\nMust also run with -k to set a key."
                     << endl;
                return 0;
            default:
                cout << "ERROR: You used an invalid flag.\n"
                     << "Please run with --help or -h for more information." 
                     << endl;
                exit(1);
        }
    }

    if (!initialized) {
        cout << "ERROR: Please also run with --key to set a key.\n"
             << "Run with --help for more information."
             << endl;
        exit(1);
    }

    if (encrypting && decrypting) {
        cout << "ERROR: You can ony encrypt or decrypt, not both.\n"
             << "Run with --help for more information."
             << endl;
        exit(1);
    }

    /*
     * Encryption and decryption
     */
    if (encrypting) {
        uberzahl ciphertext = encrypt(key, plaintext);
        cout << ciphertext << endl;
    }
    else if (decrypting) {
        uberzahl plaintext = decrypt(key, ciphertext);
        cout << plaintext << endl;
    }
    else {
        assert(0);
    }

	return 0;
}

//Convert uberzahl to int. Only works for small enough inputs
int uber2int(uberzahl input)
{
    int output = 0;
    for (int x = 0; x< input.bitLength(); x++)
    {
        if (input.bit(x) == 1)
            output = output + (1<<x);
    }

    return output;
}

//swap the bits value of original with input at position start to end
uberzahl switch_bits(uberzahl original, uberzahl input, int start, int end)
{
    //extract bits in the range
    uberzahl range = original.extract(start, end);

    //clear the bits within the range in the original
    original = original - range;

    //replace with the value of "input"
    original = original + (input << start);
    return original;
}

uberzahl encrypt(uberzahl key, uberzahl plaintext){
	  //Declaration of our S-Boxes and P-Boxes
    uberzahl S_BOX[16] = {
        12, 5, 6, 11, 9, (long long unsigned) 0, 10, 13, 
        3, 14, 15, 8, 4, 7, 1, 2
    };

    long long unsigned pBox[64] = {
        0, 16, 32, 48, 1, 17, 33, 49, 2, 18, 34, 50, 3, 19, 35, 51,
        4, 20, 36, 52, 5, 21, 37, 53, 6, 22, 38, 54, 7, 23, 39, 55,
        8, 24, 40, 56, 9, 25, 41, 57, 10, 26, 42, 58, 11, 27, 43, 59,
        12, 28, 44, 60, 13, 29, 45, 61, 14, 30, 46, 62, 15, 31, 47, 63
    };  
    uberzahl keySchedule[32];
    
    uberzahl temp;
    int temp2;
    
    //Generating subkeys for each round
    for( int x = 0; x< 32; x++){     
        //save current key into key schedule
        keySchedule[x] = key>>64;

        //Step 1: rotate the key 61 bits to the left
        key = key.rotateLeft(61, 0, 127);

        //Step 2: S-box the first 4 bits
        temp = key.extract(124,127)>>124; 
        temp2 = uber2int(temp);
        temp = S_BOX[temp2];
        key = switch_bits(key, temp, 124,127);

        //Step 3: S-box the second 4 bits
        temp = key.extract(120,123)>>120; 
        temp2 = uber2int(temp);
        temp = S_BOX[temp2];
        key = switch_bits(key, temp, 120,123);

        //Step 4: XOR bit 62 to 66 with the round counter
        temp = key.extract(62,66)>>62; 
        temp2 = uber2int(temp);
        temp2 = temp2 ^ (x+1);
        temp = temp2;
        temp = temp;
        key = switch_bits(key, temp, 62,66);

        // cerr << "key after round "<< x <<" is " << keySchedule[x] << endl; 
        // cerr << key<<endl;
    }
    
    // cerr << "Encrypting plaintext " << plaintext 
             // << " using key " << key << endl;
	 

    //Running Encryption Rounds
    for (int x =0; x< 31; x++)
    {
        // cerr<<"------------------Round "<<x<<"----------------\n";

        //Step 1: XOR the plaintext with the subkey        
        plaintext = plaintext ^ keySchedule[x];

        // cerr <<"After XOR:" << plaintext<<endl;

		 
	//Run S-Box Layer
        for(int y = 0; y< 16; y++)
        {
            //extract each 4-bit block of the plaintext
            temp = plaintext.extract(y*4, y*4+3) >> (y*4);

            //convert it to an int
            temp2 = uber2int(temp);

            //run the value in the S-box
            temp = S_BOX[temp2];

            //switch the new value in the plaintext
            plaintext = switch_bits(plaintext, temp, y*4, y*4+3);
        }

        // cerr <<"After SBOX:" << plaintext<<endl;

        //create a copy of plaintext for permutation purposes
        uberzahl statecopy = plaintext;
      
        //P-Box Layer
        for (int x =0; x< 64; x++)
        {
            //permutate the plaintext based on the p-box
            if (statecopy.bit(x) == 1)
            {
                plaintext.setBit(pBox[x]);
            }
            else
            {
         	      plaintext.clearBit(pBox[x]);
            }
        }
		// cerr <<"After p-layer:" << plaintext<<endl;
  	}

        //XOR with the last subkey
  	plaintext = plaintext ^ keySchedule[31];

        //plaintext is now encrypted to ciphertext
  	uberzahl ciphertext = plaintext;
  	// cerr<< "ciphertext is "<< ciphertext<<endl;
  	return ciphertext;
}


uberzahl decrypt(uberzahl key, uberzahl ciphertext){
	  
    //Declaration of our S-Boxes and P-Boxes
    uberzahl S_BOX[16] = {
        12, 5, 6, 11, 9, (long long unsigned) 0, 10, 13, 
        3, 14, 15, 8, 4, 7, 1, 2
    };

    //the inverse of the S-Box to be used for decryption
    uberzahl S_BOXINV[16] = {
        5, 14, 15, 8, 12, 1, 2, 13, 
        11, 4, 6, 3, (long long unsigned) 0, 7, 9, 10
    };
    long long unsigned pBox[64] = {
        0, 16, 32, 48, 1, 17, 33, 49, 2, 18, 34, 50, 3, 19, 35, 51,
        4, 20, 36, 52, 5, 21, 37, 53, 6, 22, 38, 54, 7, 23, 39, 55,
        8, 24, 40, 56, 9, 25, 41, 57, 10, 26, 42, 58, 11, 27, 43, 59,
        12, 28, 44, 60, 13, 29, 45, 61, 14, 30, 46, 62, 15, 31, 47, 63
    };  
    uberzahl keySchedule[32];
    
    //temporary variables used for bit manipulation
    uberzahl temp;
    int temp2;

    
    //Generating subkeys for each round
    for( int x = 0; x< 32; x++){     
        //save current key into key schedule
        keySchedule[x] = key>>64;

        //Step 1: rotate the key 61 bits to the left
        key = key.rotateLeft(61, 0, 127);

        //Step 2: S-box the first 4 bits
        temp = key.extract(124,127)>>124; 
        temp2 = uber2int(temp);
        temp = S_BOX[temp2];
        key = switch_bits(key, temp, 124,127);

        //Step 3: S-box the second 4 bits
        temp = key.extract(120,123)>>120; 
        temp2 = uber2int(temp);
        temp = S_BOX[temp2];
        key = switch_bits(key, temp, 120,123);

        //Step 4: XOR bit 62 to 66 with the round counter
        temp = key.extract(62,66)>>62; 
        temp2 = uber2int(temp);
        temp2 = temp2 ^ (x+1);
        temp = temp2;
        temp = temp;
        key = switch_bits(key, temp, 62,66);

        // cerr << "key after round "<< x <<" is " << keySchedule[x] << endl; 
        // cerr << key<<endl;
    }
    
    // cerr << "Decrypting ciphertext " << ciphertext 
         // << " using key " << key << endl;


    uberzahl plaintext = ciphertext;
	 

    //Running Decryption Rounds
    for (int x =0; x< 31; x++)
    {
        // cerr<<"------------------Round "<<x<<"----------------\n";

        //feed the keyschedule in reverse order
        plaintext = plaintext ^ keySchedule[31-x];   
        // cerr <<"After XOR:" << plaintext<<endl;

        //create a copy for permutation purposes
        uberzahl statecopy = plaintext;
      
        //P-Box Layer
        for (int x =0; x< 64; x++)
        {
            //permutate the bit based on P-box
            if (statecopy.bit(pBox[x]) == 1)
            {
                plaintext.setBit(x);
            }
            else
            {
            	plaintext.clearBit(x);
            }
        }
        // cerr <<"After p-layer:" << plaintext<<endl;
  		
        //S-Box Layer
        for(int y = 0; y< 16; y++)
        {
            //extract each 4-bit block
            temp = plaintext.extract(y*4, y*4+3) >> (y*4);

            //convert to int
            temp2 = uber2int(temp);

            //get the S-box value
            temp = S_BOXINV[temp2];

            //replace the bits in the plaintext
            plaintext = switch_bits(plaintext, temp, y*4, y*4+3);     
        }

         // cerr <<"After SBOX:" << plaintext<<endl;
    }

    //XOR the plaintext with the last subkey
    plaintext = plaintext ^ keySchedule[0];

    // cerr<< "plaintext is: "<< plaintext<<endl;
    return plaintext;
}

uberzahl generate_key(void) {

    //seed the random number generator
    srand(time(NULL));

    //set the upper bound as the max of a 128-bit number
    uberzahl two = 2;
    uberzahl upperbound = two.exp(128) - 1;

    //generate a random number between the range as the key
    uberzahl key = random((long long unsigned) 0, upperbound);
    return key;
}
