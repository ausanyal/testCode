#include <stdio.h>
#include <string.h>
#include <math.h>

int
myatoi(char *s)
{
    int c = 0, idx = 0, i;
    unsigned long len = strlen(s);

    while(idx < strlen(s)) {
        i = len - idx - 1;
        c = c + pow(10, idx) * ((int)s[i] - '0'); 
        idx++;
    }

    return c;
}

int main()
{
    char s[100];

    printf("Enter str: ");
    scanf("%s", s);

    printf("%s atoi = %d \n", s, myatoi(s));

    return 0;
}
