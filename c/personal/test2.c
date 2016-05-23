#include <stdio.h>
#include <stdlib.h>

int *j;

void get_info(int **i)
{
    *i = j;
}

int main()
{
    int *i;

    j = (int *)malloc(sizeof(int));
    *j = 100;
    get_info(&i);
    printf("%d \n", *i);

    free(j);

    return 1;
}
