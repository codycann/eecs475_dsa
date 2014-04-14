#include <iostream>
#include <vector>
#include "present.h"
#include "475Project.h"

using namespace std;
int main() {
    uberzahl u("4759477275222530853120");
    Present p(u, 16);
    uberzahl u2("283686952306183");
    uberzahl c = p.encrypt(u2);
    cout << c << endl;
    uberzahl m = p.decrypt(c);
    cout << m << endl;

    vector <uberzahl> keys, plaintexts, cyphers;
    keys.push_back(keygen(80));
    keys.push_back(keygen(128));
    plaintexts.push_back(u.random(64));
    plaintexts.push_back(u.random(64));
    cyphers = encrypt(keys, plaintexts);
    cout << "keys: " << " " << keys[0] << " " << keys[1] << endl;
    cout << "plaintexts: " << " " << plaintexts[0] << " " << plaintexts[1] << endl;
    cout << "cyphers: " << " " << cyphers[0] << " " << cyphers [1] << endl;
    plaintexts = decrypt(keys, cyphers);
    cout << "plaintexts decrypted: " << " " << plaintexts[0] << " " << plaintexts[1] << endl;
}
