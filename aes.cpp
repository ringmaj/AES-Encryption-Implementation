#include "State.h"
#include <iostream>
#include  <iomanip>
#include <string>
#include <vector>

using namespace std;

unsigned char state[4][4];
unsigned char key[4][4];
unsigned char sBox[16][16];
vector<vector<unsigned char>> roundKeys;
vector<unsigned char> CipherResult;

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

unsigned char eTable[16][16] =
{
    {0x01, 0x03, 0x05, 0x0F, 0x11, 0x33, 0x55, 0xFF, 0x1A, 0x2E, 0x72, 0x96, 0xA1, 0xF8, 0x13, 0x35},
    {0x5F, 0xE1, 0x38, 0x48, 0xD8, 0x73, 0x95, 0xA4, 0xF7, 0x02, 0x06, 0x0A, 0x1E, 0x22, 0x66, 0xAA},
    {0xE5, 0x34, 0x5C, 0xE4, 0x37, 0x59, 0xEB, 0x26, 0x6A, 0xBE, 0xD9, 0x70, 0x90, 0xAB, 0xE6, 0x31},
    {0x53, 0xF5, 0x04, 0x0C, 0x14, 0x3C, 0x44, 0xCC, 0x4F, 0xD1, 0x68, 0xB8, 0xD3, 0x6E, 0xB2, 0xCD},
    {0x4C, 0xD4, 0x67, 0xA9, 0xE0, 0x3B, 0x4D, 0xD7, 0x62, 0xA6, 0xF1, 0x08, 0x18, 0x28, 0x78, 0x88},
    {0x83, 0x9E, 0xB9, 0xD0, 0x6B, 0xBD, 0xDC, 0x7F, 0x81, 0x98, 0xB3, 0xCE, 0x49, 0xDB, 0x76, 0x9A},
    {0xB5, 0xC4, 0x57, 0xF9, 0x10, 0x30, 0x50, 0xF0, 0x0B, 0x1D, 0x27, 0x69, 0xBB, 0xD6, 0x61, 0xA3},
    {0xFE, 0x19, 0x2B, 0x7D, 0x87, 0x92, 0xAD, 0xEC, 0x2F, 0x71, 0x93, 0xAE, 0xE9, 0x20, 0x60, 0xA0},
    {0xFB, 0x16, 0x3A, 0x4E, 0xD2, 0x6D, 0xB7, 0xC2, 0x5D, 0xE7, 0x32, 0x56, 0xFA, 0x15, 0x3F, 0x41},
    {0xC3, 0x5E, 0xE2, 0x3D, 0x47, 0xC9, 0x40, 0xC0, 0x5B, 0xED, 0x2C, 0x74, 0x9C, 0xBF, 0xDA, 0x75},
    {0x9F, 0xBA, 0xD5, 0x64, 0xAC, 0xEF, 0x2A, 0x7E, 0x82, 0x9D, 0xBC, 0xDF, 0x7A, 0x8E, 0x89, 0x80},
    {0x9B, 0xB6, 0xC1, 0x58, 0xE8, 0x23, 0x65, 0xAF, 0xEA, 0x25, 0x6F, 0xB1, 0xC8, 0x43, 0xC5, 0x54},
    {0xFC, 0x1F, 0x21, 0x63, 0xA5, 0xF4, 0x07, 0x09, 0x1B, 0x2D, 0x77, 0x99, 0xB0, 0xCB, 0x46, 0xCA},
    {0x45, 0xCF, 0x4A, 0xDE, 0x79, 0x8B, 0x86, 0x91, 0xA8, 0xE3, 0x3E, 0x42, 0xC6, 0x51, 0xF3, 0x0E},
    {0x12, 0x36, 0x5A, 0xEE, 0x29, 0x7B, 0x8D, 0x8C, 0x8F, 0x8A, 0x85, 0x94, 0xA7, 0xF2, 0x0D, 0x17},
    {0x39, 0x4B, 0xDD, 0x7C, 0x84, 0x97, 0xA2, 0xFD, 0x1C, 0x24, 0x6C, 0xB4, 0xC7, 0x52, 0xF6, 0x01}

};

//no value for lTable[0][0], left as just 00
unsigned char lTable[16][16] = {
    {0x00, 0x00, 0x19, 0x01, 0x32, 0x02, 0x1A, 0xC6, 0x4B, 0xC7, 0x1B, 0x68, 0x33, 0xEE, 0xDF, 0x03},
    {0x64, 0x04, 0xE0, 0x0E, 0x34, 0x8D, 0x81, 0xEF, 0x4C, 0x71, 0x08, 0xC8, 0xF8, 0x69, 0x1C, 0xC1},
    {0x7D, 0xC2, 0x1D, 0xB5, 0xF9, 0xB9, 0x27, 0x6A, 0x4D, 0xE4, 0xA6, 0x72, 0x9A, 0xC9, 0x09, 0x78},
    {0x65, 0x2F, 0x8A, 0x05, 0x21, 0x0F, 0xE1, 0x24, 0x12, 0xF0, 0x82, 0x45, 0x35, 0x93, 0xDA, 0x8E},
    {0x96, 0x8F, 0xDB, 0xBD, 0x36, 0xD0, 0xCE, 0x94, 0x13, 0x5C, 0xD2, 0xF1, 0x40, 0x46, 0x83, 0x38},
    {0x66, 0xDD, 0xFD, 0x30, 0xBF, 0x06, 0x8B, 0x62, 0xB3, 0x25, 0xE2, 0x98, 0x22, 0x88, 0x91, 0x10},
    {0x7E, 0x6E, 0x48, 0xC3, 0xA3, 0xB6, 0x1E, 0x42, 0x3A, 0x6B, 0x28, 0x54, 0xFA, 0x85, 0x3D, 0xBA},
    {0x2B, 0x79, 0x0A, 0x15, 0x9B, 0x9F, 0x5E, 0xCA, 0x4E, 0xD4, 0xAC, 0xE5, 0xF3, 0x73, 0xA7, 0x57},
    {0xAF, 0x58, 0xA8, 0x50, 0xF4, 0xEA, 0xD6, 0x74, 0x4F, 0xAE, 0xE9, 0xD5, 0xE7, 0xE6, 0xAD, 0xE8},
    {0x2C, 0xD7, 0x75, 0x7A, 0xEB, 0x16, 0x0B, 0xF5, 0x59, 0xCB, 0x5F, 0xB0, 0x9C, 0xA9, 0x51, 0xA0},
    {0x7F, 0x0C, 0xF6, 0x6F, 0x17, 0xC4, 0x49, 0xEC, 0xD8, 0x43, 0x1F, 0x2D, 0xA4, 0x76, 0x7B, 0xB7},
    {0xCC, 0xBB, 0x3E, 0x5A, 0xFB, 0x60, 0xB1, 0x86, 0x3B, 0x52, 0xA1, 0x6C, 0xAA, 0x55, 0x29, 0x9D},
    {0x97, 0xB2, 0x87, 0x90, 0x61, 0xBE, 0xDC, 0xFC, 0xBC, 0x95, 0xCF, 0xCD, 0x37, 0x3F, 0x5B, 0xD1},
    {0x53, 0x39, 0x84, 0x3C, 0x41, 0xA2, 0x6D, 0x47, 0x14, 0x2A, 0x9E, 0x5D, 0x56, 0xF2, 0xD3, 0xAB},
    {0x44, 0x11, 0x92, 0xD9, 0x23, 0x20, 0x2E, 0x89, 0xB4, 0x7C, 0xB8, 0x26, 0x77, 0x99, 0xE3, 0xA5},
    {0x67, 0x4A, 0xED, 0xDE, 0xC5, 0x31, 0xFE, 0x18, 0x0D, 0x63, 0x8C, 0x80, 0xC0, 0xF7, 0x70, 0x07}
};

unsigned char fixedMatrix[4][4]  =
{
    {0x02, 0x03, 0x01, 0x01},
    {0x01, 0x02, 0x03, 0x01},
    {0x01, 0x01, 0x02, 0x03},
    {0x03, 0x01, 0x01, 0x02}
};


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
void makeState(string &plainText) {

  int index = 0;
  char null = 20;
  int size = plainText.size();

  // pad the plainText to fit 16 characters
   if(plainText.size() < 16)
   {
       for(int i = 0; i < (16 - size); i++)
           plainText += '.';
   }

   cout << "plaintext: " << plainText << endl;

  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {

      if(index >= plainText.size())
        state[j][i] = '.';
      else
        state[j][i] = plainText[index];

      index++;
    }
  }

  // Now remove the first 16 characters since we've already used them

  cout << "size: " << plainText.size() << endl;


    plainText = plainText.substr(16, string::npos);



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
  vector<unsigned char> g;
  vector<unsigned char> keyTuple;

  int r;
  int c;
  int round = 1;
  unsigned char roundConstant;
  unsigned char tempChar;

  while (numRoundKeys <= 10) {

    // find g(w[i-1])
    // 1. circular byte shift left of g(w[i-1])

    index = 3 + (4 * row);
    g.push_back(roundKeys[index][1]);
    g.push_back(roundKeys[index][2]);
    g.push_back(roundKeys[index][3]);
    g.push_back(roundKeys[index][0]);

    // 2. Byte substitution of g(w[i-1])

    cout << dec << "w[" << index << "]  = (" << hex << (int)g[0] << ", " << hex
         << (int)g[1] << ", " << hex << (int)g[2] << ", " << hex << (int)g[3]
         << ")" << endl;

    for (int j = 0; j < 4; j++) {
      // find column for substitution, get lower 4 bits
      c = g[j] & 0x0f;
      // find row for substitution, get upper 4 bits
      r = g[j] >> 4;
      g[j] = sBox[r][c];
      cout << "Substituting: " << hex << (int)roundKeys[index][j] << " -> "
           << hex << (int)g[j] << endl;
    }

    cout << hex << (int)g[0] << " " << hex << (int)g[1] << " " << hex
         << (int)g[2] << " " << hex << (int)g[3] << endl;
    cout << endl;

    // 3. Add round constant

    switch (round) {
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
    for (int j = 0; j < 4; j++) {
      tempChar = roundKeys[i - 4][j] ^ g[j];
      keyTuple.push_back(tempChar);
    }

    // add key to roundkeys
    roundKeys.push_back(keyTuple);
    keyTuple.clear();

    // find next 3 subkeys w[x] for row
    for (int j = 1; j < 4; j++) {
      int keyIndex = index + j;

      for (int k = 0; k < 4; k++) {
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


void printState(int round, string stage) {

  cout << endl;
  cout << dec << stage << " Round: " << round << endl;
  cout << "________________" << endl;
  for (int i = 0; i < 4; i++) {
    cout << "| ";
    for (int j = 0; j < 4; j++) {
      cout << setfill('0') << setw(2) << hex << (int)state[i][j] << "  ";
    }
    cout << endl;
  }
  cout << endl;
}

void printCipher(int round) {

 cout << dec << "Cipher after Round " << round << ": ";

  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      cout << setfill('0') << setw(2) << hex << (int)state[j][i] << "  ";
    }
  }
  cout << endl;
}

void getCipher()
{
    for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 4; j++) {
           CipherResult.push_back(state[j][i]);
      }
    }
}

void printResultCipher(string plainText, string key)
{
    int newLine = 15;
    cout << "Plaintext: " << plainText << endl;
    cout << "Key (128): " << key << endl;
    cout << "Final Cipher: ";
    for(int i = 0; i < CipherResult.size(); i++){
        cout << setfill('0') << setw(2) << hex << (int)CipherResult[i] << " ";

        if(i == newLine)
        {
            cout << endl;
            cout << "              ";
            newLine += 16;
        }

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

  cout << dec << "Number of sub-keys: " << roundKeys.size() << endl;
  int round = 0;
  for (int i = 0; i < roundKeys.size(); i = i + 4) {

    cout << dec << "Round " << round << ": ";

    for (int j = 0; j < 4; j++)
      cout << setfill('0') << setw(2) << hex << (int)roundKeys[i][j] << " ";
    for (int j = 0; j < 4; j++)
      cout << setfill('0') << setw(2) << hex << (int)roundKeys[i + 1][j] << " ";
    for (int j = 0; j < 4; j++)
      cout << setfill('0') << setw(2) << hex << (int)roundKeys[i + 2][j] << " ";
    for (int j = 0; j < 4; j++)
      cout << setfill('0') << setw(2) << hex << (int)roundKeys[i + 3][j] << " ";

    cout << endl;
    round++;
  }
  cout << endl;
  cout << endl;
}

void substituteBytes(){

    int r;
    int c;

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            // find column for substitution, get lower 4 bits
            c = state[i][j] & 0x0f;
            // find row for substitution, get upper 4 bits
            r = state[i][j] >> 4;

            state[i][j] = sBox[r][c];

        }
    }
}

void addRoundKey(int round)
{
    int r = round * 4;

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            state[j][i] = state[j][i] ^ roundKeys[r][j];
        }
        r++;
    }
}

void shiftRows()
{
    vector<unsigned char> stateTuple;
    int row = 1;

    // second row, left circular shift by 1
    stateTuple.push_back(state[row][1]);
    stateTuple.push_back(state[row][2]);
    stateTuple.push_back(state[row][3]);
    stateTuple.push_back(state[row][0]);

    for (int i = 0; i < 4; i++)
    {
        state[row][i] = stateTuple[i];
    }

    // second row, left circular shift by 2
    row++;
    stateTuple.clear();
    stateTuple.push_back(state[row][2]);
    stateTuple.push_back(state[row][3]);
    stateTuple.push_back(state[row][0]);
    stateTuple.push_back(state[row][1]);

    for (int i = 0; i < 4; i++)
    {
        state[row][i] = stateTuple[i];
    }


    // third row, left circular shift by 3
    row++;
    stateTuple.clear();
    stateTuple.push_back(state[row][3]);
    stateTuple.push_back(state[row][0]);
    stateTuple.push_back(state[row][1]);
    stateTuple.push_back(state[row][2]);

    for (int i = 0; i < 4; i++)
    {
        state[row][i] = stateTuple[i];
    }

}

void mixColumns()
{
    unsigned char l1,l2,l3,l4;
    unsigned char result;
    unsigned char midResult;
    bool lTableZero = false;
    int r;
    int c;

    // make a copy of the state matrix so it's not changed will being solved

    unsigned char copyState[4][4];

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            copyState[i][j] = state[i][j];
        }
    }




    for (int i = 0; i < 4; i++)
    {


        for (int j = 0; j < 4; j++)
        {

            //--------------------------------------------------------------

            //y corresponds to fixed matrix row number
            int y = 0;
            result = 0x00;


            for (int k = 0; k < 4; k++)
            {

                lTableZero = false;
                //state[j][i] * fixedMatrix[i][j];

                // find row and column numbers
                c = copyState[y][i] & 0x0f;
                r = copyState[y][i] >> 4;
                //cout << "rc = " << r << "," << c << endl;
                if(r == 0 && c == 0)
                    lTableZero = true;
                l1 = lTable[r][c];


                c = fixedMatrix[j][y] & 0x0f;
                r = fixedMatrix[j][y] >> 4;
                //cout << "rc = " << r << "," << c << endl;
                if(r == 0 && c == 0)
                    lTableZero = true;
                l2 = lTable[r][c];

                //cout << hex << (int)copyState[y][i] << " * " << (int)fixedMatrix[j][y] << endl;

                midResult = l1 + l2;

                if(l1 + l2 > 255)
                {
                    //cout <<dec << int(l1) <<" + " <<(int)l2 << " = " << l1 + l2 << endl;
                    midResult = (l1 + l2) - 0xFF;
                }

                //cout << "GREATER THAN 255: " << dec << (int)midResult << endl;

                //E(midResult) XOR E(midResult2) ...

                c = midResult & 0x0f;
                r = midResult >> 4;

                if(lTableZero == false)
                result = result ^ eTable[r][c];


                y++;
            }

            state[j][i] = result;
            //if(result > 255)

            result = 0x00;
            //cout<<endl;


        }



    }


}

void line() {
    cout << "\033[1;31m--------------------------------------------------------------------------------\033[0m" << endl;
}

void AES(){

    int round = 0;

    line();
    cout << dec << "\033[1;31mAES Round: \033[0m"<< round << endl;

    //round 0, add roundkey to state matrix
    addRoundKey(0);
    printState(0, "Add Roundkey");
    line();
    cout << endl;
    cout << endl;
    round++;

    // for rounds 1-10
    while(round <= 10)
    {
        cout << dec << "\033[1;31mAES Round: \033[0m"<< round << endl;
        substituteBytes();
        printState(round, "Substitute Bytes");
        shiftRows();
        printState(round, "Shift Rows");

        if(round <10){
            mixColumns();
            printState(round, "Mix Column");
        }

        addRoundKey(round);
        printState(round, "Add Roundkey");;
        printCipher(round);
        cout << endl;
        line();
        cout << endl;
        cout << endl;
        round++;


    }

    // Pushes final cipher into CipherResult vector
    getCipher();

}


//https://stackoverflow.com/questions/3381614/c-convert-string-to-hexadecimal-and-vice-versa
std::string hex_to_string(const std::string& in) {
    std::string output;

    if ((in.length() % 2) != 0) {
        throw std::runtime_error("String is not valid length ...");
    }

    size_t cnt = in.length() / 2;

    for (size_t i = 0; cnt > i; ++i) {
        uint32_t s = 0;
        std::stringstream ss;
        ss << std::hex << in.substr(i * 2, 2);
        ss >> s;

        output.push_back(static_cast<unsigned char>(s));
    }

    return output;
}

int main() {

  string plainText;
  string originalPlainText;
  string key;
  string stringOptions;
  // initialize state and key objects
  initState();

  // initialize sBox
  initSBox();

  cout << "\033[1;31mPress 1 for string input, 2 for hex\033[0m\n";
  getline(cin, stringOptions);

  // Enter inputs
  cout << "\033[1;31mEnter text to encrypt\033[0m\n";
  getline(cin, plainText);
  cout << "\033[1;31mEnter 128-bit encryption key\033[0m\n";
  getline(cin, key);

  if(stringOptions == "2")
  {
      plainText = hex_to_string(plainText);
      key = hex_to_string(key);

      cout << plainText << endl;
      cout << key << endl;
  }


  // Save the original plaintext for output later
  originalPlainText = plainText;
  cout<< endl;



  // Make key structure, also expand round keys
  makeKey(key);
  makeRoundKeys();
  printRountKeys();





  while(plainText.size() > 0)
  {
      // build state structure from plaintext
      makeState(plainText);

      // run AES on text segment
      AES();
  }



  printResultCipher(originalPlainText,key);



  return 0;
}
