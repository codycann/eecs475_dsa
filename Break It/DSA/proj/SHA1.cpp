#include <iostream>
#include <cstdlib>
#include "uberzahl.h"
#include "SHA1.h"
#include <string>
#include <vector>
#include <cmath>

using namespace std;

//32-bit bitwise left rotate
unsigned int rotate(unsigned int rot, int index)
{
	unsigned int point = 2147483648;
	std::vector<bool> bits(32);
	for(int i = 0; i < 32; i++)
	{
		if(rot >= point)
		{
			bits[i] = 1;
			rot = rot - point;
		}	
		else
		{
			bits[i] = 0;
		}
		point = point/2;
	}
	point = 2147483648;
	unsigned int out = 0;
	for(int i = 0; i < 32; i++)
	{
		if(bits[(i + index)%32])
		{
			out = out + point;
		}
		point = point/2;

	}
	return out;
}

//SHA-1 hash function adapted from pseudocode on wikipedia
uberzahl SHA1(string message)
{
	//initialize variables
	std::vector<bool> bitVector;
	unsigned int h0 = 0x67452301;
	unsigned int h1 = 0xEFCDAB89;
	unsigned int h2 = 0x98BADCFE;
	unsigned int h3 = 0x10325476;
	unsigned int h4 = 0xC3D2E1F0;

	//parse string into bits
	for(int i = 0; i < message.length(); i++)
	{
		unsigned int temp = message[i];
		int point = 128;
		while(point >= 1)
		{	if(temp >= point)
			{
				bitVector.push_back(1);
				temp -= point;
			}
			else
			{
				bitVector.push_back(0);
			}
			point = point/2;
		}
	}

	//preprocess
	unsigned long long m1 = bitVector.size();
	uberzahl M1 = m1;
	bitVector.push_back(1);
	while(bitVector.size()%512 != 448)
	{
		bitVector.push_back(0);
	}
	uberzahl point = 9223372036854775808;
	while(point >= 1)
	{	if(M1 >= point)
		{
			bitVector.push_back(1);
			M1 = M1 - point;
		}
		else
		{
			bitVector.push_back(0);
		}
		point = point/2;
	}

	//process into 512 bit chunks
	for(int i = 0; i < bitVector.size()/512; i++)
	{
		//break each chunk into 16 32-bit words
		std::vector < std::vector < bool > > words(80, vector<bool>(32));
		for(int j = 0; j < 16; j++)
		{
			for(int k = 0; k < 32; k++)
			{
				words[j][k] = bitVector[i * 512 + j * 32 + k];
			}
		}
		//extend the 16 32-bit words into 80 32-bit words
		for(int j = 16; j < 80; j++)
		{
			for(int k = 0; k < 32; k++)
			{
				int k1 = (k - 1);
				if(k1 < 0)
				{
					k1 += 32;
				}
				words[j][k1] = words[j-3][k] ^ words[j-8][k] ^ words[j-14][k] ^ words[j-16][k];
			}
		}

		//Initialize hash values
		unsigned int a = h0;
		unsigned int b = h1;
		unsigned int c = h2;
		unsigned int d = h3;
		unsigned int e = h4;

		//Main Loop
		for(int j = 0; j < 80; j++)
		{
			unsigned int k;
			unsigned int f;
			if(j < 20)
			{
				f = (b & c) | ((~b) & d);
				k = 0x5A827999;
			}
			else if(j < 40)
			{
				f =  b ^ c ^ d;
				k = 0x6ED9EBA1;
			}
			else if(j < 60)
			{
				f =  (b & c) | (b & d) | (c & d);
				k = 0x8F1BBCDC;
			}
			
			else
			{
				f =  b ^ c ^ d;
				k = 0xCA62C1D6;
			}
			
			unsigned int temp = 0;
			unsigned int a1 = rotate(a, 5);
			unsigned int point2 = 2147483648;
			temp = a1 + f + e + k;
			// + w[j]
			for(int l = 0; l < 32; l++)
			{
				if(words[j][l])
				{
					temp = temp + point2;
				}
				point2 = point2/2;
			}
			e = d;
			d = c;
			unsigned int b1 = rotate(b,30);
			c = b1;
			b = a;
			a = temp;
		}

		//Add this chunks result to hash
		h0 = h0 + a;
		h1 = h1 + b;
		h2 = h2 + c;
		h3 = h3 + d;
		h4 = h4 + e;
	}

	//Produce the final hash value (big-endian as a 160-bit number)
	uberzahl H0 = h0;
	uberzahl H1 = h1;
	uberzahl H2 = h2;
	uberzahl H3 = h3;
	uberzahl H4 = h4;
	uberzahl hh = (H0 << 128) | (H1 << 96) | (H2 << 64) | (H3 << 32) | H4;
	return hh;

}



