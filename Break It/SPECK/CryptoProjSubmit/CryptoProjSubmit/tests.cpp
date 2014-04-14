//  Team: 32 (The Muffin Men)
//  Name: Anthony Chiang, Eric Edmond, Steven Louie
//  Uniqname: aycc, eedmond, slouie

#include "speck.hpp"
#include "utility.hpp"
#include <string.h>

#include <sstream>

using namespace std;


void bassert(int result)
{
    static int test = 0;
    if(!result)
    {
        cerr << "failed on test " << test << '\n';
    }
    assert(result);
    ++test;
}

int main()
{
    ostringstream out;
    timer<nanoseconds> t1;
    {
        //0
        string test = "speck_cipher keygen", expected = "Run"; //Run is the expected first word based on unit testing scheme
        t1.reset();t1.start();
        bassert(run_test(num_words(test),string_to_argv(test),expected));
        t1.stop(); t1.accumulate();
        out << t1.get_time() << '\n';
    }

    {
        
        string test = "speck_cipher encrypt 0f0e0d0c0b0a0908 0706050403020100 6c61766975716520 7469206564616d20",
               expected = "a65d985179783265 7860fedf5c570d18";
        t1.reset();t1.start();
        bassert(run_test(num_words(test),string_to_argv(test),expected));
        t1.stop(); t1.accumulate();
        out << t1.get_time() << '\n';

    }

    {
        //2
        string test = "speck_cipher decrypt 0f0e0d0c0b0a0908 0706050403020100 a65d985179783265 7860fedf5c570d18 ",
               expected = "6c61766975716520 7469206564616d20";
        t1.reset();t1.start();
        bassert(run_test(num_words(test),string_to_argv(test),expected));
        t1.stop(); t1.accumulate();
        out << t1.get_time() << '\n';

    }


    {
        string test = "speck_cipher decrypt 0f0e0d0c0b0a0908 0706050403020100 6c61766975716520 7469206564616d20",
               expected = "a65d985179783265 7860fedf5c570d18";
        t1.reset();t1.start();
        bassert(!run_test(num_words(test),string_to_argv(test),expected));
        t1.stop(); t1.accumulate();
        out << t1.get_time() << '\n';
    }


    {
        //4
        string test = "speck_cipher encrypt 14ff6935660e42bc aec926f3928088dc 3d791d78bc67d50b 36a27ba5866bcd47",
               expected = "86976b3de309ee32 d6df752af13e3ef8";
        t1.reset();t1.start();
        bassert(run_test(num_words(test),string_to_argv(test),expected));
        t1.stop(); t1.accumulate();
        out << t1.get_time() << '\n';

    }

    {
        string test = "speck_cipher decrypt 14ff6935660e42bc aec926f3928088dc 86976b3de309ee32 d6df752af13e3ef8",
               expected = "3d791d78bc67d50b 36a27ba5866bcd47";
        t1.reset();t1.start();
        bassert(run_test(num_words(test),string_to_argv(test),expected));
        t1.stop(); t1.accumulate();
        out << t1.get_time() << '\n';

    }

    {
        //6
        string test = "speck_cipher encrypt a54b720b14098463 88c40e29c8cad69 a5c28ecd15bd934b 13a022333568e613",
               expected = "8b0981f30a68074b b281ad68ebb03096";
        t1.reset();t1.start();
        bassert(run_test(num_words(test),string_to_argv(test),expected));
        t1.stop(); t1.accumulate();
        out << t1.get_time() << '\n';

    }

    {
        string test = "speck_cipher decrypt a54b720b14098463 88c40e29c8cad69 8b0981f30a68074b b281ad68ebb03096",
               expected = "a5c28ecd15bd934b 13a022333568e613";
        t1.reset();t1.start();
        bassert(run_test(num_words(test),string_to_argv(test),expected));
        t1.stop(); t1.accumulate();
        out << t1.get_time() << '\n';
    }

    {
        //8
        string test = "speck_cipher encrypt 7b694dab586e4de2 b218a5be43990588 5c88d39f61bcdae0 5ba058471cc43cfe",
               expected = "e04c64ee675ece64 1189454bd7bc7e68";
        t1.reset();t1.start();
        bassert(run_test(num_words(test),string_to_argv(test),expected));
        t1.stop(); t1.accumulate();
        out << t1.get_time() << '\n';

    }

    {
        string test = "speck_cipher decrypt 7b694dab586e4de2 b218a5be43990588 e04c64ee675ece64 1189454bd7bc7e68",
               expected = "5c88d39f61bcdae0 5ba058471cc43cfe";
        t1.reset();t1.start();
        bassert(run_test(num_words(test),string_to_argv(test),expected));
        t1.stop(); t1.accumulate();
        out << t1.get_time() << '\n';

    }

    {
        //10
        string test = "speck_cipher encrypt 406ffcc6e422b542 924614b00865c320 529f1501fc434b08 3c763e92ebc9968e",
               expected = "7d956b2740f7877c 2ac6b334eae4516b";
        t1.reset();t1.start();
        bassert(run_test(num_words(test),string_to_argv(test),expected));
        t1.stop(); t1.accumulate();
        out << t1.get_time() << '\n';
    }

    {
        string test = "speck_cipher decrypt 406ffcc6e422b542 924614b00865c320 7d956b2740f7877c 2ac6b334eae4516b",
               expected = "529f1501fc434b08 3c763e92ebc9968e";
        t1.reset();t1.start();
        bassert(run_test(num_words(test),string_to_argv(test),expected));
        t1.stop(); t1.accumulate();
        out << t1.get_time() << '\n';
    }

    {
        //12
        string test = "speck_cipher encrypt 3d739e802dfcc53 a0e04c8547dc4458 f772dc3fc848bb0f 1c5cee29df463a8d",
               expected = "afee2288fe1b9d08 eea6da225b500837";
        t1.reset();t1.start();
        bassert(run_test(num_words(test),string_to_argv(test),expected));
        t1.stop(); t1.accumulate();
        out << t1.get_time() << '\n';

    }

    {
        string test = "speck_cipher decrypt 3d739e802dfcc53 a0e04c8547dc4458  afee2288fe1b9d08 eea6da225b500837",
               expected = "f772dc3fc848bb0f 1c5cee29df463a8d";
        t1.reset();t1.start();
        bassert(run_test(num_words(test),string_to_argv(test),expected));
        t1.stop(); t1.accumulate();
        out << t1.get_time() << '\n';
    }

    {
        //14
        string test = "speck_cipher encrypt bcfc1f24873bc860 6de102c78773e97b 3b794da5b1614d0c 60a64cdc49291774",
               expected = "d6318e0876ad385b bb9fc47def607c41";
        t1.reset();t1.start();
        bassert(run_test(num_words(test),string_to_argv(test),expected));
        t1.stop(); t1.accumulate();
        out << t1.get_time() << '\n';

    }

    {
        string test = "speck_cipher decrypt bcfc1f24873bc860 6de102c78773e97b d6318e0876ad385b bb9fc47def607c41",
               expected = "3b794da5b1614d0c 60a64cdc49291774";
        t1.reset();t1.start();
        bassert(run_test(num_words(test),string_to_argv(test),expected));
        t1.stop(); t1.accumulate();
        out << t1.get_time() << '\n';

    }

    {
        //16
        string test = "speck_cipher encrypt bcfc1f24873bc860 6de102c78773e97b d6318e0876ad385b bb9fc47def607c41",
               expected = "3b794da5b1614d0c 60a64cdc49291774";
        t1.reset();t1.start();
        bassert(!run_test(num_words(test),string_to_argv(test),expected));
        t1.stop(); t1.accumulate();
        out << t1.get_time() << '\n';

    }

    {
        string test = "speck_cipher afiasdfdsaj ijafpoasdjfpoidasjfpji",
               expected = "Run";
        t1.reset();t1.start();
        bassert(run_test(num_words(test),string_to_argv(test),expected));
        t1.stop(); t1.accumulate();
        out << t1.get_time() << '\n';

    }
    cout << "------------------------All tests passed------------------------\n";
    cout << "----------Run time in microsecond(s) of each test case-----------\n";
    cout << out.str();
    return 0;
}
