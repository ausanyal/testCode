#include <stdio.h>

int main()
{
    int num, n, c = 0, c2 = 0, flag = 0;
    printf("Enter num: ");
    scanf("%d", &num);

    n = num;

    if (n & (n-1)) {
        printf("not power of 2 \n");
    }
    else {
        flag = 1;
        printf("power of 2 \n");
    }

    while(n) {
        c += (n & 0x01);
        c2++;
        n >>= 1;
    }

    printf("c = %d c2 = %d \n", c, c2);

    if (!flag) {
        num = 1 << c2;
    }

    printf("closest power of 2 = %d \n", num);

    return 0;
}
