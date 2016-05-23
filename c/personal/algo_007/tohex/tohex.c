#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char legend[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'}; 

char *
tohex(int num)
{
    int r, c = 1;
    char *s;

    s = (char *)malloc(sizeof(char));
    
    while(num) {
        r = num % 16;
        num = num / 16;
        s[c-1] = legend[r];
        //printf("c=%d r %d -> %c\n", c, r, legend[r]);
        ++c;
        s = realloc(s, c * sizeof(char));  
    }

    int i, last = strlen(s) - 1;
    char ch;
    for (i = 0 ; i < strlen(s)/2 ; i++) {
        ch = s[i];
        s[i] = s[last];
        s[last] = ch;
    }

    return s;
}

int main()
{
    int num;
    char *s;

    printf("Enter num: ");
    scanf("%d", &num);
    s= tohex(num);
    printf("hex = %s\n", s);
    free(s);

    return 1;
}
