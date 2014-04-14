
#include <x86intrin.h>
#include <stdint.h>
#define LCS _lrotl //left circular shift
#define RCS _lrotr //right circular shift
#define u64 uint64_t
#define R(x,y,k) (x=RCS(x,8), x+=y, x^=k, y=LCS(y,3), y^=x)

void Speck128ExpandKeyAndEncrypt(u64 pt[], u64 ct[], u64 K[])
{
u64 i,B=K[1],A=K[0];
ct[0]=pt[0]; ct[1]=pt[1];
for(i=0; i<32; i++){R(ct[1], ct[0], A); R(B, A, i);}
}