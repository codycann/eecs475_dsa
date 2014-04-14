#include "simon.h"
#include <random>
#include <cstdlib>

#define RAND_NUM_BOUND 256
#define RAND_NUM_BITS 8

static uberzahl z[5] = { uberzahl("4506230155203752166"),
uberzahl("2575579794259089498"),
uberzahl("3160415496042964403"),
uberzahl("3957284701066611983"),
uberzahl("3781244162168104175") };

static bool check_nm(int n, int m, int &T, int &k);
static void error_message(int n, int m);
static vector<uberzahl> key_expend(vector<uberzahl> key, int n, int m);

vector<uberzahl> keygen(int n, int m){
	vector<uberzahl> ans;
	int T, k;
	check_nm(n, m, T, k);
	int length = n / RAND_NUM_BITS;
	uberzahl c = (uberzahl("1") << n) - uberzahl("4");
	for (int i = 0; i < m; i++){
		ans.push_back(uberzahl("0"));
		while (ans[i].bitLength() < n - 7){
			ans[i] = (ans[i] << RAND_NUM_BITS);
			ans[i] = ans[i] + uberzahl(rand() % RAND_NUM_BOUND);
		}
	}
	return ans;
}

vector<uberzahl> encrypt(vector<uberzahl> key, vector<uberzahl> plaintext){
	int n, m;
	n = key[0].bitLength() % 8 == 0 ? key[0].bitLength() : (key[0].bitLength() / 8 + 1) * 8;
	m = key.size();
	key = key_expend(key, n, m);
	uberzahl t = (uberzahl("1") << n) - 1;
	vector<uberzahl> ciphertext;
	for (int k = 0; k < plaintext.size(); k++){
		uberzahl x, y;
		y = plaintext[k] & t;
		x = (plaintext[k] >> n) & t;
		for (int i = 0; i < key.size(); i++){
			uberzahl temp = x;
			x = y ^ (x.rotateLeft(1, 0, n - 1)&x.rotateLeft(8, 0, n - 1)) ^ x.rotateLeft(2, 0, n - 1) ^ key[i];
			y = temp;
		}
		uberzahl temp = x;
		temp = (temp << n) + y;
		ciphertext.push_back(temp);
	}
	return ciphertext;
}

vector<uberzahl> decrypt(vector<uberzahl> key, vector<uberzahl> ciphertext){
	int n, m;
	n = key[0].bitLength() % 8 == 0 ? key[0].bitLength() : (key[0].bitLength() / 8 + 1) * 8;
	m = key.size();
	key = key_expend(key, n, m);
	uberzahl t = (uberzahl("1") << n) - 1;
	vector<uberzahl> plaintext;
	for (int k = 0; k < ciphertext.size(); k++){
		uberzahl x, y;
		y = ciphertext[k] & t;
		x = (ciphertext[k] >> n) & t;
		for (int i = key.size() - 1; i >= 0; i--){
			uberzahl temp = y;
			y = x ^ (y.rotateLeft(1, 0, n - 1) & y.rotateLeft(8, 0, n - 1)) ^ y.rotateLeft(2, 0, n - 1) ^ key[i];
			x = temp;
		}
		uberzahl temp = x;
		temp = (temp << n) + y;
		plaintext.push_back(temp);
	}
	return plaintext;
}

static bool check_nm(int n, int m, int &T, int &k){
	switch (n){
	case 16:
		if (m == 4){
			T = 32;
			k = 0;
		}
		else
			error_message(n, m);
		break;
	case 24:
		if (m == 3){
			T = 36;
			k = 0;
		}
		else if (m == 4){
			T = 36;
			k = 1;
		}
		else
			error_message(n, m);
		break;
	case 32:
		if (m == 3){
			T = 42;
			k = 2;
		}
		else if (m == 4){
			T = 44;
			k = 3;
		}
		else
			error_message(n, m);
		break;
	case 48:
		if (m == 2){
			T = 52;
			k = 2;
		}
		else if (m == 3){
			T = 54;
			k = 3;
		}
		else
			error_message(n, m);
		break;
	case 64:
		if (m == 2){
			T = 68;
			k = 2;
		}
		else if (m == 3){
			T = 69;
			k = 3;
		}
		else if (m == 4){
			T = 72;
			k = 4;
		}
		else
			error_message(n, m);
		break;
	defult:
		error_message(n, m);
	}
	return true;
}

static void error_message(int n, int m){
	cerr << "error: invalid input of n = "<<n<<" m = "<<m<<"\nusage:" << endl;
	cerr << "if n be 16, m must be 4" << endl;
	cerr << "if n be 24 or 32, m must be 3 or 4" << endl;
	cerr << "if n be 48, m must be 2 or 3" << endl;
	cerr << "if n be 64, m must be 2, 3 or 4" << endl;
	exit(1);
}

static vector<uberzahl> key_expend(vector<uberzahl> key, int n, int m){
	int T, k;
	check_nm(n, m, T, k);
	key.resize(m);
	uberzahl c = (uberzahl("1") << n) - uberzahl("4");
	for (int i = m; i < T; i++){
		key.push_back(uberzahl("0"));
		uberzahl temp = key[i - 1].rotateRight(3, 0, n - 1);
		if (m == 4)
			temp = temp^key[i - 3];
		temp = temp^temp.rotateRight(1, 0, n - 1);
		key[i] = key[i - m] ^ temp^c^uberzahl(z[k].bit((61-((i - m)) % 62)));
	}
	return key;
}