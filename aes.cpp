# include <iostream>
# include <string>
# include <vector>
# include "State.h"


using namespace std;

char state[4][4];
string cipherText;

// Initializes state
void initState()
{
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
            state[i][j] = 0;
    }
}



int main()
{
    // initialize state object, empty
    initState();

    string plainText;
    cout << "Enter text to encrypt: " << endl;

    cin >> plainText;

    char c = 'B';

    int e = c;

    cout << e << endl;

    return 0;

}
