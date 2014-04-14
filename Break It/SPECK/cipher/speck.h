#ifndef __SPECK_H_
#define __SPECK_H_

#include "uberzahl/uberzahl.h"
#include <vector>
#include <utility>
typedef std::pair<uberzahl, uberzahl> uberpair;

class speck {
private:
    static const int n = 64;
    uberzahl np;
    static const int m = 4;
    static const int T = 34;
    static const int a = 8;
    static const int b = 3;

    void keyExpansion(uberzahl K, std::vector<uberzahl> &k, std::vector<uberzahl> &l);
    uberpair split(uberzahl m);
    uberzahl join(uberzahl x, uberzahl y);

public:
    speck();
    uberzahl encrypt(uberzahl key, uberzahl plaintext);
    uberzahl decrypt(uberzahl key, uberzahl ciphertext);
};

#endif /* __SPECK_H__ */
