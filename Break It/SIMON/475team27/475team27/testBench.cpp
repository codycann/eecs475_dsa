#include <iostream>
#include <vector>
#include "present.h"
#include "475Project.h"

using namespace std;

int main() {
    uberzahl key;
    uberzahl key2("18446744073709551615"); // 0xFFFFFFFFFFFFFFFFFFFF
    uberzahl plaintext;
    uberzahl plaintext2("18446744073709551615"); // 0xFFFFFFFFFFFFFFFFFFFF

    Present p(key, 10);
    uberzahl c = p.encrypt(plaintext);
    cout << "PlainText: 0; Key: 0; CipherText: " << c << "; Decrypted Back: " << p.decrypt(c) <<  endl;

    Present p2(key2, 10);
    c = p2.encrypt(plaintext);
    cout << "PlainText: 0; Key: 18446744073709551615; CipherText: " << c << "; Decrypted Back: " << p2.decrypt(c) << endl;

    Present p3(key, 10);
    c = p3.encrypt(plaintext2);
    cout << "PlainText: 18446744073709551615; Key: 0; CipherText: " << c << "; Decrypted Back: " << p3.decrypt(c) <<endl;

    Present p4(key2, 10);
    c = p4.encrypt(plaintext2);
    cout << "PlainText: 18446744073709551615; Key: 18446744073709551615; CipherText: " << c << "; Decrypted Back: " << p4.decrypt(c) << endl;
}
