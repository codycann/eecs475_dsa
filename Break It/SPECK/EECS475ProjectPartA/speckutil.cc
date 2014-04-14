//see speckutil.readme for information on what this program does
#include "speck.h"
#include <getopt.h>
#include <fstream>
#include <iomanip>
#include <openssl/sha.h>
#include <chrono>

using namespace std;

int main(int argc, char** argv){
	//getopt cmd line parsing
	struct option longOpts[] = {
		{"password", required_argument, NULL, 'p'},
		{"key", required_argument, NULL, 'k'},
		{"encrypt", required_argument, NULL, 'e'},
		{"decrypt", required_argument, NULL, 'd'},
		{"generate", no_argument, NULL, 'g'},
		{"output", required_argument, NULL, 'o'},
		{"time", no_argument, NULL, 't'},
		{"help", required_argument, NULL, 'h'}
	};

	if(argc	== 1){
		cout << "No flags specified. Please check speckutil.readme for info on how to use this program." << endl;
		exit(0);
	}

	string password, keyInputFile, inputFile, outputFile, keyOutputFile;
	bool passwordFlag, keyFlag, encFlag, decFlag, genFlag, outFlag, timeFlag;
	passwordFlag = keyFlag = encFlag = decFlag = genFlag = outFlag  = timeFlag = false;


	int opt = 0, index = 0;
	while((opt = getopt_long (argc, argv, "p:k:e:d:go:t", longOpts, &index)) != -1){
		switch(opt) {
			case 'h':
				cout << "Please check speckutil.readme for info on how to use this program." << endl;
				exit(0);
				break;
			case 'p':
				password = optarg;
				passwordFlag = true;
				break;
			case 'k':
				keyInputFile = optarg;
				keyFlag = true;
				break;
			case 'e':
				inputFile = optarg;
				encFlag = true;
				break;
			case 'd':
				inputFile = optarg;
				decFlag = true;
				break;
			case 'g':
				genFlag = true;
				break;
			case 'o':
				outputFile = optarg;
				outFlag = true;
				break;
			case 't':
				timeFlag = true;
				break;
			case '?':
				cout << "Unrecognized flag. Check speckutil.readme for info on how to use the program." << endl;
				exit(1);
				break;
		}
	}

	if (encFlag && decFlag){
		cout << "Both encryption and decryption were specified, quitting." << endl;
		return 1;
	}

	if ((encFlag || decFlag) && genFlag){
		cout << "Both encryption/decryption and key generation were specified, quitting." << endl;
		return 1;
	}

	if (keyFlag && passwordFlag){
		cout << "Both a password and a key file were specified, quitting." << endl;
		return 1;
	}

	if (!encFlag && !decFlag && !genFlag){
		cout << "No mode of operation was specified, quitting." << endl;
		return 1;
	}

	
	//Variables
	uint64_t key[2];
	ofstream keyOutput;
	ofstream output;
	ifstream input;
	ifstream keyInput;
	string buffer;
	int rc;
	
	if (genFlag) {
		//Key Generation
		if (!outFlag){
			cout << "No output file was specified for key generation, default output file (key.bin) will be used." << endl;
			keyOutputFile = "key.bin";
		} else {
			keyOutputFile = outputFile;
			cout << "Saving generated key to " << keyOutputFile << endl;
		}

		//Generate key
		rc = speck_generate_key(key);
		if (rc < 0) {
			cout << "Error in generating key, exiting." << endl;
			return 1;
		}

		//Transform into string
		buffer = transform_uint64_array(key, false, 2);

		//Print generated key
		cout << "String buffer representation of key is : 0x";
		for(unsigned char i : buffer){
			cout << hex << setw(2) << setfill('0') << (unsigned int) i;
		}
		cout << endl;

		//Write to file
		keyOutput.open(keyOutputFile, ios_base::binary);
		if(!keyOutput.is_open()) {
			cout << "Failed to open output file, exiting.\n";
			return 1;
		}
		keyOutput.write(buffer.c_str(), buffer.size());

		cout << "Successfully generated key and saved to " << keyOutputFile << endl;

	} else {
		//Encryption/Decryption

		if (!passwordFlag && !keyFlag){
			cout << "No key specified. Saving generated key to key.bin" << endl;
			keyOutputFile = "key.bin";

			//Generate key
			rc = speck_generate_key(key);
			if (rc < 0) {
				cout << "Error in generating key, exiting." << endl;
				return 1;
			}

			//Transform into string
			buffer = transform_uint64_array(key, false, 2);

			//Write to file
			keyOutput.open(keyOutputFile, ios_base::binary);
			if(!keyOutput.is_open()) {
				cout << "Failed to open output file, exiting.\n";
				return 1;
			}
			keyOutput.write(buffer.c_str(), buffer.size());

			cout << "Successfully generated key and saved to " << keyOutputFile << endl;

		} else if (passwordFlag) {
			//use SHA256 to transform password into 128 bit key
			cout << "Transforming password to 128 bit key" << endl;
			unsigned char hash[SHA256_DIGEST_LENGTH];
		    SHA256_CTX sha256;
		    SHA256_Init(&sha256);
		    SHA256_Update(&sha256, password.c_str(), password.size());
		    SHA256_Final(hash, &sha256);

		    //Add hash to buffer
		   	buffer.clear();
		    buffer.append((char*) hash, 16);

			cout << "String buffer representation of key is : 0x";
			for(unsigned char i : buffer){
				cout << hex << setw(2) << setfill('0') << (unsigned int) i;
			}
			cout << endl;

			size_t tempSize;
			uint64_t* temp = transform_string(buffer, false, tempSize);
			key[0] = temp[0];
			key[1] = temp[1];
			delete[] temp;
		} else {
			//Load key from specified key file
			keyInput.open(keyInputFile, ios_base::binary);
			if(!keyInput.is_open()) {
				cout << "Failed to open key file, exiting.\n";
				return 1;
			}
			istreambuf_iterator<char> begin(keyInput), end;
			buffer.append(begin, end);

			//Transfer key into key array
			size_t tempSize;
			uint64_t* temp = transform_string(buffer, false, tempSize);
			key[0] = temp[0];
			key[1] = temp[1];
			delete[] temp;
		}

		if (!outFlag){
			cout << "No output file was specified for output, default output file (output.bin) will be used." << endl;
			outputFile = "output.bin";
		} else {
			cout << "Saving output to " << outputFile << endl;
		}

		//Open input filestream
		input.open(inputFile, ios_base::binary);

		if (!input.is_open()){
			cout << "Failed to open file, exiting.\n";
			return 1;
		}

		//Save to input string
		buffer.clear();
		istreambuf_iterator<char> begin(input), end;
		buffer.append(begin, end);

		//Transform into uint64_t array
		uint64_t* tempBuffer;
		size_t tempBufferSize;
		tempBuffer = transform_string(buffer, encFlag, tempBufferSize);

		if (tempBuffer == nullptr) {
			cout << "Failed to transform string." << endl;
			return 1;
		}

		uint64_t* stream;
		size_t streamSize;

		auto current = chrono::steady_clock::now();
		auto previous = current;

		//If timing requested, get timing data
		if(timeFlag){
			cout << "Timing enabled. Timing stream operation..." << endl;
			current = chrono::steady_clock::now();
		}

		//Encrypt stream
		if(encFlag) {
			stream = speck_encrypt_ecb(key, tempBuffer, false, tempBufferSize, streamSize);
		} else {
			//Decrypt stream
			stream = speck_decrypt_ecb(key, tempBuffer, false, tempBufferSize, streamSize);
		}
		
		//Timing data
		previous = current;
		current = chrono::steady_clock::now();

		delete[] tempBuffer;

		if (stream == nullptr){
			cout << "Failed to encrypt/decrypt." << endl;
			return 1;
		}

		//Print timing data
		if(timeFlag){
			auto elapsed = chrono::duration_cast<chrono::duration<double>>(current - previous);
			cout << "Time taken for stream operation: " << endl;
			cout << elapsed.count() << " seconds" << endl;
		}

		//Open output filestream
		output.open(outputFile, ios_base::binary);
		if (!output.is_open()){
			cout << "Failed open output file, exiting." << endl;
			return 1;
		}

		//Transform output into string
		buffer = transform_uint64_array(stream, !encFlag, streamSize);

		//Save output to file
		output.write(buffer.c_str(), buffer.size());

		cout << "Operation completed." << endl;
		delete[] stream;
	}


	return 0;
}
