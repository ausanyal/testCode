#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "print_macro.h"

int main()
{
    arp_types_t t;
    INIT_DEFAULTS();
    int c = 20;
    //if_ordinal iod = 10; t.iod = iod;
    int dmac = 5; t.dmac = dmac;

    //iod = 5;

    MY_PRINT("%d", c);

    return 0;
}
