#include "DSA.h"
#include "hash.h"
#include <ctime>
#include <cstdlib>
#include <iostream>
#include <gmpxx.h>
#include <cassert>
using namespace std;

const mediumType kNumTests = 50;
static const uberzahl zero ("0");
static const uberzahl two ("2");

static uberzahl montyReduction(const uberzahl& T, const uberzahl& R, const uberzahl& M
	, const uberzahl& Mprime, smallType RBitLength);
static uberzahl modExpMonty(const uberzahl& a, uberzahl b, const uberzahl& M);
static uberzahl getP(size_t bitsp, size_t bitsq, const mpz_class& q);
static mpz_class getQ(size_t bitsq, gmp_randstate_t& state);
static uberzahl getGen(const uberzahl& p, const uberzahl& q);


Signature sign(const PublicKey& pubKey, const uberzahl& x, const uberzahl& m) {
	uberzahl r;
	uberzahl s;
	//generate random k s.t. 0<k<q
	uberzahl k = random(zero, pubKey.q);
	assert(zero < k && k < pubKey.q);
	
	do{
		r = modExpMonty(pubKey.g,k,pubKey.p) % pubKey.q;
	} while(r == zero);

	do{
		s = k.inverse(pubKey.q)*(sha256(m,pubKey.N)+x*r) % pubKey.q;
	}
	while(s == zero);

	Signature sig = {r, s};
	return sig;
}

bool verify(const Signature& sig, const PublicKey& pubKey, const uberzahl& m) {
	// make values easier to work with
	const uberzahl& g = pubKey.g;
	const uberzahl& p = pubKey.p;
	const uberzahl& q = pubKey.q;
	const uberzahl& y = pubKey.y;
	const uberzahl& r = sig.r;
	const uberzahl& s = sig.s;

	if(!(zero < r && r < q) || !(zero < s && s < q))
		return false;

	uberzahl w = s.inverse(q);
	uberzahl u1 = (sha256(m,pubKey.N)*w) % q;
	uberzahl u2 = (r * w) % q;
	uberzahl v1 = modExpMonty(g,u1,p);
	uberzahl v2 = modExpMonty(y,u2,p);
	uberzahl v = ((v1 * v2) % p) % q;

	// assert(sha256(m, pubKey.N).bitLength() == pubKey.N);
	assert((w * s) % q == 1);
	assert(u1 == modExpMonty(sha256(m, pubKey.N) * w, 1, q));
	assert(u2 == modExpMonty(r * w, 1, q));
	//assert(v == modExpMonty(v1 * v2, 1, q));
	if(v == r)
		return true;
	return false;
}

void testDSA() {
	uberzahl a = 7;
	uberzahl b = 2;
	uberzahl n = 11;
	cout << modExpMonty(a,b,n) << endl;
	
	mpz_class g ("3016039721141937871805538731801507672609541358279912926790998377681195641194377433294076358544232448251941493472361899191023056177271785866885619193423746816443527084185355876941329854790411944973297146947531495526491330903267392308088037805615430735641490016493558375091057366702614723444313028524620722551");
	mpz_class k ("182925440200460003849078955987769983778593014525");
	mpz_class q ("1278576197130442914354605876731966457836118802191");
	mpz_class p ("157268907079531168571016512275475674236181708331762078770015447411320093224521865970960715151153217242695930676334665846553044765009240537953179281124914667342254259142255790779365698887745659659491312905252692164779532588148430300949015008133126102463547142916865889793094381651817076179816398730238778151123");
	mpz_class ans;
	mpz_powm(ans.get_mpz_t(), g.get_mpz_t(), k.get_mpz_t(), p.get_mpz_t());
	cout << ans << endl << endl;
	cout << ans % q << endl << endl;
	cout << modExpMonty(g, k, p) << endl << endl;
	cout << modExpMonty(g, k, p) % q<< endl;

	cout << "testDSA() over" << endl;

}

PublicKey::PublicKey(const mediumType L, const mediumType N_, uberzahl& x)
	: p (), q (), g (static_cast<largeType>(1)), y(), N (N_) {

	mpz_class seed(time(NULL));
	gmp_randstate_t state;
	gmp_randinit_default(state);
	gmp_randseed(state, seed.get_mpz_t());

	while(p == zero) {
		//generate q of bit size N
		mpz_class q_mpz = getQ(N, state);
		q = q_mpz;

		//generate p of bit size L s.t. q|p-1
		p = getP(L, N, q_mpz);
	}

	//calculate g (generator)
	g = getGen(p, q);

	//generate x randomly 0<x<q
	do {
		x = random(zero, q);
	} while(x == zero || x == q);

	// generate y
	y = modExpMonty(g,x,p);

	// Things we pass
	assert((uberzahl(1) << (L - 1)) < p && p < (uberzahl(1) << L));
	assert(p.bitLength() == L);
	assert((uberzahl(1) << (N - 1)) < q && q < (uberzahl(1) << N));
	assert(q.bitLength() == N);
	assert((p - 1) % q == zero);
	assert(modExpMonty(g, q, p) == 1);
	assert(uberzahl("1") < g && g < p);
	assert(zero < x && x < q);
	assert(y == g.expm(x, p));

}

static uberzahl montyReduction(const uberzahl& T, const uberzahl& R, const uberzahl& M
	, const uberzahl& Mprime, smallType RBitLength){

	uberzahl m=(T*Mprime)&(R-1);
	uberzahl t=(T+m*M) >> RBitLength; // if we use regular divide it's slow
	if(t>=M){
		return t-M;
	} else if(t>=zero){
		return t;
	} else {
		return t+M;
	}
}

static uberzahl modExpMonty(const uberzahl& a, uberzahl b, const uberzahl& M) {
	#ifndef NDEBUG
	uberzahl oldB (b);
	#endif

	uberzahl R=1;
	smallType RBitLength = 0;
	while(R<=M){
		R = R << 1;
		++RBitLength;
	}
	uberzahl Mprime=(R*R.inverse(M)-1)/M;
	uberzahl aHat=(a*R)%M;
	uberzahl rtrn=1;
	while(b>zero){
		if(b%2){
			rtrn=montyReduction(aHat*rtrn,R,M,Mprime, RBitLength);
		}
		b=b>>1;
		aHat=montyReduction(aHat*aHat,R,M,Mprime, RBitLength);
	}
	return rtrn;
}

static mpz_class getQ(size_t bitsq, gmp_randstate_t& state) {
	mpz_class minQ (1); // q must be > 2^(N - 1)
	mpz_mul_2exp(minQ.get_mpz_t(), minQ.get_mpz_t(), bitsq - 1);
	mpz_class q;
	
	do {	
		//this will run until q is determined to be prime and 2^(N-1) < q < 2^N
		mpz_urandomb(q.get_mpz_t(), state, bitsq);
	} while(!mpz_probab_prime_p(q.get_mpz_t(), kNumTests) || q < minQ);
	return q;
}

static uberzahl getP(size_t bitsp, size_t bitsq, const mpz_class& q) {
	mpz_class p_minus_one;
	mpz_class p;
	mpz_mul_2exp(p_minus_one.get_mpz_t(), q.get_mpz_t(), bitsp-bitsq);

	//basically loook for an appropriately sized p that's also prime
	for(; mpz_sizeinbase(p_minus_one.get_mpz_t(), 2) == bitsp; p_minus_one += q) {
		p = p_minus_one + 1;
		if(mpz_probab_prime_p(p.get_mpz_t(), kNumTests))
			return p;
	}
	return uberzahl();
}

static uberzahl getGen(const uberzahl& p, const uberzahl& q) {
	// mpz_class ans;
	uberzahl exp = (p - 1) / q;
	uberzahl h = 2;
	uberzahl g = 2;
	while(h < p) {
		g = modExpMonty(h, exp, p);
		if(g != 1)
			return g;
		h = h + 1;
	}
	abort();
	return uberzahl();
}
