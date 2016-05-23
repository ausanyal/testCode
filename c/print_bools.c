#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define PRINT_IF_PRESENT(val, str) {\
    if ((val)) {\
        str_bools += sprintf(str_bools, "%s,", str);\
    }\
}

typedef unsigned int c_bool_t;

void print_lib_db_bools(char *str_bools, c_bool_t sc, c_bool_t ssr, c_bool_t dsr)
{
    PRINT_IF_PRESENT(sc,  " sc");
    PRINT_IF_PRESENT(ssr, "ssr");
    PRINT_IF_PRESENT(dsr, "dsr");
    *(str_bools -  1) = '\0';
}

int main()
{
    char str_bools[35];

    memset(str_bools, 0, sizeof(str_bools)); 
    print_lib_db_bools(str_bools, 1, 1, 1);
    printf("%s\n", str_bools);
    
    return 0;
}
