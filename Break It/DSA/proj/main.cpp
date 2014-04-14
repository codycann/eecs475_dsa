#include <iostream>
#include "uberzahl.h"
#include "keygen.h"
#include "SHA1.h"
#include "SignAndVerify.h"
#include <cstdlib>
#include <vector>
using namespace std;



vector<pubkey> keygen(vector<uberzahl> &x)
{
	uberzahl p =123, q = 23, g = 34;
	pqgGen(p,q,g);
	vector<pubkey> keys;
	for(int i = 0; i < x.size(); i++)
	{
		 x[i]= x[i] % q;
		pubkey key(p,q,g,"0");
		key.y = g.expm(x[i], p);
		keys.push_back(key);
	}
	return keys;
}

vector<sig> sign(vector<string> messages, vector<uberzahl> privateKeys, vector<pubkey> publicKeys)
{
	vector<sig> signitures;
	for(int i = 0; i < messages.size(); i++)
	{
		signitures.push_back(sign(messages[i],privateKeys[i],publicKeys[i]));
	}
	return signitures;
}

vector<bool> verify(vector<sig> signitures, vector<pubkey> keys, vector<string> messages )
{
	vector<bool> results;
	for(int i = 0; i < messages.size(); i++)
	{
		results.push_back(verify(signitures[i],keys[i],messages[i]));
	}
	return results;

}


//TEST CODE//
/*
int main( void )
{
	srand(42);
	uberzahl zero = "0";
	
	//Makes one private key
	vector<uberzahl> x ;
	x.push_back(random(zero, 46234623451241245));
	
	//makes the matching public key
	vector <pubkey> key;
	key = keygen(x);
	
	vector<string> m ;
	m.push_back("Hello World!");
	
	//signs one message
	vector<sig> test;
	test=sign(m,x,key);
	
	//verifys the message
	vector<bool> results;
	results=verify(test,key,m);
	
	if(results[0])
		cout<<"YAY!!!"<<endl;
	else
		cout<<"oops"<<endl;

	return 0;
}
*/










