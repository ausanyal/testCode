#include <stdio.h>

int main()
{
    int i = 0;
    int arr[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    while ( i < 10 ) {
        printf("i = %d -> %d \n", i, arr[i++]);
    }

    return 1;
}
