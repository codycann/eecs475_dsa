//  Team: 32 (The Muffin Men)
//  Name: Anthony Chiang, Eric Edmond, Steven Louie
//  Uniqname: aycc, eedmond, slouie

#ifndef UTILITY_HPP
#define UTILITY_HPP

#include <iostream>
#include <cassert>
#include <cstdlib>
#include <sstream>
#include <string.h>
#include <ratio>
#include <chrono>

#define BASE_16 16
#define CLASSIC_NUMARGS 6
#define KEYGEN_NUMARGS 2

using namespace std::chrono;
using namespace std;


u64 string_to_u64(const char * s);
void print_usage(char * progname, ostream & out); //Output allowed command line options
int run_test(int argc, char *argv[], string expected); //Runs a test and compares to expected output
char ** string_to_argv(string commands);
int num_words(string commands);

//Utility code to accommodate command line options
int run_speck(int argc, char *argv[], ostream & out = cout)
{
    if(argc < KEYGEN_NUMARGS)
    {
        print_usage(argv[0], out);
        return 1;
    }
    SpeckBlockCipher speck;
    if(!strcmp(argv[1],"keygen") && argc == KEYGEN_NUMARGS)
    {

        u64 kw[2] = {0};

        speck.GenerateKey();
        speck.GetKey(kw);

        out << hex;
        out << kw[1] << " " <<  kw[0] <<  endl;

    }
    else if(!strcmp(argv[1],"encrypt") && argc == CLASSIC_NUMARGS)
    {

        u64 kw[2] = {0}, pt[2] = {0}, ct[2] = {0};

        kw[1] = string_to_u64(argv[2]);
        kw[0] = string_to_u64(argv[3]);

        pt[1] = string_to_u64(argv[4]);
        pt[0] = string_to_u64(argv[5]);

        speck.AssignKey(kw);
        speck.Encrypt(pt,ct);

        out << hex;
        out << ct[1] << " " << ct[0] << endl;

    }
    else if(!strcmp(argv[1],"decrypt") && argc == CLASSIC_NUMARGS)
    {
        u64 kw[2] = {0}, pt[2] = {0}, ct[2] = {0};

        kw[1] = string_to_u64(argv[2]);
        kw[0] = string_to_u64(argv[3]);

        ct[1] = string_to_u64(argv[4]);
        ct[0] = string_to_u64(argv[5]);

        speck.AssignKey(kw);
        speck.Decrypt(pt,ct);

        out << hex;
        out << pt[1] << " " << pt[0] << endl;

    }
    else
    {
        print_usage(argv[0], out);
        return 1;
    }

    return 0;


}

u64 string_to_u64(const char * s)
{
    return strtoull(s, NULL, BASE_16);
}

void print_usage(char * progname, ostream & out)
{
    out << "Run with one of the following options\n";
    out << "Note that all arguments should be in base-16 format\n";
    out <<  "\t " << progname << " kegen \n";
    out <<  "\t " << progname << " encrypt keyword1 keyword0 plaintext1 plaintext0\n";
    out <<  "\t " << progname << " decrypt keyword1 keyword0 ciphertext1 ciphertext0\n";

    out <<  "\n";
    out <<  "Output from each of the three options:\n";
    out << "keygen  -> keyword1 keyword0\n";
    out << "encrypt -> ciphertext1 ciphertext0\n";
    out << "decrypt -> plaintext1 plaintext0\n";
}

int run_test(int argc, char *argv[], string expected)
{
    ostringstream out;

    run_speck(argc, argv, out);
    if(!strcmp(argv[1],"keygen"))
    {
        istringstream in(out.str());
        string k1, k2;
        in >> k1 >> k2;
        assert(k1.length()>= 15 && k1.length() <= 16);
        assert(k2.length()>= 15 && k2.length() <= 16);
        return 1;
    }
    else if(!strcmp(argv[1],"encrypt") || !strcmp(argv[1],"decrypt"))
    {
        string result = out.str();
        result.erase(result.length()-1);
        return result == expected;
    }
    else
    {
        istringstream in(out.str());
        string word;
        in >> word;
        return word == expected;
    }
}

char ** string_to_argv(string commands)
{
    istringstream in(commands);
    char ** argv = new char *[commands.length()+1];
    for(int i = 0; i < commands.length(); ++i)
    {
        string word;
        in >> word;
        char * s = new char[word.length()+1];

        for(int j = 0; j < word.length(); ++j)
        {
            s[j] = word[j];
        }

        s[word.length()] = '\0';
        argv[i]=s;

    }
    argv[commands.length()] = NULL;
    return argv;
}

int num_words(string commands)
{
    int n = 0;
    istringstream in(commands);
    string word;
    while(in >> word)
        ++n;
    return n;
}

//a nice high-res clock interface
template <typename T>
class timer{
public:
    timer();
    ~timer();
    void start();
    void stop();
    void accumulate();
    double get_time();
    void reset();
private:
    bool initialized;
    high_resolution_clock::time_point t1, t2;
    T total;
};

template <typename T>
timer<T>::timer(){
    initialized = false;
    total = T(0);
}

template <typename T>
timer<T>::~timer(){
}

template <typename T>
void timer<T>::start(){
    initialized = true;
    t2 = t1  = high_resolution_clock::now();
}

template <typename T>
void timer<T>::stop(){
    t2 = high_resolution_clock::now();
}

template <typename T>
void timer<T>::reset(){
    initialized = false;
    total = T(0);
}

template <typename T>
void timer<T>::accumulate(){
    total += duration_cast<T>(t2 - t1);
}

template <typename T>
double timer<T>::get_time(){
    if(!initialized)
        return 0;
    return total.count();
}


#endif