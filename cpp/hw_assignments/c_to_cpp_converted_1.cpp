#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

const int sides=6;

inline int r_side() 
{ 
    return (rand() % sides + 1); 
}

int main()
{
    int n;

    srand(clock());

    cout << "Numer attempts: ";
    cin >> n;

    int *outcomes = new int[2 * sides + 1];

    for (int i = 0 ; i < 15 ; i++) {
        outcomes[i] = 0;
    }

    for (int j = 0; j < n; j++) {
        outcomes[r_side() + r_side()]++;
    }

    cout << "Probability \n";
    for (int j = 2; j < (2 * sides + 1); j++) {
        cout << "j = " << j << " p = " << static_cast<double>(outcomes[j])/n << endl;
    }

    return 0;
}
