#include <stdio.h>

// fib = 0 1 1 2 3 5 7 12

int nfib(int n)
{
    if (n == 1 || n == 0) {
        return n;
    }

    return (nfib(n-1) + nfib(n-2));
}

int main()
{
    int a, n;

    //printf("n: ");
    //scanf("%d", &n);

    for (a = 0; a < 10 ; a++) {
        printf("%d = %d \n", a, nfib(a));
    }

    return 0;
}
