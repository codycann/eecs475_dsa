# include "aux.h"
# include <iostream>
uberzahl shiftLeft64(uberzahl value, int shift, int numbits){
	uberzahl val = (uberzahl(1)<<shiftNum)-1;
    return ((value << shift)&val) | (value >> (numbits - shift));
}
 
uberzahl shiftRight64(uberzahl value, int shift, int numbits) {
	uberzahl val = (uberzahl(1) << shiftNum) - 1;
    return ((value >> shift)) | ((value << ((numbits - shift)))&val);
}
