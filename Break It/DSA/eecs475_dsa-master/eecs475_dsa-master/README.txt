Kevin Krakauer, Mykola Kravchenko, Cody Cannon, Brendan Driscoll
DSA Implementation

Files included:

DSA.cpp, DSA.h:
  Contains the general DSA implementation. The sign() and verify() functions perform the corresponding DSA processes using
  a modified version of SHA256 for the hash function. The PublicKey struct contains the values p, q, g, and y; the 
  Signature struct contains the pair r,s. The uberzahl library is used as the primary data type for values. However, libgmp 
  is used to generate p, q, and g; these values are converted to the uberzahl type after their generation. Montgmery 
  arithmetic is generally used for modular arithmetic. 
  
hash.cpp, hash.h:
  A modified version of SHA256 can be found here. The hash follows the same general algorithm as SHA256 but produces
  different output for reasons we could not identify. Because of this it may be less secure than SHA256; we could not
  determine any defects in security due to this. This is the only bug that remains in our implementation, but should not
  greatly affect the algorithm.
Makefile:
  Note: there are 2 compilation options, "make" and "make debug". "Make" runs very fast compared to "make debug".
  
test.cpp:
  Contains int main() and is used for testing purposes.
  
uberzahl.cpp, uberzahl.h:
  Contains the uberzahl library written by the instructors.
