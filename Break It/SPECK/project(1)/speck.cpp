#include <iostream>
#include <string>
#include <cstdlib>
#include <cmath>
#include "uberzahl.cpp"

using namespace std;

static long long m = 3;//Number of key words required.
static long long n = 64;//Word size (bits).
static long long T = 33;//Number of iterations in encryption/decryption.
static long long alpha = 8, beta = 3;//Parameters used for rotational bit-shifts.
static uberzahl two = 2;//Garbage variable used to calculate mod.
static uberzahl mod = two.exp(n);//Modulus used for arithmetic in crypto.
static uberzahl randMin = two.exp(63), randMax = two.exp(64);//Used for random().
/*Global parameters above. These never change for the 128/192 
  version of SPECK.*/

void expand(uberzahl l[], uberzahl k[])
{
	for(long long i = 0; i < (T - 1); i = i + 1)
	{
		l[i+m-1] = ((k[i] + l[i].rotateRight(alpha, 0, 63)) % mod) ^ i;
		k[i + 1] = k[i].rotateLeft(beta, 0, 63) ^ l[i+m-1];
	}
	return;
}/*Performs the key expansion. Modifies both l and k. l[1], l[0], and k[0]
	should already be initialized before calling this function.*/

void encrypt(uberzahl &x, uberzahl &y, uberzahl k[])
{
	for(long long i = 0; i < T; i = i + 1)
	{
		x = ((x.rotateRight(alpha, 0, 63) + y) % mod) ^ k[i];
		y = y.rotateLeft(beta, 0, 63) ^ x;
	}	
	return;
}/*Encryption function that modifies x and y to become their corresponding 
	ciphertexts. Requires that key expansion has already been performed and
	x and y are plaintexts.*/

void decrypt(uberzahl &x, uberzahl &y, uberzahl k[])
{
	for(long long i = T - 1; i >= 0; i = i - 1)
	{
		y = (y ^ x).rotateRight(beta, 0, 63);
		x = (((x ^ k[i]) + (mod - y)) % mod).rotateLeft(alpha, 0, 63);
	}
		return;
}/*Decryption function that modifies x and y to become their corresponding 
	plaintexts. Requires that key expansion has already been performed and
	x and y are ciphertexts.*/

char getArgs (char* argv[], int argc, uberzahl &x, uberzahl &y, uberzahl &k1, uberzahl &k2, uberzahl &k3) 
{
    if (argc != 2 && argc != 6) 
    {	
    	return -1;
    }//Checks to make sure the argument count is valid.
    
    if (string(argv[1]) == "-e" || string(argv[1]) == "-d") 
    {   
        uberzahl temp = argv[2];
        x = temp >> 64;//First half of plaintext (bitwise)
        y = temp & 0xFFFFFFFFFFFFFFFF;//Second half of plaintext (bitwise)
        k1 = argv[3];
        k2 = argv[4];
        k3 = argv[5];
        return argv[1][1];
    }/*Assigns arguments to their proper variable to perform an encryption
    	or decryption. Returns either 'e' or 'd' for "encryption or "decryption".
    	Plaintext/ciphertext is split into two halves, x and y.*/
    
    else if(string(argv[1]) == "-k")
    {
     		uberzahl rand = random(randMin, randMax);
     		cout << "key_part1: " << rand << endl;
     		rand = random(randMin, randMax);
    		cout << "key_part2: " << rand << endl;
     		rand = random(randMin, randMax);
    		cout << "key_part3: " << rand << endl;
    		return 1;
    }//Generates 3 "random" 64-bit keys to be output for the user.
      
    else
    {
    	 	return -2;
	 }//Invalid flag was used.
	 
}/*This function takes the command line arguments and performs either a key
	generation algorithm, or sets all variables to the proper value if an 
	encryption or decryption is requested.*/
    
int main(int argc, char** argv)
{
	srand(time(0));
	uberzahl x, y, k1, k2, k3;//First half of text, second half of text, and three keys, respectively.
	uberzahl l[T+1], k[T];//Arrays to hold the original 3 keys and the expansion.
	uberzahl encryption;
	uberzahl decryption;
	
	char type = getArgs(argv, argc, x, y, k1, k2, k3);
	
	l[1] = k1;
	l[0] = k2;
	k[0] = k3;

	if (type == -1) 
   {
      cout << "Wrong number of arguments" << endl;
      return 1;
   }//Nonzero return value if argument count was invalid.
   
   else if (type == -2)
   {
      cout << "Use only -d or -e flags" << endl;
      return 1;
   }//Nonzero return value if invalid flag is used.
   
   else if (type == 'd') 
   {
      expand(l,k);
	   decrypt(x,y,k);
	   decryption = (x << 64) + y;
	   cout << decryption << "\n";
   }/*Performs the required key expansion and then decrypts the entered ciphertext.
      Prints the recoved plaintext to the terminal.*/
   else if (type == 'e') 
   {
      expand(l,k);
      encrypt(x,y,k);
      encryption = (x << 64) + y;
	   cout << encryption << "\n";
   }/*Performs the required key expansion and then encrypts the entered plaintext.
   	Prints the new ciphertext to the terminal.*/
	
	return 0;
}
