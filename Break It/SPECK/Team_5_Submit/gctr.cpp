#include <iostream>
#include <cstdlib>
#include <math.h>
#include <openssl/aes.h>
#include "uberzahl.h"
#include <string.h>
#include <vector>
#include <string.h>
#include <sstream>
#include "increment.h"
#include <iomanip>

using namespace std;

/*The implementation of counter mode in the Intel AES-NI sample library
 * implements a Big-Endian 32-bit increment. That is, the most
 * significant 32 bits of the counter are incremented by 1
 * (when viewed as a big-endian integer), and the remaining 96 bits
 *  are unchanged.
 * This is the definition required by GCM (Galois Counter Mode).*/

uberzahl betterExtract(uberzahl d, int start, int end) {
	uberzahl ret = d.extract(start, end) >> start;
	return ret;
}

string uber2hex(uberzahl d) {
	string dhex="";
	for(unsigned int i = 0; i < d.bitLength() ; i += 4){
		uberzahl temp = betterExtract(d, i, i + 3);
		if(temp == 1){dhex="1"+dhex;}
		else if(temp == 2){dhex="2"+dhex;}
		else if(temp == 3){dhex="3"+dhex;}
		else if(temp == 4){dhex="4"+dhex;}
		else if(temp == 5){dhex="5"+dhex;}
		else if(temp == 6){dhex="6"+dhex;}
		else if(temp == 7){dhex="7"+dhex;}
		else if(temp == 8){dhex="8"+dhex;}
		else if(temp == 9){dhex="9"+dhex;}
		else if(temp == 10){dhex="a"+dhex;}
		else if(temp == 11){dhex="b"+dhex;}
		else if(temp == 12){dhex="c"+dhex;}
		else if(temp == 13){dhex="d"+dhex;}
		else if(temp == 14){dhex="e"+dhex;}
		else if(temp == 15){dhex="f"+dhex;}
		else{dhex="0"+dhex;}
		if(d.bitLength() <= 4) {
			return dhex;
		}
	}
	return dhex;
}

uberzahl hex2uber(string hexd){
	//note 6 hex pair (6 byte padding)
	ostringstream digit;
	string d=hexd;
	 //change to length-12 if you want to remove padding
	 uberzahl temp;
	 for(unsigned int i=0;i<d.length();i++){
		 if(d[i]=='1'){temp=temp+1;}
		 else if(d[i]=='2'){temp=temp+2;}
		 else if(d[i]=='3'){temp=temp+3;}
		 else if(d[i]=='4'){temp=temp+4;}
		 else if(d[i]=='5'){temp=temp+5;}
		 else if(d[i]=='6'){temp=temp+6;}
		 else if(d[i]=='7'){temp=temp+7;}
		 else if(d[i]=='8'){temp=temp+8;}
		 else if(d[i]=='9'){temp=temp+9;}
		 else if(d[i]=='a'){temp=temp+10;}
		 else if(d[i]=='b'){temp=temp+11;}
		 else if(d[i]=='c'){temp=temp+12;}
		 else if(d[i]=='d'){temp=temp+13;}
		 else if(d[i]=='e'){temp=temp+14;}
		 else if(d[i]=='f'){temp=temp+15;}
			if(!(i==d.length()-1)){
			temp=temp<<4;
		}
	 }

	return temp;
}

uberzahl ciph_k(uberzahl u_key, uberzahl u_text){

	ostringstream temp,final;
	//convert key to properformat

	string keystr=uber2hex(u_key);
	unsigned int x=0x0;
	char key[16];
	int count=0;
	for(unsigned int i=0;i<keystr.length();i+=2){

		stringstream ss;
		ss << std::hex <<keystr.substr(i,2);

		ss >> x;
		key[count]=0x0+x;

		count++;
	}

	//convert text to uberzahl
	string texts = uber2hex(u_text);
	x=0x0;
	char text[512];
	for(int i = 0; i < 512; ++i) {
		text[i] = 0;
	}
	count=0;
	for(unsigned int i=0;i<texts.length();i+=2){

		stringstream ss;
		ss << std::hex <<texts.substr(i,2);

		ss >> x;
		text[count]=0x0+x;

		count++;
	}

	unsigned char enc_out[1024];
	for(int i = 0; i < 1024; ++i) {
		enc_out[i] = 0;
	}
	AES_KEY enc_key;
    AES_set_encrypt_key((unsigned char *)key, 128, &enc_key);
    AES_encrypt((unsigned char *)text, enc_out, &enc_key);
	ostringstream test;
	for(int i=0;*(enc_out+i)!=0x00;i++){
		test<< setfill('0') << setw(2) << hex<<int(*(enc_out+i));
	}

	string stuff(test.str());
	uberzahl cipher=hex2uber(stuff);

	return cipher;
}

string u2str(uberzahl in){
	ostringstream o;
	o<<in;

	return o.str();
}


//pass IV as ICB
uberzahl gctr(uberzahl key, uberzahl x, uberzahl ICB, int bit_length_diff)
{
	uberzahl temp;
	if(x=="0"){return temp;}

	int chunk_size = 128;
	int n = ceil(x.bitLength()/128.0);
	int low_index = x.bitLength() - chunk_size + bit_length_diff;
	int high_index = x.bitLength() - 1;

	vector <uberzahl> Y(n);
	vector <uberzahl> CB(n);
	CB[0] = ICB;
	for (int i = 1; i < n; i++){
		CB[i]=increment(CB[i-1]);
	}
	bool bit_length_diff_add = true;
	for (int i=0;i<n-1;i++){
		temp=betterExtract(x, low_index, high_index);
		//check length here

		Y[i]=temp^ciph_k(key,CB[i]);

		low_index -= chunk_size;
		high_index -= chunk_size;
		if (bit_length_diff_add)
		{
			high_index = high_index + bit_length_diff;
			bit_length_diff_add = false;
		}

	}
	low_index=0;
	//check length here
	uberzahl msb_ciph= ciph_k(key,CB[n-1]);
	uberzahl op1 = betterExtract(x,low_index,high_index);
	uberzahl op2 = betterExtract(msb_ciph, msb_ciph.bitLength() - high_index - 1, msb_ciph.bitLength() - 1);
	Y[n-1]= op1 ^ op2;
	uberzahl ctext;
	for(int i=0;i<n;i++){
		ctext = ctext + Y[i];
		if(i != n-1) {
			ctext = ctext << chunk_size;
		}
	}
	return ctext;
}
