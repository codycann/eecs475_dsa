#include <algorithm>
#include <getopt.h>
#include "Speck.h"

using namespace std;

uberzahl hex2dec(const char *text_arg);
//converts text_arg in hexadecimal into a decimal number

string dec2hex(uberzahl value);
//converts value in decimal into a hexadecimal string

int main(int argc, char* argv[]){
//============== FLAGS =============================
	bool mflag = 0, tflag = 0, kflag = 0, xflag = 0, hflag = 0;

	const struct option long_opts[] = {
		{"mode", required_argument, 0, 'm'},
		{"text", required_argument, 0, 't'},
		{"key", required_argument, 0, 'k'},
		{"hexadecimal", no_argument, 0, 'x'},
		{"help", no_argument, 0, 'h'},
	};

	int index;
	int arg = 0;
	char *mode_arg;
	const char *text_arg;
	const char *key_arg;
	string mode;
	uberzahl gen_key, ctext, ptext;	
	opterr = 0;


	while(arg != -1)
	{

	arg = getopt_long(argc, argv, "m:t:k:xh", long_opts, &index);
	
		switch (arg)
		{			
			case 'm':
				mflag = 1;
				mode_arg = optarg;
				break;
			case 't':
				tflag = 1;
				text_arg = optarg;
				break;
			case 'k':
				kflag = 1;
				key_arg = optarg;
				break;
		
			case 'h':
				hflag = 1;
				break;
				
			case 'x':
				xflag = 1;
				break;
			
			case '?':
				std::cerr << "Illegal command(s)" << endl;
				exit(1);
				break;
			
			default:
				break;
		}
	}
	
	if(hflag){
		cout << "Please refer to readme.txt" << endl;
		exit(0);
	}

	if(!mflag){
		cerr << "You must specify a mode" << endl;
		exit(1);
	}
	
	mode = mode_arg;
	
	if (mode != "ENCRYPT" && mode != "DECRYPT" && mode != "GENERATE") {
		cerr << "No valid mode specified" << endl;
		exit(1);
	}

	if (mode == "ENCRYPT" && (!tflag || !kflag) ){
		cerr << "You must specify a plaintext and key for ENCRYPT mode." << endl;
		exit(1);
	}
	if (mode == "DECRYPT" && (!tflag || !kflag) ){
		cerr << "You must specify a ciphertext and key for DECRYPT mode." << endl;
		exit(1);
	}
	if (mode == "GENERATE" && (tflag || kflag) ){
		cout << "Your key was generated, but this function does "; 
		cout << "not take into consideration the text and/or key you provided" << endl;
	}

//==========================================================
	if(mode == "GENERATE"){
		gen_key = keygen();
		if (xflag) cout << "key: " << dec2hex(gen_key) << endl;
		else cout << "key: " << gen_key << endl;
		return 0;
	}
	uberzahl text;
	uberzahl key;
	if (xflag) {
		text = hex2dec(text_arg);
		key = hex2dec(key_arg);
	}
	else {
		text = text_arg;
		key = key_arg;
	}
	if (mode == "ENCRYPT"){
		ctext = encrypt(key, text);
		if (xflag) cout << "ciphertext: " << dec2hex(ctext) << endl;
		else cout << "ciphertext: " << ctext << endl;
	}
	if(mode == "DECRYPT"){
		ptext = decrypt(key, text);
		if (xflag) cout << "plaintext: " << dec2hex(ptext) << endl;
		else cout << "plaintext: " << ptext << endl;
	}	
	return 0;
}

uberzahl hex2dec(const char *text_arg) {
	unsigned int index = 0;
	uberzahl result;
	while (text_arg[index]) {
		result = result << 4;
		if (text_arg[index] >= '0' && text_arg[index] <= '9') result = result + text_arg[index] - '0';
		else if (text_arg[index] >= 'a'&& text_arg[index] <= 'f') result = result + text_arg[index] - 'a' + 10;
		else {
			cerr << text_arg << " is not a valid hexadecimal number" << endl;
			exit(1);
		}
		++index;
	}
	return result;
}

string dec2hex(uberzahl value) {
	string result;
	unsigned int residue;
	const char digit[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};
	do {
		residue = value % 16;
		value = value >> 4;
		result += digit[residue];
	} while (value > "0");
	reverse(result.begin(), result.end());
	return result;
}
