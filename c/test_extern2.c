#include <stdio.h>

int main()
{
    extern int gl_access;

    printf("gl_access: %d \n", gl_access);
    return 0;
}
