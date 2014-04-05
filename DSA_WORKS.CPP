#include "DSA.h"
#include <ctime>
#include <cstdlib>
#include <iostream>
#include <gmpxx.h>
#include <cassert>
// #include <openssl/sha.h>
using namespace std;

const mediumType kNumTests = 5;
static const uberzahl zero ("0");
static const uberzahl two ("2");

static uberzahl montyReduction(uberzahl T, uberzahl R, uberzahl M, uberzahl Mprime, smallType RBitLength);
static uberzahl modExpMonty(uberzahl a, uberzahl b, uberzahl M);
static uberzahl hash(uberzahl m, uberzahl M);
static uberzahl getP(size_t bitsp, size_t bitsq, const mpz_class& q);
static mpz_class getQ(size_t bitsq, gmp_randstate_t& state);
static uberzahl getGen(const uberzahl& p, const uberzahl& q);


// TODO const refs, PLEASE

Signature sign(const PublicKey& pubKey, const uberzahl& x, const uberzahl& m) {
	// make values easier to work with
	// const uberzahl& g = pubKey.g;
	// const uberzahl& p = pubKey.p;
	// const uberzahl& q = pubKey.q;
	// const uberzahl& M = pubKey.M;

	uberzahl r;
	uberzahl s;
	//generate random k s.t. 0<k<q
	uberzahl k = random(zero, pubKey.q);
	assert(zero < k && k < pubKey.q);
	cout << "k: " << k << endl;
	
	do{
		// TODO this is slow. Problem?
		cout << "pubKey.g = " << pubKey.g << endl;
		r = modExpMonty(pubKey.g,k,pubKey.p) % pubKey.q;
		cout << "r: " << r << endl;
	} while(r == zero);

	do{
		s = k.inverse(pubKey.q)*(hash(m,pubKey.M)+x*r) % pubKey.q;
		cout << endl << endl << "K-INVERSE " << k.inverse(pubKey.q) << endl << endl;
		cout << "s: " << s << endl;
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
	const uberzahl& M = pubKey.M;
	const uberzahl& r = sig.r;
	const uberzahl& s = sig.s;

	// TODO old implementation. I might be missing something, but this looks
	// wrong to me. I'm going off wikipedia
	// int temp = 0;
	// uberzahl zero = temp;
	// if(r<=zero || s<=zero || r>=q || s>=q){
	// 	return false;
	// }
	// uberzahl w=s.inverse(q);
	// uberzahl u_temp=modExpMonty(w,1,q);
	// uberzahl u1=hash(m,M)*u_temp;
	// uberzahl u2=r*u_temp;
	// uberzahl v1=modExpMonty(g,u1,p);
	// uberzahl v2=modExpMonty(y,u2,p);
	// uberzahl v=modExpMonty(v1*v2,1,q);
	// if(v==r){
	// 	return true;
	// }
	// return false;

	if(!(zero < r && r < q) || !(zero < s && s < q))
		return false;

	uberzahl w = s.inverse(q);
	uberzahl u1 = (hash(m,M)*w) % q;
	uberzahl u2 = (r * w) % q;
	uberzahl v1 = modExpMonty(g,u1,p);
	uberzahl v2 = modExpMonty(y,u2,p);
	uberzahl v = ((v1 * v2) % p) % q;
	cout << endl << "v1: " << v1 << endl;
	cout << endl << "v2: " << v2 << endl;
	cout << "u1: " << u1 << endl;
	cout << "u2 " << u2 << endl;
	cout << "v: " << v << endl;
	cout << "r: " << r << endl;

	assert(hash(m, M).bitLength() == 160);
	assert((w * s) % q == 1);
	assert(u1 == modExpMonty(hash(m, M) * w, 1, q));
	assert(u2 == modExpMonty(r * w, 1, q));
	//assert(v == modExpMonty(v1 * v2, 1, q));
	if(v == r)
		return true;
	return false;
}

void testDSA() {
	// uberzahl a = 7;
	// uberzahl b = 2;
	// uberzahl n = 11;
	// cout << modExpMonty(a,b,n) << endl;
	
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

// for fast testing
PublicKey::PublicKey(uberzahl& x)
	: p ("151922796094617702893525465464802434266410100690351740282113590602668472872428967532578883964046372880303449703749959751435043967793536508977788065921707024729264249502471101743113063338218963644408273415835478279591089439839415199754387849538501285876473282325811196636754031408309235362687073645583188822031")
	, q ("1235112995284249207516635239491827277563965299903")
	, g ("65799697247778758509851377705700137107206282741829920291857468052498913055438332826497676193845143464393276736346787649341306709463027769032395857054341832424089152033800432895513182114939541824323105115664759033076750143109414617880141244374499045745310946620961036469371509417282316580888169263813743485518")
{
	x = random(zero, q);
}

PublicKey::PublicKey(const mediumType L, const mediumType N, uberzahl& x)
	: p (), q (), g (static_cast<largeType>(1)), y() {

	mpz_class seed(time(NULL));
	gmp_randstate_t state;
	gmp_randinit_default(state);
	gmp_randseed(state, seed.get_mpz_t());

	while(p == zero) {
		//generate q of bit size N
		mpz_class q_mpz = getQ(N, state);
		q = q_mpz;
		// cout << "q_mpz: " << q_mpz << endl;

		//generate p of bit size L s.t. q|p-1
		p = getP(L, N, q_mpz);
	}

	//calculate g (generator)
	g = getGen(p, q);

	//generate M for hashing
	M = p * q;

	//generate x randomly 0<x<q
	do {
		x = random(zero, q);
	} while(x == zero || x == q);
	cout <<endl <<  "X: "<< x << endl << endl;
	// generate y
	y = modExpMonty(g,x,p);

	cout << "P: " << p << endl;
	cout << "Q: " << q << endl;
	cout << "G: " << g << endl;
	cout << "G^Q mod P = " << modExpMonty(g, q, p) << endl;

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

	cout << "These values pass all requirements for the public key" << endl;
}

static uberzahl montyReduction(uberzahl T, uberzahl R, uberzahl M, uberzahl Mprime, smallType RBitLength){
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

static uberzahl modExpMonty(uberzahl a, uberzahl b, uberzahl M){
	uberzahl oldB (b);// TODO remove
	uberzahl R=1;
	smallType RBitLength = 0;
	while(R<=M){
		// R=R*2;
		R = R << 1;
		++RBitLength;
	}
	// cout << "Done with R" << endl;
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

	// #ifndef NDEBUG
	// uberzahl correct = a.expm(oldB, M);
	// if(rtrn != correct) {
	// 	cout << "a: " << a << endl;
	// 	cout << "b: " << b << endl;
	// 	cout << "M: " << M << endl;
	// 	cout << "rtrn = " << rtrn << endl;
	// 	cout << "a.expm(b, M) = " << correct << endl;
	// 	assert(false);
	// }
	// #endif
	return rtrn;
}

// TODO what should this do? Is this correct?
static uberzahl hash(uberzahl m, uberzahl M){
	uberzahl retVal = "874821946267123792280058894950784228535709909184"; // SHA-1 of SNSD (160 bits)
	cout << "bitLength of retVal " << retVal.bitLength() << endl;
	assert(retVal.bitLength() == 160);
	return retVal;
}

static mpz_class getQ(size_t bitsq, gmp_randstate_t& state) {
	mpz_class minQ (1); // q must be > 2^(N - 1)
	mpz_mul_2exp(minQ.get_mpz_t(), minQ.get_mpz_t(), bitsq - 1);
	mpz_class q;
	
	do {	
		//this will run until q is determined to be prime and 2^(N-1) < q < 2^N
		mpz_urandomb(q.get_mpz_t(), state, bitsq);
		// cout << "randomly chose q = " << q << endl;
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
		cout << h << endl;
	}
	abort();
	return uberzahl();
}
