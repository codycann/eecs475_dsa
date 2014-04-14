#include <iostream>
#include <cstdlib>
#include <string>
#include <algorithm>
#include <random>
#include <gmpxx.h>
#include <fstream>
#include <chrono>
#include <ctime>
#include <ratio>
#include "dsa.h"
#include "sha256.h"

using namespace std;
using namespace std::chrono;

int main() 
{
//Tests Performance
//
//EFFECTS: tests performance of keyGeneration and encryptMessage,
//         and verify with different bit lengths for p and q. 
//         Outputs results to Data.csv

	mpz_class myMessage = 123123;
	
	ofstream out;
	out.open("Data.csv");
	out << "P_Bitlength,Q_Bitlength,E_t1,V_t1,E_t2,V_t2,";
	out << "E_t3,V_t3,E_t4,V_t4,E_t5,V_t5" << '\n';
	
	for (int i=1; i < 9; i++) 
	{
		int p_bit = 2*i*16, q_bit = i*16;
		
		out << p_bit << ',' << q_bit << ',';
		
		for (int j=0; j < 5; j++) 
		{
			high_resolution_clock::time_point t1 = high_resolution_clock::now();
			
			mpz_class myX = keyGeneration(p_bit, q_bit);
			pair<mpz_class, mpz_class> encryptedMessage = encrypt(myX, myMessage);
			
			high_resolution_clock::time_point t2 = high_resolution_clock::now();
			
			high_resolution_clock::time_point t3 = high_resolution_clock::now();
			
			if (!(verify(encryptedMessage, myMessage))) 
			{
				cout << "ERROR: Could not verify\n";
				return 0;
			}
			
			high_resolution_clock::time_point t4 = high_resolution_clock::now();
			
			chrono::duration<double> time_encrypt = duration_cast<duration<double>>(t2-t1);
			chrono::duration<double> time_verify = duration_cast<duration<double>>(t4-t3);
			
			out << time_encrypt.count() << ',' << time_verify.count() << ',';
		}
		out << '\n';
	}
	
	out << "2048,256,";
	
	for (int i=0; i < 5; i++) 
	{
		high_resolution_clock::time_point t1 = high_resolution_clock::now();
		
		mpz_class myX = keyGeneration();
		pair<mpz_class, mpz_class> encryptedMessage = encrypt(myX, myMessage);
		
		high_resolution_clock::time_point t2 = high_resolution_clock::now();
		
		high_resolution_clock::time_point t3 = high_resolution_clock::now();
		if (!(verify(encryptedMessage, myMessage))) 
		{
			cout << "ERROR: Could not verify\n";
			return 0;
		}
		
		high_resolution_clock::time_point t4 = high_resolution_clock::now();
		
		chrono::duration<double> time_encrypt = duration_cast<duration<double>>(t2-t1);
		chrono::duration<double> time_verify = duration_cast<duration<double>>(t4-t3);
		
		out << time_encrypt.count() << ',' << time_verify.count() << ',';
	}
	
	return 0;
}
