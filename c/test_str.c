#include <stdio.h>

void chgstr(char **name)
{
    *name="YYY";
    return;
}

int main()
{
    char *name = "XXX";
    chgstr(&name);
    printf("Name: %s \n", name);

    return 1;
}
