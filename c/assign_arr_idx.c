#include <stdio.h>

int main()
{
    int arr[5] = {
        [0] = 1,
        [2] = 2,
        [1] = 1,
    };
    printf("%d %d \n", arr[1], arr[2]);

    printf("%lu \n", sizeof(arr)/sizeof(arr[0]));

    return 1;
}
