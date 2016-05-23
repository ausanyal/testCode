#include <stdio.h>
#include <string.h>

int myatoi(char *str)
{
    int val = 0, len =strlen(str), is_neg = 1;


    for (int i = 0; i < len; i++ ) {
        if (i == 0 && str[i] == '-') {
            is_neg = -1;
            continue;
        }
        val *= 10;
        val += str[i] - '0';
    }

    return val * is_neg;
}

int main()
{
    printf("val: %d \n", myatoi("-123"));
    return 0;
}
