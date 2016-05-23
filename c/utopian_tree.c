#include <stdio.h>
#include <stdlib.h>

int
print_growth(int n)
{
    int init = 1;
    int growth = init;

    for (int i = 0; i < n; i++) {
        //printf("i: %d growth: %d \n", i, growth);
        growth = ((i%2) ? (growth+1) : (growth * 2)); 
    }

    return growth;
}

int main()
{
    int t, *n;

    scanf("%d", &t);

    
    n = (int *)malloc(t * sizeof(int));
    for(int i = 0; i < t; i++) {
        scanf("%d", &n[i]);
    }

    for(int i = 0; i < t; i++) {

        printf("%d\n", print_growth(n[i]));

    }
}
