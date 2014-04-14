#ifndef SPECK_H
#define SPECK_H

#include <vector>
#include <iostream>
#include <string>
#include <random>
#include <chrono>
#include "functionlib.h"
#include "fixedInt.h"

#define uint16 fixedInt<2>
#define uint24 fixedInt<3>
#define uint32 fixedInt<4>
#define uint48 fixedInt<6>
#define uint64 fixedInt<8>
#define uint96 fixedInt<12>
#define uint128 fixedInt<16>

using namespace std;

/* generates a key using Mersenne Twister engine.
 * to generate a  256-bit key:
 *
 * vector<uint64> key = generate_key<uint64>(4); // key with 4 64-bit words
 */
template <typename T>
vector<T> generate_key(int num_words) {
	unsigned seed = chrono::system_clock::now().time_since_epoch().count();
	mt19937 gen(seed);
	uniform_int_distribution<int> dist(0, 255);
	vector<T> k;
	for (int i = 0; i < num_words; ++i) {
		T x(0);
		for (int j = 0; j < sizeof(T); ++j)
			x.data[j] = dist(gen);
		k.push_back(x);
	}
	return k;
}
/* round function as described in SPECK paper */
template <typename T, typename U>
U roundFunction(T x, T y, T k, unsigned int alpha, unsigned int beta)
{
	//add word-size modulus, if neccessary
	U xret=(rotate(x,0-(unsigned int)alpha)+y)^k;
	U yret=rotate(y,beta)^((T)xret);
	U ret=(xret<<(8*sizeof(U)/2))+yret;
	return ret;
}
/* inverse of the round function as described in SPECK paper */
template <typename T, typename U>
U invertedRoundFunction(T x, T y, T k, unsigned int alpha, unsigned int beta)
{
	U yret=rotate(y^x,0-(unsigned int)beta);
	U xret=rotate((x^k)-(T)yret,alpha);
	U ret=(xret<<(8*sizeof(U)/2))+yret;
	return ret;
}
/* takes a key generated by generate_key() and expands it for encryption and decryption */
template <typename T, typename U>
vector<T> expandKey(unsigned int numberRounds, vector<T> k)
{
	unsigned int numberKeyWords = k.size();

	int alpha = 7+(sizeof(T) * 8 !=16);
	int beta  = 2+(sizeof(T) * 8 !=16);
	vector<T>l=k;

	l.resize(numberRounds+numberKeyWords);
	//pop front
	for(int i=0;i<l.size()-1;++i)
		l[i]=l[i+1];
	k.resize(numberRounds);

	for(unsigned int i=0;i<=numberRounds-2;++i)
	{	
		U roundResult;
		roundResult=roundFunction<T, U>(l[i],k[i], T(i), alpha, beta);
		l[i+numberKeyWords-1]=msh(roundResult);
		k[i+1]=lsh(roundResult);
	}
	return k;
	
}
/* encrypts a single block. */
template <typename T, typename U>
U encipherBlock(U block, unsigned int numberRounds, vector<T> k)
{
	int alpha = 7+(sizeof(T) * 8 !=16);
	int beta  = 2+(sizeof(T) * 8 !=16);

	for(int i=0;i<numberRounds;++i)
	{	
		T x=msh(block);
		T y=lsh(block);
		block=roundFunction<T, U>(x,y,k[i], alpha, beta);
	}
	return block;
}
/* decrypts a single block. */
template <typename T, typename U>
U decipherBlock(U block, unsigned int numberRounds, vector<T> k)
{
	int alpha = 7+(sizeof(T) * 8 !=16);
	int beta  = 2+(sizeof(T) * 8 !=16);

	for(int i=numberRounds-1;i>=0;--i)
	{
		T x=msh(block);
		T y=lsh(block);
		block=invertedRoundFunction<T, U>(x,y,k[i], alpha, beta);
	}
	return block;
}
/* encrypts a sequence of blocks in ECB mode. */
template <typename T, typename U>
vector<U> encrypt(vector<U> message, 
                       vector<T> key,
                       unsigned int numberRounds = 27)
{
	for(unsigned int i=0;i<message.size();++i)
		message[i]=encipherBlock(message[i],numberRounds,key);
	return message;
}
/* decrypts a sequence of blocks in ECB mod. */
template <typename T, typename U>
vector<U> decrypt(vector<U> message,
                       vector<T> key,
                       unsigned int numberRounds = 27)
{
	for(unsigned int i=0;i<message.size();++i)
		message[i]=decipherBlock(message[i],numberRounds,key);
	return message;

}
#endif