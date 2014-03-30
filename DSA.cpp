#include "DSA.h"
using namespace std;

static uberzahl montyReduction(uberzahl T, uberzahl R, uberzahl M, uberzahl Mprime);
static uberzahl modExpMonty(uberzahl a, uberzahl b, uberzahl M);
// static int generateNums(uberzahl N, uberzahl L);
static uberzahl hash(uberzahl m, uberzahl M);


Signature sign(const PublicKey& pubKey, const uberzahl& x, const uberzahl& m) {
	// make values easier to work with
	const uberzahl &g = pubKey.g;
	const uberzahl &p = pubKey.p;
	const uberzahl &q = pubKey.q;
	// const uberzahl &y = pubKey.y;
	const uberzahl &M = pubKey.M;

	int temp = 0;
	uberzahl zero = temp;
	uberzahl r,r1,k,s;
	do{
		//generate random k s.t. 0<k<q
		r1=modExpMonty(g,k,p);
		r=modExpMonty(r1,1,q);
	}
	while(r == zero);

	do{
		s=modExpMonty(k.inverse(q)*(hash(m,M)+x*r),1,q);
	}
	while(s == zero);

	Signature sig = {r, s};
	return sig;
}

bool verify(const Signature& sig, const PublicKey& pubKey, const uberzahl& m) {
	// make values easier to work with
	const uberzahl &g = pubKey.g;
	const uberzahl &p = pubKey.p;
	const uberzahl &q = pubKey.q;
	const uberzahl &y = pubKey.y;
	const uberzahl &M = pubKey.M;

	const uberzahl& r = sig.r;
	const uberzahl& s = sig.s;

	int temp = 0;
	uberzahl zero = temp;
	if(r<=zero || s<=zero || r>=q || s>=q){
		return false;
	}
	uberzahl w=s.inverse(q);
	uberzahl u_temp=modExpMonty(w,1,q);
	uberzahl u1=hash(m,M)*u_temp;
	uberzahl u2=r*u_temp;
	uberzahl v1=modExpMonty(g,u1,p);
	uberzahl v2=modExpMonty(y,u2,p);
	uberzahl v=modExpMonty(v1*v2,1,q);
	if(v==r){
		return true;
	}
	return false;
}

void testDSA() {
	uberzahl a = 7;
	uberzahl b = 2;
	uberzahl n = 11;
	cout << modExpMonty(a,b,n) << endl;
}

PublicKey::PublicKey(const uberzahl& L, const uberzahl& N, uberzahl& x)
	: p (), q (), g (static_cast<largeType>(1)), y() {

	//generate q of bit size N
	//generate p of bit size L s.t. q|p-1
	static const uberzahl one = 1;
	uberzahl a=(p-one)/q;
	//calculate g (generator)
	int count=2;
	while(g==one){
		g=modExpMonty(count,a,p);
		count++;
	}
	//generate M for hashing
	//generate x randomly 0<x<q
	y=modExpMonty(g,x,p);
}

static uberzahl montyReduction(uberzahl T, uberzahl R, uberzahl M, uberzahl Mprime){
	int temp = 0;
	uberzahl zero = temp;
	uberzahl m=(T*Mprime)&(R-1);
	uberzahl t=(T+m*M)/R;
	if(t>=M){
		return t-M;
	} else if(t>=zero){
		return t;
	} else {
		return t+M;
	}
}

static uberzahl modExpMonty(uberzahl a, uberzahl b, uberzahl M){
	uberzahl R=1;
	int temp = 0;
	uberzahl zero = temp;
	while(R<=M){
		R=R*2;
	}
	uberzahl Mprime=(R*R.inverse(M)-1)/M;
	uberzahl aHat=(a*R)%M;
	uberzahl rtrn=1;
	while(b>zero){
		if(b%2){
			rtrn=montyReduction(aHat*rtrn,R,M,Mprime);
			//rtrn=(a*rtrn)%M;
		}
		b=b>>1;
		aHat=montyReduction(aHat*aHat,R,M,Mprime);
		//a=(a*a)%M;
	}
	return rtrn;
}

static uberzahl hash(uberzahl m, uberzahl M){
	return modExpMonty(m,1,M);
}

