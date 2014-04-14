#include "speck.h"

Speck::Speck() {
}

// Returns number trimmed to the number of least significant bits specified
uberzahl trimmedNum(uberzahl num, int bits) {
  return num & ( (uberzahl("1") << bits) - 1);
}

// Generates and returns random 128-bit key
uberzahl Speck::genKey() {
	srand(time(NULL));
	uberzahl tempKey;
	tempKey.setBit(0);
	for(int i = 1; i < KEYSIZE; ++i) {
		tempKey = (tempKey << 1) | (rand() % 2);
	}
	return tempKey;
}

// Sets key used for encryption and decryption
// Performs key expansion and stores results for future use
void Speck::setKey(uberzahl userKey) {
  if (userKey != key) {
    key = userKey;

    uberzahl leftKeyWord = key >> WORDSIZE;
    uberzahl rightKeyWord = trimmedNum(key, WORDSIZE);
  
    // Shift keywords and store for later
    for(int i = 0; i < NUMROUNDS; i++){
      rounds[i] = rightKeyWord;
      expand(leftKeyWord, rightKeyWord, uberzahl(i));
    }
  }
}

// Key/text expansion function used every round; modifies x and y
void Speck::expand(uberzahl &x, uberzahl &y, uberzahl k){
	x = x.rotateRight(ALPHA, 0, WORDSIZE - 1);	
	x = x + y;
	//x = trimmedNum(x, WORDSIZE);
	x = x ^ k;
	x = trimmedNum(x, WORDSIZE);
	y = y.rotateLeft(BETA, 0, WORDSIZE - 1);
	y = y ^ x;	
	y = trimmedNum(y, WORDSIZE);
}

// Inverse of key/text expansion function used every round; modifies x and y
void Speck::contract(uberzahl &x, uberzahl &y, uberzahl k){
	y = y ^ x;	
	y = trimmedNum(y, WORDSIZE);
	y = y.rotateRight(BETA, 0, WORDSIZE - 1);
	x = x ^ k;
	x = trimmedNum(x, WORDSIZE);
	if (x<y) {
	  x = (uberzahl("1") << WORDSIZE) - (y-x);
	} else {
  	x = x - y;
  }
	x = x.rotateLeft(ALPHA, 0, WORDSIZE - 1);
}

// Encrypts given plaintext using existing key; returns cipher text
uberzahl Speck::encrypt(uberzahl plaintext) {
  uberzahl left = plaintext >> WORDSIZE;
  uberzahl right = trimmedNum(plaintext, WORDSIZE);
  
  for(int i=0; i <NUMROUNDS; i++){
    expand(left, right, rounds[i]);
  }
  
  return (left << WORDSIZE) | right;
}


// Decrypts given ciphertext using existing key; returns plaintext
uberzahl Speck::decrypt(uberzahl ciphertext) {
  uberzahl left = ciphertext >> WORDSIZE;
  uberzahl right = trimmedNum(ciphertext, WORDSIZE);
  
  for (int i=NUMROUNDS-1; i>=0; i--) {
    contract(left, right, rounds[i]);
  }
  
  return (left << WORDSIZE) + right;
}

// Sets key and performs encryption
uberzahl Speck::encrypt(uberzahl key, uberzahl plaintext) {
  setKey(key);
  return encrypt(plaintext);
}

// Sets key and performs encryption
uberzahl Speck::decrypt(uberzahl key, uberzahl ciphertext) {
  setKey(key);
  return decrypt(ciphertext);
}

/*
int main() {
  Speck speck;
  
  // PERFORMANCE TESTING CODE
  // Uncomment to run. Should take ~5 seconds total.
  uberzahl key[6*15];
  int numsPerSize = 15;
  for (int i=0; i<6; i++) {
    for (int j=0; j<numsPerSize; j++) {
      key[i*numsPerSize+j] = trimmedNum(speck.genKey(), 4<<i);
    }
  }
  
  clock_t start,end;
  
  for (int i=0; i<6; i++) {
    start = clock();
    for (int j=0; j<numsPerSize; j++) {
      speck.setKey(key[i*numsPerSize+j]);
      for (int k=0; k<numsPerSize; k++) {
        uberzahl cipher = speck.encrypt(key[i*numsPerSize+k]);
        uberzahl plain = speck.decrypt(cipher);
        if (key[i*numsPerSize+k] != plain) {
          cout << "WE DONE GOOFED" << endl;
          break;
        }
      }
    }
    end = clock();
    int average = (end-start)*1000.0/numsPerSize/numsPerSize/CLOCKS_PER_SEC;
    cout << average << " ms for " << (4<<i) << " bits"<<endl;
  }
	
  const int testVectorLen = 7;
  uberzahl key[testVectorLen], cipher[testVectorLen], plain[testVectorLen];
	
  key[0] = "20011376718272490338853433276725592320";
  cipher[0] = "221137820289473687857657110085594713368";
  plain[0] = "144062678019685919772856771483193666848";
	
  key[1] = "0";
  cipher[1] = "1000444146172915417172513437297852742";
  plain[1] = "144062678019685919772856771483193666848";
	
  key[2] = "1";
  cipher[2] = "197518563100430107023787069588039329421";
  plain[2] = "152037107215241021449068138290853736037";
	
  key[3] = "1442304682729575737805390361686";
  cipher[3] = "32096542000650209713025123876611961071";
  plain[3] = "111111111111111111111111111111111";
	
  key[4] = "12345678901234567890123456";
  cipher[4] = "215825222094735503735129327493762389553";
  plain[4] = "111111111111111111111111111111111";
	
  key[5] = "5176156079219501789779";
  cipher[5] = "84750170069882828649038465849818186684";
  plain[5] = "1189998819991197253";
	
  key[6] = "1189998819991197253";
  cipher[6] = "132247081460443199909686876827324754061";
  plain[6] = "1189998819991197253";
  
  for (int i=0; i<testVectorLen; i++) {
    uberzahl testcipher = speck.encrypt(key[i], plain[i]);
    cout << "Expected ciphertext:\t" << cipher[i] << endl 
          << "Resulting ciphertext:\t" << testcipher << endl;

    uberzahl testplain = speck.decrypt(key[i], testcipher);
    cout << "Expected plaintext:\t" << plain[i] << endl 
         << "Result plaintext:\t" << testplain << endl;
    cout << "-----------------------------------------" << endl;
    
    if (testcipher != cipher[i] || testplain != plain[i]) {
      cout << "Inconsistency detected. Aborting." << endl;
      break;
    }
  }

	return 0;
}
*/
