#include <stdio.h>
#include <stdlib.h>

#define HEXIFY(X) 0x##X##LU

#define B8IFY(Y) (((Y&0x0000000FLU)?1:0)  + \
                  ((Y&0x000000F0LU)?2:0)  + \
                  ((Y&0x00000F00LU)?4:0)  + \
                  ((Y&0x0000F000LU)?8:0)  + \
                  ((Y&0x000F0000LU)?16:0) + \
                  ((Y&0x00F00000LU)?32:0) + \
                  ((Y&0x0F000000LU)?64:0) + \
                  ((Y&0xF0000000LU)?128:0))

#define B8(Z) ((unsigned char)B8IFY(HEXIFY(Z)))

/* test if x is even */
#define B_EVEN(x)        (((x)&1)==0)

/* test if x is odd */
#define B_ODD(x)         (!B_EVEN((x)))

/* test if n-th bit in x is set */
#define B_IS_SET(x, n)   (((x) & (1<<(n)))?1:0)

/* set n-th bit in x */
#define B_SET(x, n)      ((x) |= (1<<(n)))

/* unset n-th bit in x */
#define B_UNSET(x, n)    ((x) &= ~(1<<(n)))

/* toggle n-th bit in x */
#define B_TOGGLE(x, n)   ((x) ^= (1<<(n)))

/* turn off right-most 1-bit in x */
#define B_TURNOFF_1(x)   ((x) &= ((x)-1))

/* isolate right-most 1-bit in x */
#define B_ISOLATE_1(x)   ((x) &= (-(x)))

/* right-propagate right-most 1-bit in x */
#define B_PROPAGATE_1(x) ((x) |= ((x)-1))

/* isolate right-most 0-bit in x */
#define B_ISOLATE_0(x)   ((x) = ~(x) & ((x)+1))

/* turn on right-most 0-bit in x */
#define B_TURNON_0(x)    ((x) |= ((x)+1))


/* 
** bithacks-test.c - tests for bithacks.h
**
** Released under the MIT license.
*/

int error_count;

#define TEST_OK(exp, what) do { \
    if ((exp)!=(what)) { \
        error_count++; \
        printf("Test '%s' at line %d failed.\n", #exp, __LINE__); \
    } } while(0)

#define TEST_END do { \
    if (error_count) { \
        printf("Testing failed: %d failed tests.\n", error_count); \
    } else { \
        printf("All tests OK.\n"); \
    } } while (0)

void test_B8()
{
    /* test B8 */
    TEST_OK(B8(0), 0);
    TEST_OK(B8(1), 1);
    TEST_OK(B8(11), 3);
    TEST_OK(B8(111), 7);
    TEST_OK(B8(1111), 15);
    TEST_OK(B8(11111), 31);
    TEST_OK(B8(111111), 63);
    TEST_OK(B8(1111111), 127);
    TEST_OK(B8(00000000), 0);
    TEST_OK(B8(11111111), 255);
    TEST_OK(B8(1010), 10);
    TEST_OK(B8(10101010), 170);
    TEST_OK(B8(01010101), 85);
}

void test_B_EVEN()
{
    /* test B_EVEN */
    TEST_OK(B_EVEN(B8(0)), 1);
    TEST_OK(B_EVEN(B8(00000000)), 1);
    TEST_OK(B_EVEN(B8(1)), 0);
    TEST_OK(B_EVEN(B8(11111111)), 0);
    TEST_OK(B_EVEN(B8(10101010)), 1);
    TEST_OK(B_EVEN(B8(01010101)), 0);
    TEST_OK(B_EVEN(44), 1);
    TEST_OK(B_EVEN(131), 0);
}

void test_B_ODD()
{
    /* test B_ODD */
    TEST_OK(B_ODD(B8(0)), 0);
    TEST_OK(B_ODD(B8(00000000)), 0);
    TEST_OK(B_ODD(B8(1)), 1);
    TEST_OK(B_ODD(B8(11111111)), 1);
    TEST_OK(B_ODD(B8(10101010)), 0);
    TEST_OK(B_ODD(B8(01010101)), 1);
    TEST_OK(B_ODD(44), 0);
    TEST_OK(B_ODD(131), 1);
}

void test_B_IS_SET()
{
    /* test B_IS_SET */
    TEST_OK(B_IS_SET(B8(0), 0), 0);
    TEST_OK(B_IS_SET(B8(00000000), 0), 0);
    TEST_OK(B_IS_SET(B8(1), 0), 1);
    TEST_OK(B_IS_SET(B8(11111111), 0), 1);
    TEST_OK(B_IS_SET(B8(11111111), 1), 1);
    TEST_OK(B_IS_SET(B8(11111111), 2), 1);
    TEST_OK(B_IS_SET(B8(11111111), 3), 1);
    TEST_OK(B_IS_SET(B8(11111111), 4), 1);
    TEST_OK(B_IS_SET(B8(11111111), 5), 1);
    TEST_OK(B_IS_SET(B8(11111111), 6), 1);
    TEST_OK(B_IS_SET(B8(11111111), 7), 1);
    TEST_OK(B_IS_SET(B8(11110000), 0), 0);
    TEST_OK(B_IS_SET(B8(11110000), 1), 0);
    TEST_OK(B_IS_SET(B8(11110000), 2), 0);
    TEST_OK(B_IS_SET(B8(11110000), 3), 0);
    TEST_OK(B_IS_SET(B8(11110000), 4), 1);
    TEST_OK(B_IS_SET(B8(11110000), 5), 1);
    TEST_OK(B_IS_SET(B8(11110000), 6), 1);
    TEST_OK(B_IS_SET(B8(11110000), 7), 1);
    TEST_OK(B_IS_SET(B8(00001111), 0), 1);
    TEST_OK(B_IS_SET(B8(00001111), 1), 1);
    TEST_OK(B_IS_SET(B8(00001111), 2), 1);
    TEST_OK(B_IS_SET(B8(00001111), 3), 1);
    TEST_OK(B_IS_SET(B8(00001111), 4), 0);
    TEST_OK(B_IS_SET(B8(00001111), 5), 0);
    TEST_OK(B_IS_SET(B8(00001111), 6), 0);
    TEST_OK(B_IS_SET(B8(00001111), 7), 0);
    TEST_OK(B_IS_SET(B8(10101010), 0), 0);
    TEST_OK(B_IS_SET(B8(10101010), 1), 1);
    TEST_OK(B_IS_SET(B8(10101010), 2), 0);
    TEST_OK(B_IS_SET(B8(10101010), 3), 1);
    TEST_OK(B_IS_SET(B8(10101010), 4), 0);
    TEST_OK(B_IS_SET(B8(10101010), 5), 1);
    TEST_OK(B_IS_SET(B8(10101010), 6), 0);
    TEST_OK(B_IS_SET(B8(10101010), 7), 1);
    TEST_OK(B_IS_SET(B8(01010101), 0), 1);
    TEST_OK(B_IS_SET(B8(01010101), 1), 0);
    TEST_OK(B_IS_SET(B8(01010101), 2), 1);
    TEST_OK(B_IS_SET(B8(01010101), 3), 0);
    TEST_OK(B_IS_SET(B8(01010101), 4), 1);
    TEST_OK(B_IS_SET(B8(01010101), 5), 0);
    TEST_OK(B_IS_SET(B8(01010101), 6), 1);
    TEST_OK(B_IS_SET(B8(01010101), 7), 0);
}

void test_B_SET()
{
    /* test B_SET */
    unsigned char x;

    x = B8(00000000);
    TEST_OK(B_SET(x, 0), B8(00000001));
    TEST_OK(B_SET(x, 1), B8(00000011));
    TEST_OK(B_SET(x, 2), B8(00000111));
    TEST_OK(B_SET(x, 3), B8(00001111));
    TEST_OK(B_SET(x, 4), B8(00011111));
    TEST_OK(B_SET(x, 5), B8(00111111));
    TEST_OK(B_SET(x, 6), B8(01111111));
    TEST_OK(B_SET(x, 7), B8(11111111));

    x = B8(11111111);
    TEST_OK(B_SET(x, 0), B8(11111111));
    TEST_OK(B_SET(x, 1), B8(11111111));
    TEST_OK(B_SET(x, 2), B8(11111111));
    TEST_OK(B_SET(x, 3), B8(11111111));
    TEST_OK(B_SET(x, 4), B8(11111111));
    TEST_OK(B_SET(x, 5), B8(11111111));
    TEST_OK(B_SET(x, 6), B8(11111111));
    TEST_OK(B_SET(x, 7), B8(11111111));
}

void test_B_UNSET()
{
    unsigned char x;
   
    x = B8(11111111);
    TEST_OK(B_UNSET(x, 0), B8(11111110));
    TEST_OK(B_UNSET(x, 1), B8(11111100));
    TEST_OK(B_UNSET(x, 2), B8(11111000));
    TEST_OK(B_UNSET(x, 3), B8(11110000));
    TEST_OK(B_UNSET(x, 4), B8(11100000));
    TEST_OK(B_UNSET(x, 5), B8(11000000));
    TEST_OK(B_UNSET(x, 6), B8(10000000));
    TEST_OK(B_UNSET(x, 7), B8(00000000));

    x = B8(00000000);
    TEST_OK(B_UNSET(x, 0), B8(00000000));
    TEST_OK(B_UNSET(x, 1), B8(00000000));
    TEST_OK(B_UNSET(x, 2), B8(00000000));
    TEST_OK(B_UNSET(x, 3), B8(00000000));
    TEST_OK(B_UNSET(x, 4), B8(00000000));
    TEST_OK(B_UNSET(x, 5), B8(00000000));
    TEST_OK(B_UNSET(x, 6), B8(00000000));
    TEST_OK(B_UNSET(x, 7), B8(00000000));
}

void test_B_TOGGLE()
{
    unsigned char x = B8(11111111);
    TEST_OK(B_TOGGLE(x, 0), B8(11111110));
    TEST_OK(B_TOGGLE(x, 0), B8(11111111));
    TEST_OK(B_TOGGLE(x, 1), B8(11111101));
    TEST_OK(B_TOGGLE(x, 1), B8(11111111));
    TEST_OK(B_TOGGLE(x, 2), B8(11111011));
    TEST_OK(B_TOGGLE(x, 2), B8(11111111));
    TEST_OK(B_TOGGLE(x, 3), B8(11110111));
    TEST_OK(B_TOGGLE(x, 3), B8(11111111));
    TEST_OK(B_TOGGLE(x, 4), B8(11101111));
    TEST_OK(B_TOGGLE(x, 4), B8(11111111));
    TEST_OK(B_TOGGLE(x, 5), B8(11011111));
    TEST_OK(B_TOGGLE(x, 5), B8(11111111));
    TEST_OK(B_TOGGLE(x, 6), B8(10111111));
    TEST_OK(B_TOGGLE(x, 6), B8(11111111));
    TEST_OK(B_TOGGLE(x, 7), B8(01111111));
    TEST_OK(B_TOGGLE(x, 7), B8(11111111));
}

void test_B_TURNOFF_1()
{
    unsigned char x;

    x = B8(11111111);
    TEST_OK(B_TURNOFF_1(x), B8(11111110));
    TEST_OK(B_TURNOFF_1(x), B8(11111100));
    TEST_OK(B_TURNOFF_1(x), B8(11111000));
    TEST_OK(B_TURNOFF_1(x), B8(11110000));
    TEST_OK(B_TURNOFF_1(x), B8(11100000));
    TEST_OK(B_TURNOFF_1(x), B8(11000000));
    TEST_OK(B_TURNOFF_1(x), B8(10000000));
    TEST_OK(B_TURNOFF_1(x), B8(00000000));
    TEST_OK(B_TURNOFF_1(x), B8(00000000));

    x = B8(10101010);
    TEST_OK(B_TURNOFF_1(x), B8(10101000));
    TEST_OK(B_TURNOFF_1(x), B8(10100000));
    TEST_OK(B_TURNOFF_1(x), B8(10000000));
    TEST_OK(B_TURNOFF_1(x), B8(00000000));
    TEST_OK(B_TURNOFF_1(x), B8(00000000));

    x = B8(01010101);
    TEST_OK(B_TURNOFF_1(x), B8(01010100));
    TEST_OK(B_TURNOFF_1(x), B8(01010000));
    TEST_OK(B_TURNOFF_1(x), B8(01000000));
    TEST_OK(B_TURNOFF_1(x), B8(00000000));
    TEST_OK(B_TURNOFF_1(x), B8(00000000));
}

void test_B_ISOLATE_1()
{
    unsigned char x;

    x = B8(11111111);
    TEST_OK(B_ISOLATE_1(x), B8(00000001));
    TEST_OK(B_ISOLATE_1(x), B8(00000001));

    x = B8(11111110);
    TEST_OK(B_ISOLATE_1(x), B8(00000010));
    TEST_OK(B_ISOLATE_1(x), B8(00000010));

    x = B8(11111100);
    TEST_OK(B_ISOLATE_1(x), B8(00000100));
    TEST_OK(B_ISOLATE_1(x), B8(00000100));

    x = B8(11111000);
    TEST_OK(B_ISOLATE_1(x), B8(00001000));
    TEST_OK(B_ISOLATE_1(x), B8(00001000));

    x = B8(11110000);
    TEST_OK(B_ISOLATE_1(x), B8(00010000));
    TEST_OK(B_ISOLATE_1(x), B8(00010000));

    x = B8(11100000);
    TEST_OK(B_ISOLATE_1(x), B8(00100000));
    TEST_OK(B_ISOLATE_1(x), B8(00100000));

    x = B8(11000000);
    TEST_OK(B_ISOLATE_1(x), B8(01000000));
    TEST_OK(B_ISOLATE_1(x), B8(01000000));

    x = B8(10000000);
    TEST_OK(B_ISOLATE_1(x), B8(10000000));
    TEST_OK(B_ISOLATE_1(x), B8(10000000));

    x = B8(00000000);
    TEST_OK(B_ISOLATE_1(x), B8(00000000));

    x = B8(10000000);
    TEST_OK(B_ISOLATE_1(x), B8(10000000));

    x = B8(10001001);
    TEST_OK(B_ISOLATE_1(x), B8(00000001));

    x = B8(10001000);
    TEST_OK(B_ISOLATE_1(x), B8(00001000));
}

void test_B_PROPAGATE_1()
{
    unsigned char x;

    x = B8(00000000);
    TEST_OK(B_PROPAGATE_1(x), B8(11111111));
    TEST_OK(B_PROPAGATE_1(x), B8(11111111));

    x = B8(10000000);
    TEST_OK(B_PROPAGATE_1(x), B8(11111111));

    x = B8(11000000);
    TEST_OK(B_PROPAGATE_1(x), B8(11111111));

    x = B8(11100000);
    TEST_OK(B_PROPAGATE_1(x), B8(11111111));

    x = B8(11110000);
    TEST_OK(B_PROPAGATE_1(x), B8(11111111));

    x = B8(11111000);
    TEST_OK(B_PROPAGATE_1(x), B8(11111111));

    x = B8(11111100);
    TEST_OK(B_PROPAGATE_1(x), B8(11111111));

    x = B8(11111110);
    TEST_OK(B_PROPAGATE_1(x), B8(11111111));

    x = B8(11111111);
    TEST_OK(B_PROPAGATE_1(x), B8(11111111));

    x = B8(00100000);
    TEST_OK(B_PROPAGATE_1(x), B8(00111111));
    TEST_OK(B_PROPAGATE_1(x), B8(00111111));

    x = B8(10101000);
    TEST_OK(B_PROPAGATE_1(x), B8(10101111));
    TEST_OK(B_PROPAGATE_1(x), B8(10101111));

    x = B8(10101010);
    TEST_OK(B_PROPAGATE_1(x), B8(10101011));
    TEST_OK(B_PROPAGATE_1(x), B8(10101011));

    x = B8(10101010);
    TEST_OK(B_PROPAGATE_1(x), B8(10101011));
    TEST_OK(B_PROPAGATE_1(x), B8(10101011));
}

void test_B_ISOLATE_0()
{
    unsigned char x;

    x = B8(00000000);
    TEST_OK(B_ISOLATE_0(x), B8(00000001));
    TEST_OK(B_ISOLATE_0(x), B8(00000010));
    TEST_OK(B_ISOLATE_0(x), B8(00000001));

    x = B8(00000011);
    TEST_OK(B_ISOLATE_0(x), B8(00000100));
    TEST_OK(B_ISOLATE_0(x), B8(00000001));

    x = B8(00000111);
    TEST_OK(B_ISOLATE_0(x), B8(00001000));
    TEST_OK(B_ISOLATE_0(x), B8(00000001));

    x = B8(00001111);
    TEST_OK(B_ISOLATE_0(x), B8(00010000));
    TEST_OK(B_ISOLATE_0(x), B8(00000001));

    x = B8(00011111);
    TEST_OK(B_ISOLATE_0(x), B8(00100000));
    TEST_OK(B_ISOLATE_0(x), B8(00000001));

    x = B8(00111111);
    TEST_OK(B_ISOLATE_0(x), B8(01000000));
    TEST_OK(B_ISOLATE_0(x), B8(00000001));

    x = B8(01111111);
    TEST_OK(B_ISOLATE_0(x), B8(10000000));
    TEST_OK(B_ISOLATE_0(x), B8(00000001));

    x = B8(11111111);
    TEST_OK(B_ISOLATE_0(x), B8(00000000));

    x = B8(01010101);
    TEST_OK(B_ISOLATE_0(x), B8(00000010));

    x = B8(01010111);
    TEST_OK(B_ISOLATE_0(x), B8(00001000));

    x = B8(01011111);
    TEST_OK(B_ISOLATE_0(x), B8(00100000));

    x = B8(01111111);
    TEST_OK(B_ISOLATE_0(x), B8(10000000));
}

void test_B_TURNON_0()
{
    unsigned char x;

    x = B8(00000000);
    TEST_OK(B_TURNON_0(x), B8(00000001));
    TEST_OK(B_TURNON_0(x), B8(00000011));
    TEST_OK(B_TURNON_0(x), B8(00000111));
    TEST_OK(B_TURNON_0(x), B8(00001111));
    TEST_OK(B_TURNON_0(x), B8(00011111));
    TEST_OK(B_TURNON_0(x), B8(00111111));
    TEST_OK(B_TURNON_0(x), B8(01111111));
    TEST_OK(B_TURNON_0(x), B8(11111111));
    TEST_OK(B_TURNON_0(x), B8(11111111));

    x = B8(10101010);
    TEST_OK(B_TURNON_0(x), B8(10101011));
    TEST_OK(B_TURNON_0(x), B8(10101111));
    TEST_OK(B_TURNON_0(x), B8(10111111));
    TEST_OK(B_TURNON_0(x), B8(11111111));

    x = B8(10000000);
    TEST_OK(B_TURNON_0(x), B8(10000001));
    TEST_OK(B_TURNON_0(x), B8(10000011));
    TEST_OK(B_TURNON_0(x), B8(10000111));
    TEST_OK(B_TURNON_0(x), B8(10001111));
    TEST_OK(B_TURNON_0(x), B8(10011111));
    TEST_OK(B_TURNON_0(x), B8(10111111));
    TEST_OK(B_TURNON_0(x), B8(11111111));
}

int main()
{
    test_B8();
    test_B_EVEN();
    test_B_ODD();
    test_B_IS_SET();
    test_B_SET();
    test_B_UNSET();
    test_B_TOGGLE();
    test_B_TURNOFF_1();
    test_B_ISOLATE_1();
    test_B_PROPAGATE_1();
    test_B_ISOLATE_0();
    test_B_TURNON_0();

    TEST_END;

    return error_count ? EXIT_FAILURE : EXIT_SUCCESS;
}
