//  Team: 32 (The Muffin Men)
//  Name: Anthony Chiang, Eric Edmond, Steven Louie
//  Uniqname: aycc, eedmond, slouie

#include "speck.hpp"
#include "utility.hpp"

int main(int argc, char *argv[])
{

    /*
    	Run with one of the following options
    	Note that all arguments should be in base-16 format

    		./speck_cipher keygen
    		./speck_cipher encrypt keyword1 keyword0 plaintext1 plaintext0
    		./speck_cipher decrypt keyword1 keyword0 ciphertext1 ciphertext0

    	Output from each of the three options:
    	keygen  -> keyword1 keyword0
    	encrypt -> ciphertext1 ciphertext0
    	decrypt -> plaintext1 plaintext0

    */

    return run_speck(argc,argv);
}