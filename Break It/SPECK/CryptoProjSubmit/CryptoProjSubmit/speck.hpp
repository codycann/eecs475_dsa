//  Team: 32 (The Muffin Men)
//  Name: Anthony Chiang, Eric Edmond, Steven Louie
//  Uniqname: aycc, eedmond, slouie

#ifndef SPECK_IMPL
#define SPECK_IMPL

#include <x86intrin.h>
#include <cstdint>
#include <ctime>
#include <cstdlib>
#include <random>
#include <unistd.h>

#define LCS _lrotl
#define RCS _lrotr
#define u64 uint64_t //short hand
#define Rf(x,y,k) (x=RCS(x,8), x+=y, x^=k, y=LCS(y,3), y^=x) //apply relation forward
#define Ri(x,y,k) (y = y^x, y = RCS(y,3), x^=k, x-=y, x=LCS(x,8)) //apply relation inverse


class SpeckBlockCipher
{
public:
    SpeckBlockCipher();
    ~SpeckBlockCipher();


    void Encrypt(u64 pt[], u64 ct[]);
    void Decrypt(u64 pt[], u64 ct[]);

    void GenerateKey();
    void AssignKey(u64 K[]);
    void GetKey(u64 K[]);
private:

    void KeyExpand();
    void RandomSleepTime();

    u64 GenerateRandomBitstring(int bits);

    u64 keyword[2], subkey[32];
};

SpeckBlockCipher::SpeckBlockCipher()
{
    //Generate a key by default
    GenerateKey();
    KeyExpand();
}

SpeckBlockCipher::~SpeckBlockCipher()
{
    ;;;
}

void SpeckBlockCipher::Encrypt(u64 pt[], u64 ct[])
{
    u64 i;

    ct[0]=pt[0];
    ct[1]=pt[1];
    for(i = 0; i < 32; i++) Rf(ct[1], ct[0], subkey[i]);

    RandomSleepTime();
}

void SpeckBlockCipher::Decrypt(u64 pt[], u64 ct[])
{
    u64 i;

    pt[0]=ct[0];
    pt[1]=ct[1];

    for(i = 31; i>= 0; i--)
    {
        Ri(pt[1], pt[0], subkey[i]);
        if(i==0) break;
    }
    RandomSleepTime();    
}

void SpeckBlockCipher::GenerateKey()
{
    keyword[0] = GenerateRandomBitstring(64);
    keyword[1] = GenerateRandomBitstring(64);
}

void SpeckBlockCipher::AssignKey(u64 K[])
{
    keyword[0] = K[0];
    keyword[1] = K[1];
    KeyExpand();
}

void SpeckBlockCipher::GetKey(u64 K[])
{
    K[0] = keyword[0];
    K[1] = keyword[1];
}

void SpeckBlockCipher::KeyExpand()
{
    //This is described in the paper. See l[i+m-1] is only used once in the entire expansion
    //We need the temps because the macro for Rf defined above modifies the arguments by reference
    u64 temp[2] = {keyword[0], keyword[1]};
    subkey[0] = temp[0];

    for(int i = 0; i < 31; ++i)
    {
        Rf(temp[1], temp[0], i);
        subkey[i+1] = temp[0];
    }
}

void SpeckBlockCipher::RandomSleepTime(){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1000,10000);
    //hopefully prevents timing attacks....
    usleep(dis(gen));
}

u64 SpeckBlockCipher::GenerateRandomBitstring(int bit)
{
    //we use c++11 random as calling time(NULL) repeatedly actually gives the same seed...
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0,1);

    u64 num = 0, one = 1;

    for(u64 i = 0; i < bit; ++i)
    {
        if(dis(gen))
            num |= one << i;
    }

    return num;
}

#endif
