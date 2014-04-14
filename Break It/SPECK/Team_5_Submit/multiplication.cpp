#include "uberzahl.h"
#include "multiplication.h"
#include <iostream>
using namespace std;

uberzahl multiply (uberzahl x, uberzahl y) {
    uberzahl z, v = y, r = "299076299051606071403356588563077529600";
    for (int i = 127; i >= 0; --i) {
        // Set z block
        if (x.bit(i) == 0) {
            z = z;
        }
        else {
            z = z ^ v;
        }
        // Set v block
        if (v.bit(0) == 0) {
            v = v >> 1;
        }
        else {
            v = (v >> 1) ^ r;
        }
    }
    return z;
}
