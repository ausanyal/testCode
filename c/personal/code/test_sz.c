#include <stdio.h>
#include <string.h>

typedef struct __attribute__((__packed__)) table_ {
//typedef struct table_ {
    unsigned char u;
    unsigned long i;
    int a;
    char c;
} table_t;

typedef struct table2_ {
    unsigned char u;
    int a;
} table2_t;

void test_sz( void *t )
{
    char *t2 = (char *)t;

    if ((*t2 & 0xff) == 0xfc) {
        printf("1 \n");
    } else {
        printf("2 \n");
    }

    if (( (*(char *)t) & 0xff) == 0xfc ) {

        printf("..1 \n");
    } else {

        printf("..2 \n");
    }
}

#if 0 // not packed
y:   0x7fff531b3b88
t:   0x7fff531b3b90 (8)
t.u: 0x7fff531b3b90 (0)
t.i: 0x7fff531b3b98 (8)
t.a: 0x7fff531b3ba0 (8)
t.c: 0x7fff531b3ba4 (4)
x:   0x7fff531b3ba8 (4)
#endif


#if 0 // packed
y:   0x7fff565c6b90
t:   0x7fff565c6b98 (8)
t.u: 0x7fff565c6b98 (0)
t.i: 0x7fff565c6b99 (1)
t.a: 0x7fff565c6ba1 (8)
t.c: 0x7fff565c6ba5 (4)
x:   0x7fff565c6ba8 (3)
#endif

int main()
{
    int x = 1;
    table_t t;
    double y = 1.0;

    memset(&t, 0, sizeof(table_t));
    t.i = -1;
    t.u = 0xfc;

    test_sz(&t);

    unsigned short *c = (unsigned short *)&t;
    int i;

    printf("size: %lu -> u: %lu i: %lu a: %lu c: %lu \n", 
        sizeof(t), sizeof(t.u), sizeof(t.i), sizeof(t.a), sizeof(t.c));

    printf("x: %p \n", &x);
    printf("t: %p \nt.u: %p (%lu)\nt.i: %p (%lu)\nt.a: %p (%lu)\nt.c: %p (%lu) \n", 
            &t, &t.u, ((char *)&t.u - (char *)&t), 
            &t.i, ((char *)&t.i - (char *)&t.u), 
            &t.a, ((char *)&t.a - (char *)&t.i), 
            &t.c, ((char *)&t.c - (char *)&t.a));
    printf("y: %p \n", &y);

    for (i = 0; i < sizeof(table_t); i = i+2) {
        printf("0x%x ", *(c+i) );
    }
    printf("\n");

    return (0);
}
