#include <stdio.h>

int main()
{
    int arr[] = {1, 2, 3};
    int *p = arr;

    printf("%lu %lu \n", sizeof(arr), sizeof(p));
    return 0;
}
