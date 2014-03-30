#ifndef DSA_H
#define DSA_H

#include "uberzahl/uberzahl.h"

/**
 * Convenience struct to allow us to pass around and return signatures
 */
struct Signature {
	uberzahl r;
	uberzahl s;
};

/**
 * Represents a public key along with value M, which is used to speed up
 * computation
 */
struct PublicKey {
	/**
	 * Returns a public key s.t. q is of bit size N and p is of bit size L.
	 * Parameter x is modified to contain the private key.
	 * From wikipedia:
	 * FIPS 186-3 specifies L and N length pairs of (1024,160), (2048,224),
	 * (2048,256), and (3072,256).
	 */
	PublicKey(const mediumType L, const mediumType N, uberzahl& x);
	uberzahl p;
	uberzahl q;
	uberzahl g;
	uberzahl y;
	// included for Montgomery optimization
	uberzahl M;
};

/**
 * Returns a signature (r, s) generated by signing message m with pubKey and
 * private key x.
 */
Signature sign(const PublicKey& pubKey, const uberzahl& x, const uberzahl& m);

/**
 * Returns true iff sig is a valid signature for pubKey and message m
 */
bool verify(const Signature& sig, const PublicKey& pubKey, const uberzahl& m);

/**
 * Run tests on DSA internals (like modexp, montgomery stuff, etc.)
 */
void testDSA();

#endif
