#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#define ARR_SZ (100000)
#define ARR_SZ (10)

void
load_array(unsigned int *arr)
{
    FILE *fp;
    unsigned int i;

    //fp = fopen("IntegerArray.txt", "r");
    fp = fopen("arr2", "r");

    for ( i = 0; i < ARR_SZ ; i++) {
        fscanf(fp, "%u\n", &arr[i]);
        printf("%u..%u \n", i, arr[i]);
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

void merge(unsigned int *arr, 
           unsigned int ls, unsigned int le, 
           unsigned int rs, unsigned int re)
{
    unsigned int lsz = (le - ls + 1), rsz = (re - rs + 1);
    unsigned int i, j, k, *left_arr, *right_arr;
    static int count = 1;

    left_arr = (unsigned int *)malloc(lsz  * sizeof(unsigned int));
    right_arr = (unsigned int *)malloc(rsz  * sizeof(unsigned int));

    memcpy(left_arr, arr + ls, lsz * sizeof(unsigned int));
    memcpy(right_arr, arr + rs, rsz * sizeof(unsigned int));

    printf("\t\tLeft array(%u - %u)  : ", ls, le);
    print_arr(left_arr, lsz);
    printf("\t\tRight array(%u - %u) : ", rs, re);
    print_arr(right_arr, rsz);

    printf("*** Before merge (%d) *** \n", count++); 

    print_arr(arr, ARR_SZ);
    i = ls;
    j = rs;
    for ( k = ls ; k < re ; k++ ) {
        printf("k %d ls %d le %d rs %d re %d i = %d j = %d left_arr[i] %d right_arr[j] %d \n", 
                k, ls, le, rs, re, i, j, left_arr[i], right_arr[j]);
        if ( (i <= le) && (left_arr[i] < right_arr[j])) {
            arr[k] = left_arr[i];
            printf("arr[k] = left_arr[i] -> arr[%d] = left_arr[%d] = %d \n", k, i, left_arr[i]);
            i++;
        } else {
            arr[k] = right_arr[j];
            printf("arr[k] = right_arr[j] -> arr[%d] = right_arr[%d] = %d \n", k, j, right_arr[j]);
            j++;
        }
    }

    printf("*** After merge (%d) *** \n", count++); 
    print_arr(arr, ARR_SZ);

    free(left_arr);
    free(right_arr);

    return;
}

void 
merge_sort(unsigned int *arr, unsigned int n1, unsigned int n2)
{

    unsigned int mid; 

    if (n2 == n1) {
        printf("\t...base case: n1  = n2 = arr[%u] = %u \n", n1, arr[n1]); 
        return;
    }

    mid = n1 + (n2 - n1)/2;

    printf("<<<>>> n1 = %d n2 = %d \n", n1, n2);

    printf("<<< n1 = %d mid = %d \n", n1, mid);
    merge_sort(arr, n1, mid);
    printf(">>> mid+1 = %d n2 = %d \n", mid+1, n2);
    merge_sort(arr, (mid + 1), n2);
    printf("-> a[%u] =  %u ; a[%u] = %u \n", n1, arr[n1], n2, arr[n2]);
    merge(arr, n1, mid, (mid + 1), n2);
}

int main()
{
    unsigned int arr[ARR_SZ];

    load_array(arr);

    print_arr(arr, ARR_SZ);
    merge_sort(arr, 0, ARR_SZ-1); 

    return 0;
}
