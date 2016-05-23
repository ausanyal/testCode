#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <limits.h>

#define MAX 100000
#define MAX_VAL 1000000001

int list[MAX];

void list_swap(int i, int j)
{
    int temp = list[i];
    list[i] = list[j];
    list[j] = temp;
}

int partition(int beg, int end)
{
    int i, j;
    
    i = (beg+1);
    for (j=(beg+1) ; j <= end ; j++) {
        if(list[j] < list[beg]) {
            list_swap(i, j);
            i++;
        }
    }

    list_swap(beg, i-1);

    return i-1;
}

void qsort_list(int beg, int end)
{
    int p;
    
    if ((end - beg + 1) <= 1) {
        return;
    }
    p = partition(beg,end);
    qsort_list(beg, p-1);
    qsort_list(p+1, end);
    return;
}

int main() {
    
    int N,K;
    int i;
    
    scanf("%d %d",&N,&K);
    if(K > N) {
        printf("0");
    }

    for(i = 0;i < N;i++)
        scanf("%d", &list[i]);
  
    qsort_list(0, N-1);
    for(i = 0;i < N;i++) {
        //printf("..%d\n", list[i]);
    }

    int min = MAX_VAL, idx;
    for(i = 0 ; i < N ; i++) {
        int j = i+K-1;
        if (j < N) {
            //printf("i: %d  j: %d  list[j] %d - list[i] %d =  %d \n", i, j, list[j], list[i], (list[j] - list[i]));
            if((list[j] - list[i]) < min) {
                min = (list[j] - list[i]);
                idx = i;
            }
        }
    }

    int unfairness = min;// Compute the min unfairness over here, using N,K,candies
        
    printf("%d\n",unfairness);
    return 0;
}

