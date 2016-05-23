#include <stdio.h>

unsigned long fast_pow(int base, int pow)
{
    unsigned long ret;

    if(pow == 1) {
        return base;
    }

    if (!(pow%2)) {
        return (fast_pow(base, pow/2) * fast_pow(base, pow/2));
    }

    return  (base * fast_pow(base, pow-1));

}

int main()
{
    int pow = 5, base = 20;
    
    printf("%lu\n", fast_pow(base, pow));

    return 0;
}
