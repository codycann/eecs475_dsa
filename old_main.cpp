#include <iostream>
using namespace std;

int inverse(int x, int M){
	int rtrn=1;
	while((rtrn*x)%M!=1){
		rtrn++;
	}
	return rtrn;
}

int montyReduction(int T, int R, int M, int Mprime){
	int m=(T*Mprime)&(R-1);
	int t=(T+m*M)/R;
	if(t>=M){
		return t-M;
	} else if(t>=0){
		return t;
	} else {
		return t+M;
	}
}

int modExpMonty(int a, int b, int M){
	int R=1;
	while(R<=M){
		R*=2;
	}
	int Mprime=(R*inverse(R,M)-1)/M;
	int aHat=(a*R)%M;
	int rtrn=1;
	while(b>0){
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

int generateNums(int N, int L){
	//generate q of bit size N
	//generate p of bit size L s.t. q|p-1
	int p,q;
	int a=(p-1)/q;
	//calculate g (generator)
	int g=1;
	int count=2;
	while(g==1){
		g=modExpMonty(count,a,p);
		count++;
	}
	//generate M for hashing
	//generate x randomly 0<x<q
	int x;
	int y=modExpMonty(g,x,p);
	return 0;
}

int Hash(int m, int M){
	return modExpMonty(m,1,M);
}

int Signing(int p, int q, int g, int y, int x, int m, int M){
	int r,r1,k,s;
	do{
		//generate random k s.t. 0<k<q
		r1=modExpMonty(g,k,p);
		r=modExpMonty(r1,1,q);
	}
	while(r == 0);

	do{
		s=modExpMonty(inverse(k,q)*(Hash(m,M)+x*r),1,q);
	}
	while(s == 0);

	return 0;
}

bool Verifying(int r, int s, int p, int q, int g, int y, int m, int M){
	if(r<=0 || s<=0 || r>=q || s>=q){
		return false;
	}
	int w=inverse(s,q);
	int u_temp=modExpMonty(w,1,q);
	int u1=Hash(m,M)*u_temp;
	int u2=r*u_temp;
	int v1=modExpMonty(g,u1,p);
	int v2=modExpMonty(y,u2,p);
	int v=modExpMonty(v1*v2,1,q);
	if(v==r){
		return true;
	}
	return false;
}

int main(){
	


	return 0;
}