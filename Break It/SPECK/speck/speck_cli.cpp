#include "speck.h"
#include "unixerror.h"
#include <iostream>
#include <memory>
#include <exception>
#include <limits>
#include <iomanip>
#include <algorithm>
#include <assert.h>
#include <sstream>

using namespace crypto;
using namespace std;

static vector<uint8_t> a2b_hex(string hexstr, int bits) {
    if (hexstr.find("0x") == 0) {
        hexstr = hexstr.substr(2);
    }
    transform(hexstr.begin(), hexstr.end(), hexstr.begin(), ::tolower);
    if ((int) hexstr.size() * 4 > bits) {
        throw runtime_error(string("hex string ") + hexstr + " too long (" +
            to_string(hexstr.size() * 4) + " > " + to_string(bits) + ")");
    }

    size_t digits = bits / 4;
    while (hexstr.size() < digits) {
        hexstr = '0' + hexstr;
    }

    const char* str = hexstr.c_str();
    vector<uint8_t> result;
    result.reserve(bits / 8);
    int temp = 0;

    for (size_t i = 0; i < hexstr.size(); i += 2) {
        if (sscanf(str + i, "%02x", &temp) != 1) {
            throw std::runtime_error(string("invalid hex string ") + hexstr);
        }

        result.push_back(temp);
    }

    assert((int) result.size() == bits / 8);
    return result;
}

static string b2a_hex(const vector<uint8_t>& data, int bits) {
    ostringstream result;
    assert((int) data.size() == bits / 8);
    for (auto b : data) {
        result << hex << setw(2) << setfill('0') << (int) b;
    }
    return result.str();
}

class Error: public exception{
public:
    Error(const string &s):msg(s){}
    string msg;
    
    virtual const char* what() const throw(){
        return msg.c_str();
    }
};

void setkey(int blockBits, int keyBits, const vector<uint8_t> &key, shared_ptr<Speck> &cipher){
    cipher = make_shared<Speck>(blockBits, keyBits);
    cipher->setKey(key.data());
    cout<<"Key Generated"<<endl;
}

string encrypt(const vector<uint8_t> &pt, shared_ptr<Speck> cipher){
    vector<uint8_t> buffer(pt.size());
    cipher->encryptBlock(pt.data(), buffer.data());
    return b2a_hex(buffer,cipher->getKeyBitSize());
}

string decrypt(const vector<uint8_t> &ct, shared_ptr<Speck> cipher){
    vector<uint8_t> buffer(ct.size());
    cipher->decryptBlock(ct.data(), buffer.data());
    return b2a_hex(buffer,cipher->getKeyBitSize());
}

/*valid commands:
 *quit
 *setkey
 *encrypt
 *decrypt
 */
int main(){
    string cmd;
    shared_ptr<Speck> cipher;
    
    while(true){
        try{
            cout<<"Enter Command: ";
            cin>>cmd;
            if(cmd == "quit"){
                return 0;
            }else if(cmd=="setkey"){
                //<blockbits> <keybits><key0>...<keyn>
                int blockBits;
                int keyBits;
                if(!(cin>>blockBits>>keyBits))
                    throw Error("expected number!");
                string line;
                cin >> line;
                vector<uint8_t> key = a2b_hex(line,keyBits);
                
                setkey(blockBits,keyBits,key,cipher);
            }else if(cmd=="encrypt"){
                // plaintext terminated by newline
                if(!cipher)
                    throw Error("must keygen first!");

                string line;
                cin >> line;
                vector<uint8_t> pt = a2b_hex(line,cipher->getBlockBitSize());

                string ct = encrypt(pt,cipher);
                cout<<"Ciphertext: "
                    <<ct
                    <<endl;
            }else if(cmd=="decrypt"){
                // ciphertext
                if(!cipher)
                    throw Error("must keygen first!");
                string line;
                cin >> line;
                vector<uint8_t> ct = a2b_hex(line,cipher->getBlockBitSize());
                string pt = decrypt(ct,cipher);
                cout<<"Plaintext: "
                    <<pt
                    <<endl;
            }else{
                throw Error("unknown command!");
            }
        }catch(Error &e){
            cout<<"ERROR: "<<e.what()<<endl;
            //ignore rest of line
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(),'\n');
        }catch(exception &e){
            cout<<e.what()<<endl;
        }
    }
    
    return 0;
}
