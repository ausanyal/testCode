#include <stdio.h>
#include <stddef.h>

typedef struct a_ {
    int i[10];
    char c;
    double x;
} a;

typedef struct aa_ {
    int i;
    char c;
    a *a_ptr;
    a a1;
} aa;

static inline int get_sz()
{
    aa aa1;
    return sizeof(aa1);
}

#define SIZE_AA get_sz();
//#define SIZE_AA (offsetof(aa, a1));
//#define offsetof(s,m) (size_t)&(((s *)0)->m)

int main()
{
    aa aa1;
    //printf("size: %lu %d\n", sizeof(aa1), SIZE_AA);
    printf("size: %d\n", get_sz());
    return 0;
}
