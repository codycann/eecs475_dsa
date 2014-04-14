#include "present.h"

Present::Present(uberzahl u, int keysize) {
    //TODO set custom rounds
    if (keysize <= 10) {
        for (int i = 0; i < 32; i++) {
            keys[i] = u >> 16;
            u = ((u & (uberzahl(1) << 19) - 1) << 61) + (u >> 19);
            u = (uberzahl(sbox[(u >> 76) % 16]) << 76) +  (u & ((uberzahl(1) << 76)- 1));
            u = u ^ (uberzahl(i + 1) << 15);
        }
    } else if (keysize <= 16) {
        for (int i = 0; i < 32; i++) {
            keys[i] = u >> 64;
            u = ((u & (uberzahl(1) << 67) - 1) << 61) + (u >> 67);
            u = (uberzahl(sbox[(u >> 124) % 16]) << 124)
                    + (uberzahl(sbox[(u >> 120) % 16]) << 120)
                    + (u & ((uberzahl(1) << 120)- 1));
            u = u ^ (uberzahl(i + 1) << 62);
        }
    } else {
        cout << "key size should be either 80bits or 128 bits" << endl;
    }
}

uberzahl Present::encrypt(uberzahl c) {
//    cout << c << endl;
    for (int i = 0; i < 31; i++) {
        c = c ^ keys[i];
        // first do sbox step
//        cout << c << endl;
        uberzahl c2;
        for (int j = 0; j < 16; j++) {
            c2 = c2 + (uberzahl(sbox[(c >> (j * 4)) % 16]) << (j * 4));
        }
//        cout << c2 << endl;
        // next do player step
        uberzahl c3;
        for (int j = 0; j < 64; j++) {
            c3 = c3 + (((c2 >> j) & 1) << pbox[j]);
        }
//        cout << c3 << endl;
        c = c3;
    }
    c = c ^ keys[31];
//    cout << c << endl;
    return c;
}

uberzahl Present::decrypt(uberzahl u) {
//    cout <<  cipher.size() << endl;
//    cout << u << endl;
    for (int i = 0; i < 31; i++) {
        u = u ^ keys[31 - i];
        uberzahl u2;
        for (int j = 0; j < 64; j++) {
            u2 = u2 + (((u >> j) & 1) << pbox_inv[j]);
        }
        uberzahl u3;
        for (int j = 0; j < 16; j++) {
            u3 = u3 + (uberzahl(sbox_inv[(u2 >> (j * 4)) % 16]) << (j * 4));
        }
        u = u3;
    }
    u = u ^ keys[0];
    return u;
}
