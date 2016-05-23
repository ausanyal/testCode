#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARR_SZ (100000)
//#define ARR_SZ (10)
//#define ARR_SZ (6)
int debug = 0;

#define dprintf(var, arg...) { \
    if (debug) { \
        printf(var, ##arg); \
    } \
}

void
load_array(unsigned int *arr)
{
    FILE *fp;
    unsigned int i;

    fp = fopen("IntegerArray.txt", "r");
    //fp = fopen("arr2", "r");
    //fp = fopen("test_file", "r");

    for ( i = 0; i < ARR_SZ ; i++) {
        fscanf(fp, "%u\n", &arr[i]);
        dprintf("%u..%u \n", i, arr[i]);
    }
}

void
print_arr(unsigned int *arr, unsigned int sz)
{
    unsigned int i;

    printf("\t\t\t");
    for ( i = 0; i < sz; i++ ) {
        printf("%u ", arr[i]);
    }
    printf("\n");
}

unsigned int count = 0;
void merge(unsigned int *arr, 
           unsigned int ls, unsigned int le, 
           unsigned int rs, unsigned int re)
{
    unsigned int lsz = (le - ls + 1), rsz = (re - rs + 1);
    unsigned int i, j, k, t, *left_arr, *right_arr;

    left_arr = (unsigned int *)malloc(lsz  * sizeof(unsigned int));
    right_arr = (unsigned int *)malloc(rsz  * sizeof(unsigned int));

    memcpy(left_arr, arr + ls, lsz * sizeof(unsigned int));
    memcpy(right_arr, arr + rs, rsz * sizeof(unsigned int));

    i = 0;
    j = 0;
    for ( k = ls ; k <= re ; k++ ) {
        if ( (i < lsz) && 
             ( (left_arr[i] < right_arr[j]) || (j >= rsz) ) ) {
            arr[k] = left_arr[i];
            i++;
        } else {
            arr[k] = right_arr[j];
            j++;
            count = count + (lsz - i);
        }
    }

    free(left_arr);
    free(right_arr);

    return;
}

void 
merge_sort(unsigned int *arr, unsigned int n1, unsigned int n2)
{

    unsigned int mid; 

    if (n2 == n1) {
        return;
    }

    mid = n1 + (n2 - n1)/2;

    merge_sort(arr, n1, mid);
    merge_sort(arr, (mid + 1), n2);
    merge(arr, n1, mid, (mid + 1), n2);
}

int main()
{
    unsigned int arr[ARR_SZ];

    load_array(arr);

    merge_sort(arr, 0, ARR_SZ-1); 
    print_arr(arr, ARR_SZ);
    printf("Split inv = %u \n", count);

    return 0;
}
