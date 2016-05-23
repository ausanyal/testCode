#include <stdio.h>

void swap_even_odd_bits(unsigned int *n)
{
    int x = *n & 0xaaaaaaaa;
    int y = *n & 0x55555555;
    int z1 = x>>1;
    int z2 = y<<1;
    printf("n: 0x%x x: 0x%x y: 0x%x z1: 0x%x z2: 0x%x \n", *n, x, y, z1, z2);
    *n = z1 | z2;
}

int
count_bits(int n)
{
    int count=0;
    while(n) {
        count++;
        n = n & (n-1);
    }

    return count;
}

// Ex: 00001001 ==> 10010000
#define GET(n, x) ((n & (1 << x )) >> x) 
#define SET(n, x) (n | (1 << x))
#define RESET(n, x) (n & (~(1 << x)))
int reverse_bits_of_byte(int n)
{
    int start = 0, end = 7, sbit, ebit, temp = n;

    while(start < end) {
        sbit = GET(n, start);
        ebit = GET(n, end);

        temp = sbit ? SET(temp, end):RESET(temp, end);
        temp = ebit ? SET(temp, start):RESET(temp, start);

        start++;
        end--;
    }

    return temp;
}

int incr_n(int n)
{

    int c = 0;

    while (c < 31) {
        if (n | (1 << c)) {
            printf("Case 1: c: %d n: 0x%x \n", c, n); 
            n = n & (~(0x1 << c));
            printf("Case 1: c: %d n: 0x%x \n", c, n); 
        } else {
            printf("Case 2: c: %d n: 0x%x \n", c, n); 
            n = n | (0x1 << c);
            printf("Case 2: c: %d n: 0x%x \n", c, n); 
            break;
        }
        c++;
    }

    return n;
}

int main()
{
    unsigned int n;
    scanf("%d", &n);

    printf("n: %d incr_n: %d \n", n, incr_n(n)); 

    int temp = reverse_bits_of_byte(n);
    printf("reversed bits n: 0x%x reversed: 0x%x \n", n, temp);

    swap_even_odd_bits(&n);
    printf("swapped: 0x%x \n", n);

    printf("bits in swapped n: %d \n", count_bits(n));

    return 0;
}
