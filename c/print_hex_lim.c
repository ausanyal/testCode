#include <stdio.h>

int main()
{
    unsigned int x = 100;

    printf("0x%1x\n", x);
    x = 16384;
    printf("0x%1x\n", x);
    x = -1;
    printf("0x%1x\n", x);
    if (x == -1) { 
        printf("val same \n");
    }
    return 0;
}
