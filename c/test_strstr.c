#include <stdio.h>
#include <string.h>


int main()
{
    char s1[30], s2[30];

    strcpy(s1, "hello how are you");
    strcpy(s2, "how are your");

    if (strstr(s1, "hello how") != NULL) {
        printf("Found \n");
    } else {
        printf("not Found \n");
    }

    return 0;
}
