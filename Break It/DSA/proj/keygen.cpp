#include <iostream>
#include <cstdlib>
#include <gmpxx.h>//compile w/ -lgmpxx -lgmp
#include "uberzahl.h"
#include "keygen.h"
using namespace std;


//must be a multiple of 64
//NIST: 2048 or 3072
//ORIGINAL DSS: 64*a + 512 (0 <= a <= 8)
//FIPS: (L_length, N_length) = (1024, 160) or (2048, 224) or (2048, 256) or (3072, 256)
unsigned int L= 512;
//must be less than or equal to hash output length
//stinson says 160 bit q
unsigned int N = 160;

uberzahl zero = "0";
mpz_class one = 1;



//x is a random number 0 < x < q - private key
void pqgGen(uberzahl & p, uberzahl & q, uberzahl & g) {
	//find a L-bit long p prime
  /*  uberzahl rand;
    p = 2;
   	rand.random(L);
   	string p_string = rand.convert_to_string();
   	mpz_class p_mpz(p_string, 10);
   	while(p.bitLength() != L) {
   		mpz_nextprime(p_mpz.get_mpz_t(), p_mpz.get_mpz_t());
   		p_string = p_mpz.get_str(10);
   		p = p_string.c_str();
   	}
   	cout<<p<<endl;
   	*/


   	//find an N bit q such that p-1 divides q
//method 1
/*	rand.random(N); 	
	string q_string = rand.convert_to_string();
   	mpz_class q_mpz(q_string, 10);
   	while(q.bitLength() != N || (((p-1)%q) != zero)) {
   		mpz_nextprime(q_mpz.get_mpz_t(), q_mpz.get_mpz_t());
   		q_string = q_mpz.get_str(10);
   		q = q_string.c_str();
   		cout<<1<<endl;
   	}
   	cout<<q<<endl;
*/
//method2
/*   	uberzahl two = 2;
   	uberzahl lower_bound = two.exp(N-1);
   	q = lower_bound;
   	mpz_class q_mpz(q.convert_to_string(), 10);
   	string q_string;
   	while(((p-1) % q) != zero) {
   		mpz_nextprime(q_mpz.get_mpz_t(), q_mpz.get_mpz_t());
   		q_string = q_mpz.get_str(10);
   		q = q_string.c_str();
   		if (q.bitLength() != N) {
   			cout<<"no such q exists"<<endl;
   			exit(1);
   		}
   		cout<<1<<endl;
   	}
   	cout<<q<<endl;
   	cout<<2<<endl;
*/



   	string p_string, q_string;
   	mpz_class p_mpz;
   	uberzahl rand;
   	bool none = 0;
   	while(1) {
   		//find random N bit prime q
   		do {
	   		rand.random(N);
	   		q = nextprime(rand, 10);
	   		q_string = q.convert_to_string();
	   		q = q_string.c_str();
	   	}
   		while (q.bitLength() != N);

   		mpz_class q_mpz(q_string, 10);


   		mpz_class mult_mpz = 3;
   		while (1) {
   			mpz_class product_mpz = mult_mpz * q_mpz;

   			//if product is odd -> no way product+1 is prime
   			if ((product_mpz % 2) == one) {
   				mult_mpz = mult_mpz + 1;
   				continue;
   			}
   			//this number should be p-1
   			mpz_class product_1_mpz = product_mpz + 1;
   			//check if p is a prime
   			uberzahl product = (product_mpz.get_str(10)).c_str();
   			uberzahl product_1 = (product_1_mpz.get_str(10)).c_str();



   			//missing 1 bit
   			if (product.bitLength() == L-1) {
   				mult_mpz = mult_mpz * 2;
   			}
   			//increment mult if mult not big enough
   			if (product.bitLength() < L) {
   				double diff = L - product.bitLength();
   				mult_mpz = mult_mpz * diff;
   			}

   			//increment multi if mult is not big enough
   			//mult too high? find other q
   			if (product.bitLength() > L) {
   				none = 1;
   				break;
   			}

   			//if it is of bitlength N --> check product if it is a prime
   			if (mpz_probab_prime_p(product_1_mpz.get_mpz_t(), 10) > 0) {
   				//found a right p
   				p_string = product_1_mpz.get_str(10);
   				p = p_string.c_str();
   				break;
   			}
   			else {
	   				mult_mpz = mult_mpz + 1;
	   		}
   		}	


   		if (none) {
   			none = 0;
   			continue;
   		}
   		else 
   			break;

   	}



//    while (q.bitLength() != N || ((p-1) % q) != zero ) {
//    	rand.random(N);
//    	uberzahl rand = random(lower_bound, upper_bound);
//     	q = nextprime(rand, 20); // this is gonna EVEN slower
//	} 



	//find g with multiplicative order modulo p = q
	uberzahl h = 20;
	g = 1;
	while (g == 1) {
		uberzahl c = ((p-1)/q);
		g = h.expm(c, p);
		h = h + 1;
		if (h == (p-1)) {
			cout<<"no such generator g"<<endl;
			exit(1);
		}
	}

}



