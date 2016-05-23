#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
Symbol  Value
I   1
V   5
X   10
L   50
C   100
D   500
M   1,000

*/

int
is_pow(int n, int base)
{
    int val = 0;
    printf("________ m %d base %d \n", n, base);
    while(1) {
        val = val * base;
        val = ((val != 0) ? val:1);
        if (val == n) return 1;
        if (val > n) return 0;
    }

    return 0;
}

char get_roman_for_num(int n, char *next)
{
    *next = '\0';
    switch(n) {
        case 1:
            *next = 'V';
            return 'I';
        case 5:
            *next = 'X';
            return 'V';
        case 10:
            *next = 'L';
            return 'X';
        case 50:
            *next = 'C';
            return 'L';
        case 100:
            *next = 'D';
            return 'C';
        case 500:
            *next = 'M';
            return 'D';
        case 1000:
            return 'M';
        default:
            return '\0';
    }
    return '\0';
}

#define GET_COUNT(RES, WITH, C, REMAIN, OUT) {       \
    C = RES/WITH;                                   \
    printf("1 %d %d %d %d [%s] \n", RES, WITH, C, REMAIN, OUT);\
    char ch;                                        \
    char next;                                  \
    if (C) {                                        \
        int temp = RES;\
        REMAIN = RES % WITH;                            \
        RES = temp;\
        if (is_pow(RES/9, 10) ||                     \
            is_pow(RES/4, 10)) {                     \
            ch = get_roman_for_num(WITH, &next);   \
            OUT[j++] = ch;                            \
            OUT[j++] = next;                            \
            printf("2 %d %d %d %d [%s] \n", RES, WITH, C, REMAIN, OUT);\
        }                                               \
        else {                                          \
            ch = get_roman_for_num(WITH, &next);   \
            for (int k = 0; k < C; k++) {               \
                OUT[j++] = ch;                        \
            }                                           \
            printf("3 %d %d %d %d [%s] \n", RES, WITH, C, REMAIN, OUT);\
        }                                               \
        total = total + C;                              \
    }                                                   \
    else {\
        REMAIN = RES;\
    }\
    printf("4 %d %d %d %d [%s] \n\n", RES, WITH, C, REMAIN, OUT);\
}

#define PRINT_LITERAL(count, literal) { \
    for (int i = 0; i < count ; i++) { \
        str[idx++] = literal;\
    } \
}

char *
to_roman(int i)
{
    int res = i, j = 0, remain = 0, total = 0, idx = 0;
    int mc=0, dc=0, cc=0, lc=0, xc=0, vc=0, ic=0;
    char *str, s[100];
    
    memset(s, 0, 100);
    GET_COUNT(res, 1000, mc, remain, s);
    GET_COUNT(remain, 500, dc, remain, s);
    GET_COUNT(remain, 100, cc, remain, s);
    GET_COUNT(remain, 50, lc, remain, s);
    GET_COUNT(remain, 10, xc, remain, s);
    GET_COUNT(remain, 5, vc, remain, s);
    GET_COUNT(remain, 1, ic, remain, s);

    str = (char *)malloc(j * sizeof(char));
    strncpy(str, s, j);
    
#if 0
    PRINT_LITERAL(mc, 'M');
    PRINT_LITERAL(dc, 'D');
    PRINT_LITERAL(cc, 'C');
    PRINT_LITERAL(lc, 'L');
    PRINT_LITERAL(xc, 'X');
    PRINT_LITERAL(vc, 'V');
    PRINT_LITERAL(ic, 'I');
#endif

    return str;
}

int main()
{
    int inp;

    scanf("%d", &inp);

    printf("%s\n", to_roman(inp));

    return 0;
}
