#include "uberzahl.h"
#include "gctr.h"

uberzahl increment (uberzahl x) {
    // Extract and "increment" least significant 32 bits from x
    uberzahl leastSigBits = betterExtract(x, 0, 31) + 1;
    uberzahl modulo = 4294967296;
    leastSigBits = leastSigBits % modulo;
    // Set least significant bits in x
    for (int i = 0; i < 32; ++i) {
        if(leastSigBits.bit(i) == 0) {
            x.clearBit(i);
        }
        else {
            x.setBit(i);
        }
    }
    return x;
}
