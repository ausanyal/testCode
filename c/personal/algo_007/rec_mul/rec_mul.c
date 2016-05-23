#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

typedef unsigned int uint32;
typedef struct num_inf_ {
    uint32 *arr;
    uint32 num;
    uint32 ndigits;
}num_inf_t;

int 
get_num_digits(uint32 num);
uint32 *
split_num(uint32 n, int sz);
void 
print_arr(uint32 *arr, int n);

#define prt(fmt, args...) {     \
    int rec_i; \
    printf("%-20s.%-5d:"fmt, __FUNCTION__, __LINE__, ##args);   \
}

#define prt2(fmt, args...) {     \
    int rec_i; \
    char sp[100]; \
    memset(sp, 0, sizeof(sp)); \
    for (rec_i = 0; rec_i < level; rec_i++) { \
        strcat(sp, "\t\t");    \
    } \
    printf("(%-2d)", level);  \
    printf("%-20s.%-5d%s"fmt, __FUNCTION__, __LINE__, sp, ##args);   \
}

/*
Karatsuba method:
-----------------
Write x = (10^(n/2)a + b)  and y = (10^(n/2)c + d)
Where a,b,c,d are n/2-digit*numbers.

x = 5678 ; y = 1234
[example: x =    5678     ;   y = 1234
          a = 56, b = 78,   c = 12, d = 34]
Then x.y = (10^(n/2)a + b).(10^(n/2)c + d)
         = 10^n(ac) + 10^(n/2)(ad + bc) + bd  (*)
Idea : recursively compute ac, ad, bc, bd, then
compute (*) in the obvious way 
1. 1. Recursively compute ac 
2. Recursively compute bd
3. Recursively compute (a+b)(c+d) = ac+bd+ad+bc
Gauss’ Trick : (3) – (1) – (2) = ad + bc
Upshot : Only need 3 recursive multiplications (and some adds)
*/

uint32
get_combined_num(uint32 *arr, int start_idx, int count)
{
    uint32  num = 0;
    int     i, pos = 0, last;

    last = start_idx + count;

    for (i = start_idx ; i < last; i++) {
        num = arr[i] + (num * pow(10, pos));
        pos++;
    }

    return num;
}

uint32 
compute_recurse(uint32 num1, uint32 num2, int level)
{
    int s1, s2, ndigits1, ndigits2, lvl;
    uint32 a, b, c, d, res, res1, res2, res3;
    uint32 *arr1, *arr2;

    lvl = level + 1;
    ndigits1 = get_num_digits(num1);
    ndigits2 = get_num_digits(num2);

    if ((ndigits1 == 1) || (ndigits2 == 1)) {
        //prt2("boundary case: n1 %d n2 %d --> res %d\n", num1, num2, (num1 * num2));
        return (num1 * num2);
    }

    arr1 = split_num(num1, ndigits1);
    arr2 = split_num(num2, ndigits2);

    //print_arr(arr1, ndigits1);
    //print_arr(arr2, ndigits2);

    s2 = ndigits1/2;
    s1 = ndigits1 - s2;
    a = get_combined_num(arr1, 0, s1);
    b = get_combined_num(arr1, s1, s2);
    prt2("a: %u b: %u \n", a, b);

    s2 = ndigits2/2;
    s1 = ndigits2 - s2;
    c = get_combined_num(arr2, 0, s1);
    d = get_combined_num(arr2, s1, s2);
    prt2("c: %u d: %u \n", c, d);

    res1 = compute_recurse(a, c, lvl);
    res2 = compute_recurse(b, d, lvl);
    res3 = compute_recurse((a+b), (c+d), lvl);

    uint32 ac = res1;
    uint32 bd = res2;
    uint32 abcd = res3;
    uint32 adbc = res3 - res2 - res1;

    res = (pow(10, ndigits1) * ac) + 
          (pow(10, ndigits1/2) * adbc) +
          bd;

    prt2("10^n.ac(a %u c %u n=%d) = %f \n", a, c, ndigits1, (pow(10, ndigits1) * ac));
    prt2("bd(b %u d %u) = %u \n", b, d, bd);
    prt2("10^n/2.(ad+bc)(n=%d) = %f \n", 
         ndigits1, (pow(10, ndigits1/2) * (res3 - res2 - res1)));


    free(arr1);
    free(arr2);

    printf("***************************************\n");
    prt2("res(%d * %d) = %d \n", num1, num2, res);
    printf("***************************************\n");

    return res;

}

uint32 *
split_num(uint32 n, int sz)
{
    uint32 *temp;
    int i, count = 0;

    temp = (uint32 *)malloc(sz * sizeof(uint32));

    while (n) {
        temp[sz - 1 - count] = (n)%10;
        count++;
        (n) = (n)/10;
    }



    return temp;
}

int main()
{
    uint32 num1, num2, res;
    int level = 0;

    printf("Enter x:");
    scanf("%u", &num1);
    printf("Enter y:");
    scanf("%u", &num2);

    res = compute_recurse(num1, num2, level);

    printf("_________________ Res = %u ________________ \n", res);

    return 0;
}

int 
get_num_digits(uint32 num)
{
    int n = 0;

    while(num) {
        num = num/10;
        n++;
    }

    return n;
}

void 
print_arr(uint32 *arr, int n)
{
    int i;
    
    prt("num: ");
    for (i = 0 ; i < n ; i++) {
        printf(" %d ", arr[i]);
    }

    printf("\n");

}
