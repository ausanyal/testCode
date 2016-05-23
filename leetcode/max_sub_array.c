//http://www.practice.geeksforgeeks.org/problem-page.php?pid=106
/*
Example:
Input
2
3
1 2 3
4
-1 -2 -3 -4

Output
6
-1
*/

#include <stdio.h>
#include <stdlib.h>

int max_sub_array(int arr[], int sz)
{
    int cur = 0, max = 0;
    
    for(int i = 0; i < sz; i++) {
        cur += arr[i];
        if (cur < 0) cur = 0;
        if (cur > max) max = cur;
    }
    
    return max;
}

int main()
{
    int num_tc = 0, arr_sz = 0, msa = 0;

    scanf("%d\n", &num_tc);
    
    for (int i = 0; i < num_tc; i++) {

        scanf("%d\n", &arr_sz);

        int *arr;
        arr = (int *)malloc(arr_sz * sizeof(int));

        for(int j = 0; j < arr_sz; j++) {
            scanf("%d", &arr[j]);
        }

        msa = max_sub_array(arr, arr_sz);
        printf("%d\n", msa);
        free(arr);
    }

    return msa;
}
