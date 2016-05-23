#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARR_SZ (10000)
//#define ARR_SZ (10)
//#define ARR_SZ (10)
int debug = 0;
typedef unsigned int uint32;

#define myprintf(var, arg...) { \
    if (debug) { \
        printf("%-20s:%-5d > "var, __FUNCTION__, __LINE__, ##arg); \
    } \
}

void
load_array(uint32 *arr)
{
    FILE *fp;
    uint32 i;

    //fp = fopen("IntegerArray.txt", "r");
    //fp = fopen("arr2", "r");
    fp = fopen("test_file", "r");

    for ( i = 0; i < ARR_SZ ; i++) {
        fscanf(fp, "%u\n", &arr[i]);
        //myprintf("%u..%u \n", i, arr[i]);
    }
}

void
print_arr(uint32 *arr, uint32 beg, uint32 end)
{
    uint32 i;

    printf("\t\t\t   > ");
    for ( i = beg; i <= end; i++ ) {
        printf("%u ", arr[i]);
    }
    printf("\n");
}

uint32 
choose_pivot(uint32 *arr, uint32 beg, uint32 end)
{
    int mid, x, median;

    x = end - beg + 1;

    //print_arr(arr, beg, end);

    if (x%2) { // odd
        mid = beg + x/2;
    } else {
        mid = beg + x/2 - 1;
    }

    //printf("arr[%d] : %u | arr[%d] : %u | arr[%d] : %u\n", 
    //        beg, arr[beg], mid, arr[mid], end, arr[end]);
    
    if ( ((arr[beg] >= arr[mid]) && (arr[beg] <= arr[end])) ||
         ((arr[beg] >= arr[end]) && (arr[beg] <= arr[mid])) ) {
            // beg is median
            median = beg;
    } 
    if ( ((arr[mid] >= arr[beg]) && (arr[mid] <= arr[end])) ||
         ((arr[mid] >= arr[end]) && (arr[mid] <= arr[beg])) ) {
            // mid is median
            median = mid;
    } 
    if ( ((arr[end] >= arr[beg]) && (arr[end] <= arr[mid])) ||
         ((arr[end] >= arr[mid]) && (arr[end] <= arr[beg])) ) {
            // end is median
            median = end;
    } 
    
    //printf("median = arr[%d] = %u\n", median, arr[median]);
    return median;
}

void
a_swap(uint32 *arr, uint32 pos1, uint32 pos2)
{
    uint32 temp;

    if (pos1 != pos2) {
        temp = arr[pos1];
        arr[pos1] = arr[pos2];
        arr[pos2] = temp;
    }
}

uint32 count = 0;

uint32
partition(uint32 *arr, uint32 pivot, uint32 beg, uint32 end)
{
    static int level = 0;
    int i, j;

    level++;


    // Swap pivot with first element
    a_swap(arr, pivot, beg);

    myprintf("sub-array: \n");

    //since pivot now at beg 
    i = beg+1;
    for (j = (beg + 1) ; j <= end ; j++) {
        if ( arr[j] < arr[beg] ) { // nothing to be done if arr[j] >= arr[pivot]
            a_swap(arr, i, j);
            i++;
        }
        count++;
    }

    a_swap(arr, beg, i-1);

    myprintf("After %d iter returning pos %d \n", level, (i-1));

    return (i-1);
}

void quick_sort(uint32 *arr, int beg, int end)
{
    uint32 a, p;

    if ( (end - beg + 1) <= 1) {
        myprintf("base case beg = %d end = %d \n", beg, end);
        return;
    }

    p = choose_pivot(arr, beg, end);
    //printf("qsort called with pivot = arr[%d]: %u beg = %d end = %d \n", p, arr[p], beg, end);
    a = partition(arr, p, beg, end);
    quick_sort(arr, beg, a-1);
    quick_sort(arr, a+1, end);
}


int main()
{
    uint32 arr[ARR_SZ];

    load_array(arr);

    //print_arr(arr, 0, ARR_SZ - 1);

    quick_sort(arr, 0, ARR_SZ-1); 

    print_arr(arr, 0, ARR_SZ - 1);
    printf("Comparisons: %u \n", count);

    return 0;
}
