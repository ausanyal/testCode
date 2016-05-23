#include <stdio.h>

typedef struct __attribute__((__packed__)) copp_spine_policer_stats_ {
    char   a[40]; 
    unsigned char b;
    unsigned long long  c;
    unsigned long long  d;
    unsigned long long  e;
    unsigned long long  f;
} copp_proto_stats_t;

int main()
{

    copp_proto_stats_t t;

    printf("char: %lu unsigned char: %lu ull: %lu copp_proto_stats_t : %lu \n", 
        sizeof(char), sizeof(unsigned char), sizeof(unsigned long long), sizeof(copp_proto_stats_t));
    printf(" a: %p \n b: %p \n c: %p \n d: %p \n e: %p \n f: %p \n", 
        &t.a[39], &t.b, &t.c, &t.d, &t.e, &t.f); 

    return 0;
}
