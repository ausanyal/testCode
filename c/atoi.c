#include <stdio.h>
#include <string.h>
#include <math.h>

int 
my_ato_i(char * s)
{
    int l = strlen(s), t, u, n = 0;
    int x = 1;

    for (int i = 0; i < l; i++) {
        if ((i == 0) && (s[i] == '-')) {
            x = -1;
            continue;
        }
        t = (s[i] - '0');
        n = n * 10 + t;
    }

    printf("x: %d n: %d\n", x, n);
    return (n * x);
}

int main()
{
    char s[100];

    scanf("%s", s);
    printf("s: %s ; atoi_n: %d \n", s, my_ato_i(s));

    return 1;
}
