#include <stdio.h>

typedef struct t_ {
    unsigned int a1:3;
    unsigned int a2:4;
    unsigned int a3:4;
    unsigned int a4:4;
    unsigned int a5:4;
}t;

int main()
{
    t t1, *p;
    p = &t1;
    t1.a1=10;
    printf("p: %p \n", p);
    printf("a1: %u \n", (unsigned int)((p+0)));
    return 0;
}
