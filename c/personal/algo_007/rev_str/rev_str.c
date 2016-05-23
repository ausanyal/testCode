#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void
rev_str(char *str)
{
    int last, i;
    char c;

    last = strlen(str) - 1;
    for ( i = 0 ; i < strlen(str)/2 ; i++ ) {
        c = str[i];
        str[i] = str[last - i];
        str[last - i] = c;
    }
    str[strlen(str)] = '\0';
}

int main()
{
    char str[100];
    printf("Enter str: ");
    scanf("%s", str);
    rev_str(str);
    printf("str = %s\n", str); 
    return 0;
}
