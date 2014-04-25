#include <iostream>
#include <assert.h>
#include <random>
#include <time.h>
#include <limits.h>
#include <string.h>
#include <thread>
#include <iomanip>
#include <future>
#include <unistd.h>

#define ull unsigned long long

using namespace std;

struct Parameters
{
	int block_size;
	int key_size;
	int word_size;
	int key_words;
	int rot_a;
	int rot_b;
	int rounds;
};

static thread_local unsigned int srand_seed;
static thread_local bool seed_rand;

ull RandULL()
{
	//Seed rand if required (Because I am too lazy to remember to put this in main...)
	// static bool seed_rand = true;
	if( seed_rand )
	{
		seed_rand = false;
		// srand((unsigned int)time(NULL));
		srand(srand_seed);
	}
	
	//Determine the size of rand(). Can be different based on the computer (according to online sources)
	//Sometimes 15 or 31. (Since rand() is never negative)
	static int rand_size = sizeof(RAND_MAX)*CHAR_BIT - 1;
	
	//Create a random ull from rands()
	int bytes_to_fill = sizeof(ull)*CHAR_BIT;
	ull num = 0;
	while( bytes_to_fill!=0 )
	{
		ull part = rand();
		if( bytes_to_fill >= rand_size )
		{
			num = num | (part << (bytes_to_fill - rand_size));
			bytes_to_fill -= rand_size;
		}
		else
		{
			num = num | (part >> (rand_size - bytes_to_fill));
			bytes_to_fill = 0;
		}
	}

	return num;
}

ull LCS(ull x, int shift) //Left Circular Shift
{
	const static int length = sizeof(ull)*CHAR_BIT;
	return (x<<shift) | (x>>(length-shift));
}

ull RCS(ull x, int shift) //Right Circular Shift
{
	const static int length = sizeof(ull)*CHAR_BIT;
	return (x>>shift) | (x<<(length-shift));
}

void RoundFunc(ull in_x, ull in_y, ull in_key, ull& out_x, ull& out_y)
{
	//Constant for Speck128_256
	static const Parameters params = {128, 256, 64, 4, 8, 3, 34};
	
	//Function taken from SPECK pseudocode
	out_x = (RCS(in_x, params.rot_a) + in_y) ^ in_key;
	out_y = LCS(in_y, params.rot_b) ^ out_x;
}

void RoundFuncInverse(ull in_x, ull in_y, ull in_key, ull& out_x, ull& out_y)
{
	//Constant for Speck128_256
	static const Parameters params = {128, 256, 64, 4, 8, 3, 34};
	
	//Function taken from SPECK pseudocode
	out_y = RCS(in_x ^ in_y, params.rot_b);
	out_x = LCS((in_x ^ in_key) - out_y, params.rot_a);
}

ull* Generate256BitKey()
{
	//Allocate the key and fill it with random ulls
	ull* key = new ull[4];
	key[3] = RandULL();
	key[2] = RandULL();
	key[1] = RandULL();
	key[0] = RandULL();
	
	return key;
}

//Speck 256 bit key expansion
ull* KeyExpansion(ull* key)
{
	//Constant for Speck128_256
	static const Parameters params = {128, 256, 64, 4, 8, 3, 34};
	
	//Initilize the expanded_key
	ull* expanded_key = new ull[params.rounds];
	expanded_key[0] = key[0];
	
	//Allocate scratch space and initilize it to key[1]...key[params.key_words-1]. Index reaches up to 35
	int local_size = params.rounds + params.key_words - 2;
	ull* local = new ull[local_size];
	memcpy(local, &key[1], (params.key_words-1)*sizeof(ull));
	
	//Perform the round function
	for( ull idx = 0; idx<=params.rounds-2; idx++ )
		RoundFunc(local[idx], expanded_key[idx], idx, local[idx+params.key_words-1], expanded_key[idx+1]);
	
	//Free scratch space
	delete[] local;
	
	//Return the expanded key
	return expanded_key;
}

//Speck128_256
void SpeckEncrypt(ull* plaintext, ull* ciphertext, ull* key)
{
	//Constant for Speck128_256
	static const Parameters params = {128, 256, 64, 4, 8, 3, 34};
	
	//Expand the key
	ull* expanded_key = KeyExpansion(key);
	
	//Initilize ciphertext to plaintext
	ciphertext[0] = plaintext[0];
	ciphertext[1] = plaintext[1];
	
	//Perform the round function based on the number of rounds required (for our case it is 34)
	for( int round = 0; round<params.rounds; round++ )
		RoundFunc(ciphertext[1], ciphertext[0], expanded_key[round], ciphertext[1], ciphertext[0]);
	
	delete [] expanded_key;
}

//Speck128_256
void SpeckDecrypt(ull* plaintext, ull* ciphertext, ull* key)
{
	//Constant for Speck128_256
	static const Parameters params = {128, 256, 64, 4, 8, 3, 34};
	
	//Expand the key
	ull* expanded_key = KeyExpansion(key);
	
	//Initilize ciphertext to plaintext
	plaintext[0] = ciphertext[0];
	plaintext[1] = ciphertext[1];
	
	//Perform the round function based on the number of rounds required (for our case it is 34)
	for( int round = params.rounds-1; round>=0; round-- )
		RoundFuncInverse(plaintext[1], plaintext[0], expanded_key[round], plaintext[1], plaintext[0]);
	
	delete [] expanded_key;
}

void test(unsigned int start, unsigned int end, int numTabs, promise<unsigned int>&& pr) {
	cout << "Running test on interval [" << start << ", " << end << ')' << endl;
	ull pt[2];
	// pt[1] = 123;
	// pt[0] = 321;
	pt[1] = 1492;
	pt[0] = 6789;
	float pct = 0.0f;
	for(srand_seed = start; srand_seed < end; ++srand_seed) {
		seed_rand = true;
		ull* key = Generate256BitKey();
		ull ct[2];
		SpeckEncrypt(pt, ct, key);

		// if(ct[0] == 16290836081917361622ULL && ct[1] == 1644490542108440927ULL) {
		if(ct[1] == 18415601473237741767ULL && ct[0] == 2527280999305784836ULL) {
			cout << "=====================LOOK I FOUND IT=====================" << endl;
			pr.set_value(srand_seed);
			return;
		}
		float newPct = ((float)(srand_seed - start)) / (end - start);
		// cout << newPct << endl;
		if(newPct - pct > .001) {
			for(int j = 0; j < numTabs; ++j) {
				cout << '\t';
			}
			cout << newPct << endl;
			pct = newPct;
		}

		delete[] key;
	}

	pr.set_value(0);
	return;
}

// myRand = 1337
// key = 11484742554852 11484742554852 11484742554852 11484742554852
// pt 1, 0 = 123 321
// ct 1, 0 = 1644490542108440927 16290836081917361622

int main(int argc, const char * argv[])
{
	cout.precision(2);

	// srand_seed = 1398416033;
	// seed_rand = true;
	// promise<unsigned int> pr;
	// test(srand_seed, srand_seed + 1, 1, move(pr));

	vector<thread> threads (4);
	const unsigned int kChunkSize = numeric_limits<unsigned int>::max() / threads.capacity();
	vector<future<unsigned int>> futures (threads.capacity());
	for(int i = 0; i < threads.capacity(); ++i) {
		promise<unsigned int> pr;
		futures[i] = pr.get_future();
		threads[i] = thread{test, i * kChunkSize, (i + 1) * kChunkSize, i, move(pr)};
		sleep(1);
	}
	for(int i = 0; i < threads.capacity(); ++i) {
		threads[i].join();
		unsigned int randVal = futures[i].get();
		if(randVal) {
			cout << "Key was generated when myRand = " << randVal << endl;
		}
	}
	return 0;

	//Determine which operations to perform based on program inputs
	if( argc==2 && strcmp(argv[1], "test")==0 )
	{
		//For performance testing:
		cout<<"Running 1 million of each operation on random messages..."<<endl;
		ull cnt = 0;
		clock_t total_time_key_gen = 0;
		clock_t total_time_encrypt = 0;
		clock_t total_time_decrypt = 0;
		while( cnt<1000000 )
		{
			//Generate a key
			clock_t start_key_gen = clock();
			ull* key = Generate256BitKey();
			total_time_key_gen += clock() - start_key_gen;
			
			//Generate a message
			ull pt[2];
			pt[1] = RandULL();
			pt[0] = RandULL();
			ull ct[2];
			
			//Encrypt the message
			clock_t start_encrypt = clock();
			SpeckEncrypt(pt, ct, key);
			total_time_encrypt += clock() - start_encrypt;
			
			//Decrypt the message
			clock_t start_decrypt = clock();
			SpeckDecrypt(pt, ct, key);
			total_time_decrypt += clock() - start_decrypt;
			
			delete [] key;
			cnt++;
		}
		
		//Print out the data
		cout << "Key Generation: " << (double)total_time_key_gen / (double)CLOCKS_PER_SEC << " seconds" << endl;
		cout << "    Encryption: " << (double)total_time_encrypt / (double)CLOCKS_PER_SEC << " seconds" << endl;
		cout << "    Decryption: " << (double)total_time_decrypt / (double)CLOCKS_PER_SEC << " seconds" << endl;
	}
	else if( argc==3 && strcmp(argv[1], "keygen")==0 )
	{
		//Key generation
		ull* key = Generate256BitKey();
		
		//Determine if the input & output should be in hex or decimal
		int base = (strcmp(argv[2], "hex")==0) ? 16 : (strcmp(argv[2], "dec")==0) ? 10 : 0;
		if( base==0 )
		{
			cout<<"Error: Invalid base"<<endl;
			return 1;
		}
		
		//Output in the base specified
		if( base==16 )
			printf("%llx %llx %llx %llx\n", key[3], key[2], key[1], key[0]);
		else if( base==10 )
			printf("%llu %llu %llu %llu\n", key[3], key[2], key[1], key[0]);
	}
	else if( argc==9 && strcmp(argv[1], "encrypt")==0 )
	{
		//Encryption
		ull key[4];
		ull plaintext[2];
		
		//Determine if the output should be in hex or decimal
		int base = (strcmp(argv[2], "hex")==0) ? 16 : (strcmp(argv[2], "dec")==0) ? 10 : 0;
		if( base==0 )
		{
			cout<<"Error: Invalid base"<<endl;
			return 1;
		}
		
		//Read the input arguments in the base specified
		for( int idx = 0; idx<4; idx++ )
			key[3-idx] = strtoull(argv[idx+3], nullptr, base);
		
		for( int idx = 0; idx<2; idx++ )
			plaintext[1-idx] = strtoull(argv[idx+7], nullptr, base);
		
		//Perform the encryption
		ull ciphertext[2];
		SpeckEncrypt(plaintext, ciphertext, key);
		
		//Output in the base specified
		if( base==16 )
			printf("%llx %llx\n", ciphertext[1], ciphertext[0]);
		else if( base==10 )
			printf("%llu %llu\n", ciphertext[1], ciphertext[0]);
	}
	else if( argc==9 && strcmp(argv[1], "decrypt")==0 )
	{
		//Decryption
		ull key[4];
		ull ciphertext[2];
		
		//Determine if the input & output should be in hex or decimal
		int base = (strcmp(argv[2], "hex")==0) ? 16 : (strcmp(argv[2], "dec")==0) ? 10 : 0;
		if( base==0 )
		{
			cout<<"Error: Invalid base"<<endl;
			return 1;
		}
		
		//Read the input arguments in the base specified
		for( int idx = 0; idx<4; idx++ )
			key[3-idx] = strtoull(argv[idx+3], nullptr, base);
		
		for( int idx = 0; idx<2; idx++ )
			ciphertext[1-idx] = strtoull(argv[idx+7], nullptr, base);
		
		//Perform the decryption
		ull plaintext[2];
		SpeckDecrypt(plaintext, ciphertext, key);
		
		//Output in the base specified
		if( base==16 )
			printf("%llx %llx\n", plaintext[1], plaintext[0]);
		else if( base==10 )
			printf("%llu %llu\n", plaintext[1], plaintext[0]);	}
	else
	{
		//Command line help
		cout<<"-------------Valid command line arguments-------------"<<endl;
		cout<<"   Key Gen: ./proj keygen (hex|dec)"<<endl;
		cout<<"Encryption: ./proj encrypt (hex|dec) k3 k2 k1 k0 p1 p0"<<endl;
		cout<<"Decryption: ./proj decrypt (hex|dec) k3 k2 k1 k0 c1 c0"<<endl;
	}
	
    return 0;
}
