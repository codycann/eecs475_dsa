#include <iostream>
#include <cstdlib>
using namespace std;
#include "uberzahl.h"
//#include <gmpxx.h>
#include <cassert>

#define asize 56
#define bsize 50
/*
mpz_class generateNumber(int bits,bool lead1) {
  mpz_class r = lead1?1:rand()%2;
  for(int b=1;b<bits;b++) {
    r*=2;
    if(rand()%2)
      r++;
  }
  if(r==0)
    return generateNumber(bits,lead1);
  return r;
}
*/

int main( void ){ 
 /* mpz_class scale= 1;
  for(int i=0;i<32;i++)
    scale*=2;

  srand(time(0));
  	for(int i=0;i<10000;i++)
      {
      mpz_class a = generateNumber(asize,false);
      mpz_class b = generateNumber(bsize,false);
  //		a=597,b=265
  unsigned int c = rand();
  unsigned int d = c%(asize*11/10);
  //cout << a << " " << b << " " << c << " " << d << endl;
  uberzahl ua = a,ub = b;
  assert(ua==a);
  assert((ua==b)==(a==b));
  assert(ua+ub==a+b);
  if(a>b)
  assert(ua-ub==a-b);
  else
  assert(ub-ua==b-a );
  //cout << ua*ub << endl << a*b << endl;
  assert(ua*ub==a*b);
  assert(ua/ub==a/b);
  assert(ua/ua==a/a);
  assert(ua/c == a/c);
  assert(ub%ua == b%a);
  assert(ub%c == b%c);
  assert((a<b)==(ua<ub));
  assert((b<a)==(ub<ua));
  assert((a<a)==(ua<ua));
  assert((a>b)==(ua>ub));
  assert((b>a)==(ub>ua));
  assert((a>a)==(ua>ua));
  assert((a<=b)==(ua<=ub));
  assert((b<=a)==(ub<=ua));
  assert((a<=a)==(ua<=ua));
  assert((a>=b)==(ua>=ub));
  assert((b>=a)==(ub>=ua));
  assert((a>=a)==(ua>=ua));
  assert((a==b)==(ua==ub));
  assert((a==a)==(ua==ua));
  assert((a!=b)==(ua!=ub));
  assert((a!=a)==(ua!=ua));
  mpz_class shift = 1;
  for(int j=0;j<d;j++)
  shift*=2;
  assert(ua<<d==a*shift);
  assert(ua>>d==a/shift);
  mpz_class r;
  mpz_gcd(r.get_mpz_t(),a.get_mpz_t(),b.get_mpz_t());
  if(r==1 && b>1)
  assert((ua*ua.inverse(ub))%ub=="1");
  else if(b>1)
  assert(ua.inverse(ub)=="0");
  uberzahl one = "1";
  //		cout << (one<<d) << " " << (ua^(one<<d)) << " " << ua.bit(d) << endl;
  assert((ua>(ua^(one<<d)))==ua.bit(d));
  int length = ua.bitLength();
  assert(ua=="0" || (one<<length>ua && one<<(length-1)<=ua));

  if(i%100==0)
  cout << i/100 << "%" << endl;
  }
 */ 
  uberzahl x;
  x.random(150);
  cout << "after (and including) " << x << ", the next prime is " << std::flush;
  cout << nextprime(x,50);
}

