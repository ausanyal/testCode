#include <stdio.h>
#include <string.h>

int main()
{
    char a[] = "abc";
    char *p = "abc";

    a[1] = 'x';

    printf("%s ( %lu ) %s \n", a, strlen(a), p);

    for (int i = 0; i < strlen(a); i++) {
        printf("%c \n", a[i]);
    }

    return 0;
    
}
