#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void interleave(char *str1, char *str2, char *str, int len)
{
    if ((str1[0] == '\0') && (str2[0] == '\0')) {
        printf("%s\n", str-len);
    }

    if (str1[0] != '\0') {
        str[0] = str1[0];
        interleave(str1+1, str2, str+1, len);
    }

    if (str2[0] != '\0') {
        str[0] = str2[0];
        interleave(str1, str2+1, str+1, len);
    }
}

int main()
{
    char *str1 = "AB";
    char *str2 = "MNO";

    int len = strlen(str1) + strlen(str2);
    printf("%d \n", len);

    char *str = (char *)malloc(len * sizeof(char));
    memset(str, 0, len+1);
    interleave(str1, str2, str, len);

    return 0;
}
