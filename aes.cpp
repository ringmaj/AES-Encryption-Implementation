#include "State.h"
#include <iostream>
#include <string>
#include <vector>

using namespace std;

char state[4][4];
char key[4][4];
unsigned char sBox[16][16];
vector<vector<unsigned char>> roundKeys;

unsigned char sArray[256] = {
    0x63, 0x7C, 0x77, 0x7B, 0xF2, 0x6B, 0x6F, 0xC5, 0x30, 0x01, 0x67, 0x2B,
    0xFE, 0xD7, 0xAB, 0x76, 0xCA, 0x82, 0xC9, 0x7D, 0xFA, 0x59, 0x47, 0xF0,
    0xAD, 0xD4, 0xA2, 0xAF, 0x9C, 0xA4, 0x72, 0xC0, 0xB7, 0xFD, 0x93, 0x26,
    0x36, 0x3F, 0xF7, 0xCC, 0x34, 0xA5, 0xE5, 0xF1, 0x71, 0xD8, 0x31, 0x15,
    0x04, 0xC7, 0x23, 0xC3, 0x18, 0x96, 0x05, 0x9A, 0x07, 0x12, 0x80, 0xE2,
    0xEB, 0x27, 0xB2, 0x75, 0x09, 0x83, 0x2C, 0x1A, 0x1B, 0x6E, 0x5A, 0xA0,
    0x52, 0x3B, 0xD6, 0xB3, 0x29, 0xE3, 0x2F, 0x84, 0x53, 0xD1, 0x00, 0xED,
    0x20, 0xFC, 0xB1, 0x5B, 0x6A, 0xCB, 0xBE, 0x39, 0x4A, 0x4C, 0x58, 0xCF,
    0xD0, 0xEF, 0xAA, 0xFB, 0x43, 0x4D, 0x33, 0x85, 0x45, 0xF9, 0x02, 0x7F,
    0x50, 0x3C, 0x9F, 0xA8, 0x51, 0xA3, 0x40, 0x8F, 0x92, 0x9D, 0x38, 0xF5,
    0xBC, 0xB6, 0xDA, 0x21, 0x10, 0xFF, 0xF3, 0xD2, 0xCD, 0x0C, 0x13, 0xEC,
    0x5F, 0x97, 0x44, 0x17, 0xC4, 0xA7, 0x7E, 0x3D, 0x64, 0x5D, 0x19, 0x73,
    0x60, 0x81, 0x4F, 0xDC, 0x22, 0x2A, 0x90, 0x88, 0x46, 0xEE, 0xB8, 0x14,
    0xDE, 0x5E, 0x0B, 0xDB, 0xE0, 0x32, 0x3A, 0x0A, 0x49, 0x06, 0x24, 0x5C,
    0xC2, 0xD3, 0xAC, 0x62, 0x91, 0x95, 0xE4, 0x79, 0xE7, 0xC8, 0x37, 0x6D,
    0x8D, 0xD5, 0x4E, 0xA9, 0x6C, 0x56, 0xF4, 0xEA, 0x65, 0x7A, 0xAE, 0x08,
    0xBA, 0x78, 0x25, 0x2E, 0x1C, 0xA6, 0xB4, 0xC6, 0xE8, 0xDD, 0x74, 0x1F,
    0x4B, 0xBD, 0x8B, 0x8A, 0x70, 0x3E, 0xB5, 0x66, 0x48, 0x03, 0xF6, 0x0E,
    0x61, 0x35, 0x57, 0xB9, 0x86, 0xC1, 0x1D, 0x9E, 0xE1, 0xF8, 0x98, 0x11,
    0x69, 0xD9, 0x8E, 0x94, 0x9B, 0x1E, 0x87, 0xE9, 0xCE, 0x55, 0x28, 0xDF,
    0x8C, 0xA1, 0x89, 0x0D, 0xBF, 0xE6, 0x42, 0x68, 0x41, 0x99, 0x2D, 0x0F,
    0xB0, 0x54, 0xBB, 0x16};

void initSBox() {
  int k = 0;

  for (int i = 0; i < 16; i++) {
    for (int j = 0; j < 16; j++) {
      sBox[i][j] = sArray[k];
      k++;
    }
  }
}

// Initializes state
void initState() {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      state[i][j] = 0;
      key[i][j] = 0;
    }
  }
}

// Adds plainText into state object
void makeState(string plainText) {

  int index = 0;
  char null = 20;

  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {

      state[j][i] = plainText[index];

      index++;
    }
  }
}

// Adds plainText into state object
void makeKey(string keyInput) {

  int index = 0;
  char null = 20;

  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {

      key[j][i] = keyInput[index];

      index++;
    }
  }
}

void makeRoundKeys() {
  // add inital keys for round 0
  vector<unsigned char> temp;
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      temp.push_back(key[j][i]);
     // cout << "pushing: " << hex << (int)temp[j]<< endl;
    }
    roundKeys.push_back(temp);
    temp.clear();
  }

  // start with 1 round key
  int numRoundKeys = 1;

  // We're trying to find w[4] now
  int i = 4;
  int row = 0;
  int index;
  vector <unsigned char> g;
  vector <unsigned char> keyTuple;

  int r;
  int c;
  int round = 1;
  unsigned char roundConstant;
  unsigned char tempChar;






  while(numRoundKeys <= 10)
  {

      // find g(w[i-1])
      // 1. circular byte shift left of g(w[i-1])

      index = 3 + (4*row);
      g.push_back(roundKeys[index][1]);
      g.push_back(roundKeys[index][2]);
      g.push_back(roundKeys[index][3]);
      g.push_back(roundKeys[index][0]);

      //2. Byte substitution of g(w[i-1])

      cout << "w["<<index<<"]  = (" << hex << (int)g[0] << ", " << hex << (int)g[1] << ", " << hex << (int)g[2] << ", " << hex << (int)g[3] << ")"<< endl;


      for(int j = 0 ; j < 4; j++)
      {
          // find column for substitution, get lower 4 bits
          c = g[j] & 0x0f;
          // find row for substitution, get upper 4 bits
          r = g[j] >> 4;
          g[j] = sBox[r][c];
          cout << "Substituting: " << hex << (int)roundKeys[index][j]<< " -> "<< hex << (int)g[j] << endl;
      }



          cout << hex << (int)g[0] << " " << hex << (int)g[1] << " " << hex << (int)g[2] << " " << hex << (int)g[3] << endl;


          //3. Add round constant

          switch (round)
          {
            case 1:
                roundConstant = 0x01;
                break;
            case 2:
                roundConstant = 0x02;
                break;
            case 3:
                roundConstant = 0x04;
                break;
            case 4:
                roundConstant = 0x08;
                break;
            case 5:
                roundConstant = 0x10;
                break;
            case 6:
                roundConstant = 0x20;
                break;
            case 7:
                roundConstant = 0x40;
                break;
            case 8:
                roundConstant = 0x80;
                break;
            case 9:
                roundConstant = 0x1B;
                break;
            case 10:
                roundConstant = 0x36;
                break;
          default:
              /* code */
              break;
          }


          // add round constant to key
          g[0] = g[0] ^ roundConstant;



          // find first subkey w[x] for row
          for(int j = 0; j < 4; j++)
          {
              tempChar = roundKeys[i-4][j] ^ g[j];
              keyTuple.push_back(tempChar);
          }

          // add key to roundkeys
          roundKeys.push_back(keyTuple);
          keyTuple.clear();


          // find next 3 subkeys w[x] for row
          for(int j = 1; j < 4; j++)
          {
              int keyIndex = index + j;

              for (int k = 0; k < 4; k++)
              {
                  tempChar = roundKeys[keyIndex][k] ^ roundKeys[keyIndex - 3][k];
                  keyTuple.push_back(tempChar);
              }

              roundKeys.push_back(keyTuple);
              keyTuple.clear();

          }


      // find next row
      i = i + 4;
      round++;
      numRoundKeys++;
      row++;
      g.clear();
  }



}

void printState() {

  cout << endl;
  cout << "     State" << endl;
  cout << "________________" << endl;
  for (int i = 0; i < 4; i++) {
    cout << "| ";
    for (int j = 0; j < 4; j++) {
      cout << hex << (int)state[i][j] << "  ";
    }
    cout << endl;
  }
  cout << endl;
}

void printSBox() {

  cout << endl;
  cout << "     S-Box" << endl;
  cout << "________________" << endl;
  for (int i = 0; i < 16; i++) {
    cout << "| ";
    for (int j = 0; j < 16; j++) {
      if ((int)sBox[i][j] < 10)
        cout << hex << (int)sBox[i][j] << "   ";
      else
        cout << hex << (int)sBox[i][j] << "  ";
    }
    cout << endl;
  }
  cout << endl;
}

void printKey() {
  cout << endl;
  cout << "     key" << endl;
  cout << "________________" << endl;
  for (int i = 0; i < 4; i++) {
    cout << "| ";
    for (int j = 0; j < 4; j++) {
      cout << hex << (int)key[i][j] << "  ";
    }
    cout << endl;
  }
  cout << endl;
}

void printRountKeys() {

  cout << "roundkeys size: " << roundKeys.size() << endl;
  int round = 0;
  for (int i = 0; i < roundKeys.size(); i = i+4) {

    cout << "Round " << round << ": ";

    for(int j = 0; j < 4; j++)
        cout << hex << (int)roundKeys[i][j] <<" ";
        for(int j = 0; j < 4; j++)
            cout << hex << (int)roundKeys[i+1][j] <<" ";
            for(int j = 0; j < 4; j++)
                cout << hex << (int)roundKeys[i+2][j] <<" ";
                for(int j = 0; j < 4; j++)
                    cout << hex << (int)roundKeys[i+3][j] <<" ";

    cout << endl;
    round++;
  }
}

int main() {

  // initialize state and key objects
  initState();

  // initialize sBox
  initSBox();

  string plainText;
  string cipherText;
  string key;

  // cout << "Enter text to encrypt: " << endl;
  cout << "\033[1;31mEnter text to encrypt\033[0m\n";
  getline(cin, plainText);

  // cout << "Enter 128 bit encryption key: " << endl;
  cout << "\033[1;31mEnter 128-bit encryption key\033[0m\n";
  getline(cin, key);

  makeState(plainText);
  makeKey(key);
   printState();
   printKey();
  // printSBox();
  makeRoundKeys();
  printRountKeys();


  return 0;
}
