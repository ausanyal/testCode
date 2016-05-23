#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define SIDES 6
#define R_SIDE (rand() % SIDES + 1)

int main()
{
    int n, j;
    int d1, d2;

    srand(clock());

    printf("Number attempts (%d): ", (2 * SIDES) + 5);
    scanf("%d", &n);
    
    int outcomes[15];
    for (int i = 0 ; i < 15; i++) {
        outcomes[i] = 0;
    }

    for (j = 0 ; j < n ; j++) {
        int idx = ((d1 = R_SIDE) + (d2 = R_SIDE));
        outcomes[idx]++;
    }

    printf("Probablity\n");
    double p, temp;
    for ( int k = 2 ; k < (2 * SIDES) + 1; k++) {
        temp = outcomes[k];
        p = temp/n;
        printf("outcomes[%d] = %d ; p = %.1f \n", k, outcomes[k], p);
    }
    return 0;
}

