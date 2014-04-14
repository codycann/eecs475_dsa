CryptoProj
==========  

Team: 32 (The Muffin Men)  
Name: Anthony Chiang, Eric Edmond, Steven Louie  
Uniqname: aycc, eedmond, slouie  
Implementation of SPECK for EECS 475 Final Project @ UofM  
  
Run with one of the following options  
Note that all arguments should be in base-16 format  
  
./speck_cipher keygen  
./speck_cipher encrypt keyword1 keyword0 plaintext1 plaintext0  
./speck_cipher decrypt keyword1 keyword0 ciphertext1 ciphertext0  
  
Output from each of the three options:  
keygen  -> keyword1 keyword0  
encrypt -> ciphertext1 ciphertext0  
decrypt -> plaintext1 plaintext0  
  
Example with output:   
Input   
./speck_cipher encrypt 0f0e0d0c0b0a0908 0706050403020100 6c61766975716520 7469206564616d20  
  
Output  
a65d985179783265 7860fedf5c570d18  
  
Many examples can be found in 'test.cpp'.  
Type make to compile.  
  
Contact info:  
aycc@umich.edu  
eedmond@umich.edu  
slouie@umich.edu  
