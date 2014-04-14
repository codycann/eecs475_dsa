#include <iostream>
#include <cstdlib>
using namespace std;
#include "uberzahl.h"
//#include <gmpxx.h>
#include <time.h>
#include <cassert>
#include <string>

#define asize 56
#define bsize 50


void addRoundKey(uberzahl &state, uberzahl &key){
    state = state ^ key;
}

void printUberzahlHexadec(const uberzahl u)
{
    for (int i = 15; i>=0; --i)
    {
        uberzahl y = (u >> (i*4)) & 15;
        string s = y.convert_to_string();
        int r = std::atoi(s.c_str());
        //cout << std::hex << r;
    }
    //cout << endl;
}

uberzahl genKeysSBoxenc(uberzahl y){
    
    int sub = 0;
    
    if (y == "0"){
        sub = 12;
    }
    else if (y == "1"){
        sub = 5;
    }
    else if (y == "2"){
        sub = 6;
    }
    else if (y == "3"){
        sub = 11;
    }
    else if (y == "4"){
        sub = 9;
    }
    else if (y == "5"){
        sub = 0;
    }
    else if (y == "6"){
        sub = 10;
    }
    else if (y == "7"){
        sub = 13;
    }
    else if (y == "8"){
        sub = 3;
    }
    else if (y == "9"){
        sub = 14;
    }
    else if (y == "10"){
        sub = 15;
    }
    else if (y == "11"){
        sub = 8;
    }
    else if (y == "12"){
        sub = 4;
    }
    else if (y == "13"){
        sub = 7;
    }
    else if (y == "14"){
        sub = 1;
    }
    else if (y == "15"){
        sub = 2;
    }
    
    uberzahl result = "0";
    result = result + sub;
    
    return result;
    
}


std::vector<uberzahl> genRoundkeys( uberzahl k );
std::vector<uberzahl> genRoundkeys( uberzahl k ){
    
    uberzahl modNum = "1208925819614629174706176";
    
    uberzahl genRoundKey = k%modNum;  // mod(2^80) to get an 80 bit key
    
    std::vector<uberzahl> roundKeyReg;
    
    for( int i = 1; i < 33; i++ ){
        
        uberzahl y = genRoundKey;    //set K_i = to last 64 bits of genKey by shifting right 16
        y = y >> 16;                //this takes off the bits 0-15 leaving only 16-63
        
        roundKeyReg.push_back(y);
        
        //shift layer
        uberzahl rotateL61 = genRoundKey.rotateLeft(61,79,0);       //this will shift this 61 bits to the left
        
        //sBox layer
        uberzahl sBoxWord = rotateL61.extract(79, 76);          //puts bits 79-76 into the s-box
        sBoxWord = sBoxWord >> 76;
        uberzahl sBoxResult = genKeysSBoxenc(sBoxWord);
        sBoxResult = sBoxResult << 76;
        
        //xor with round_counter layer
        uberzahl roundCountTemp = rotateL61.extract(19,15);
        roundCountTemp = roundCountTemp >> 15;
        roundCountTemp = roundCountTemp ^ i;         //bits 19-15 xor-ed with round counter
        roundCountTemp = roundCountTemp << 15;
        
        //add the different chunks together to make new genRoundKey value
        uberzahl genKeyTemp1 = rotateL61.extract(75,20);
        uberzahl genKeyTemp2 = rotateL61.extract(14,0);
        ////cout << "sBoxResult " << sBoxResult << endl;
        ////cout << "genKeytempt " << genKeyTemp1 <<endl;
        ////cout << "roundCountTemp " << roundCountTemp << endl;
        ////cout << "genKeyTemp2 " << genKeyTemp2 <<endl;
        genRoundKey = sBoxResult + genKeyTemp1 + roundCountTemp + genKeyTemp2;
        //cout << "genRoundKey after round " << i << ": " << genRoundKey << endl;
        
        
    }
    //cout << "-----------------------------------------" << endl;
    
    return roundKeyReg;
    
}


uberzahl sBoxlayer(uberzahl x){
    int xyz = 0;
    uberzahl result(xyz);
    int sub;
    
    for(int i = 15; i>=0; --i) {
        uberzahl y = (x >> (i*4)) & 15;
        ////cout << y << endl;
        //uberzahl temp = x.extract(i*4+3, i*4);
        if (y == xyz){
            sub = 12;
        }
        else if (y == 1){
            sub = 5;
        }
        else if (y == 2){
            sub = 6;
        }
        else if (y == 3){
            sub = 11;
        }
        else if (y == 4){
            sub = 9;
        }
        else if (y == 5){
            sub = 0;
        }
        else if (y == 6){
            sub = 10;
        }
        else if (y == 7){
            sub = 13;
        }
        else if (y == 8){
            sub = 3;
        }
        else if (y == 9){
            sub = 14;
        }
        else if (y == 10){
            sub = 15;
        }
        else if (y == 11){
            sub = 8;
        }
        else if (y == 12){
            sub = 4;
        }
        else if (y == 13){
            sub = 7;
        }
        else if (y == 14){
            sub = 1;
        }
        else if (y == 15){
            sub = 2;
        }
        //sub << i*4;
        result =  result << 4;
        result = result + sub;
        ////cout << "current state in Sbox " << result << endl;
    }
    return result;
}

uberzahl sBoxlayerDec(uberzahl x){
    int xyz = 0;
    uberzahl result(xyz);
    int sub;
    for(int i = 15; i>=0; --i) {
        uberzahl y = (x >> (i*4)) & 15;
        ////cout << y << endl;
        //uberzahl temp = x.extract(i*4 + 3,i*4);
        //temp >> i*4;
        //y = temp;
        if (y == 12){
            sub = 0;
        }
        else if (y == 5){
            sub = 1;
        }
        else if (y == 6){
            sub = 2;
        }
        else if (y == 11){
            sub = 3;
        }
        else if (y == 9){
            sub = 4;
        }
        else if (y == xyz){
            sub = 5;
        }
        else if (y == 10){
            sub = 6;
        }
        else if (y == 13){
            sub = 7;
        }
        else if (y == 3){
            sub = 8;
        }
        else if (y == 14){
            sub = 9;
        }
        else if (y == 15){
            sub = 10;
        }
        else if (y == 8){
            sub = 11;
        }
        else if (y == 4){
            sub = 12;
        }
        else if (y == 7){
            sub = 13;
        }
        else if (y == 1){
            sub = 14;
        }
        else if (y == 2){
            sub = 15;
        }
        //sub << i*4;
        result = result << 4;
        result = result + sub;
        // //cout << "current state in de-Sbox " << result << endl;
        
    }
    return result;
}

int PLayerhelper(int i)
{
    int p = i % 4;
    return ((i/4)+(16*p));
}

uberzahl PLayerhelper2(uberzahl ub, int p, int original)
{
    uberzahl result1(1);
    if (ub.bit(original))
    {
        for (int t = 1; t <= p; t++)
        {
            result1 = result1 * 2;
        }
        uberzahl result_true(result1);
        return result_true;
    }
    else
    {
        int x = 0;
        uberzahl result_false(x);
        return result_false;
    }
}

uberzahl PLayer(uberzahl ub)
{
    int x = 0;
    uberzahl result(x);
    for (int i = 0; i < 64; i++)
    {
        int p = PLayerhelper(i);
        result = PLayerhelper2(ub, p, i) + result;
    }
    return result;
}

int reversePLayerhelper(int i)
{
    int m = 0;
    if (0 <= i && i <= 15)
    {
        return i * 4;
    }
    if (16 <= i && i <= 31)
    {
        m = i - 16;
        return i / 16 + m * 4;
    }
    if (32 <= i && i <= 47)
    {
        m = i - 16*2;
        return i / 16 + m * 4;
    }
    if (48 <= i && i <= 63)
    {
        m = i - 16*3;
        return i / 16 + m * 4;
    }
    
}

uberzahl PLayerReverse(uberzahl ub)
{
    int x = 0;
    uberzahl result(x);
    for (int i = 0; i < 64; i++)
    {
        int p = reversePLayerhelper(i);
        result = PLayerhelper2(ub, p, i) + result;
    }
    return result;
}

vector<uberzahl> encrypt(vector<uberzahl> &state, vector<uberzahl> &key){
    for(int k = 0; k < state.size(); k++){
        vector<uberzahl> keys = genRoundkeys(key[k]);
        for(int i = 1; i < 32; i++){
            addRoundKey(state[k],keys[i-1]);
            ////cout << "key after AddRoundKey state " << i << ": " << keys[i-1] << endl;
            //cout << endl;
            //cout << "ciphertext after AddRoundKey round " << i << ": " << state[k] << endl;
            state[k] = sBoxlayer(state[k]);
            //cout << "ciphertext after sbox round " << i << ": " << state[k] << endl;
            state[k] = PLayer(state[k]);
            //cout << "ciphertext after pLayer round " << i << ": " << state[k] << endl;
            //cout << "-----------------------------------------";
            //cout << endl;
        }
        addRoundKey(state[k],keys[31]);
        //cout << "final ciphertext after last addRoundkey: " << state[k] << endl;
    }
    return(state);
}

void decrypt(vector<uberzahl> &state, vector<uberzahl> &key){
    //cout << endl << endl << endl << endl << endl;
    //cout << "Decryption starting...initial plaintext is: " << state[0] << endl;
    for(int k = 0; k < state.size(); k++){
        vector<uberzahl> keys = genRoundkeys(key[k]);
        addRoundKey(state[k],keys[31]);
        ////cout << "ciphertext after AddRoundKey round 31" << ": " << state[k] << endl;
        ////cout << "-----------------------------------------";
        ////cout << endl;
        for(int i = 30; i >= 0 ; i--){
            state[k] = PLayerReverse(state[k]);
            //cout << "ciphertext after sbox round " << i << ": " << state[k] << endl;
            state[k] = sBoxlayerDec(state[k]);
            //cout << "ciphertext after pLayer round " << i << ": " << state[k] << endl;
            addRoundKey(state[k],keys[i]);
            ////cout << "ciphertext after AddRoundKey round " << i << ": " << state[k] << endl;
            ////cout << "-----------------------------------------";
            ////cout << endl;
        }
    }
    //cout << "decrypted ciphertext reveals the plaintext is: " << state[0] << endl;
}

int main( void ){
    //rand(1);
    uberzahl key1 = random();
    uberzahl key2 = random();
    uberzahl key3 = random();
    uberzahl key4 = random();
    uberzahl key5 = random();
    uberzahl state1 = "123456789012345678";
    uberzahl state2 = "1234234";
    uberzahl state3 = "303339458384840";
    uberzahl state4 = "487647758593958";
    uberzahl state5 = "45883";
    //cout << "-------TEST VECTOR A-------" << endl;
    //cout << "Key: " << key4 << endl;
    //cout << "Plaintext " << state5 << endl;
    vector<uberzahl> state; state.push_back(state1); //state.push_back(state2); state.push_back(state3); state.push_back(state4); state.push_back(state5);
    vector<uberzahl> key; key.push_back(key1); //key.push_back(key2); key.push_back(key1); key.push_back(key2); key.push_back(key2);
    ////cout << "Initial Plaintext: " << state[0] << endl;// << " state2 " << state[1] << endl;
    //clock_t start = clock();
    encrypt(state,key);
    //cout << "encrypted plaintext has now become ciphertext: " << state[0] << endl;// << " state2 " << state[1] << endl;
    //clock_t end = clock();
    decrypt(state,key);
    ////cout << "state1 " << state[0] << endl << endl;//" state2 " << state[1] << endl;

    ////cout << "It took " << (end-start)*1000/CLOCKS_PER_SEC << " milliseconds" << endl;*/
}
