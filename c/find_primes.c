#include <stdio.h>
#include <stdlib.h>

int is_prime(int n)
{   
    int s = 2;
    while(s < n ) {
        if (!(n%s)) {
            return 0;
        }
        s++;
    }
    printf("\t\t\t\t\t\t%d is a PRIME!!\n", n);
    return 1;

}

#define PRINT_ARR() {\
    printf("\n"); \
    for (int v = 2; v < n; v++) {\
        printf("%d ", arr[v]); \
    } \
    printf("\n"); \
}

int countPrimes(int n) {
    int *arr, n2, i, j, k = 0;
    int count = 0;
    
    if (n <= 1) return 0;
    if (n <= 3 ) {
        return n-1;
    }

    arr = (int *)malloc(n * sizeof(int));
    
    for (i = 0 ; i < n ; i++) {
        arr[i] = i;
    }


    for (i = 2 ; i < n ; i++) {
        j = k = arr[i];
        if (arr[k] == 0) { 
            continue;
        }
        while(k <= n) {
            if (arr[k] == 0) { 
                k = k + j;
                continue;
            }
            if (k != j) arr[k] = 0;
            k = k + j;
        }
    }

    for (i = 2 ; i < n ; i++) {
        if (arr[i] != 0) count++;
    }

    free(arr);

    return count;
}

int countPrimes2(int n) {
    int running_n = 5, count = 0;
    if(n==0) return 0;
    if(n<4) return n;
    
    while(running_n < n) {
        if (running_n%2) {
            int divisor = 2, not_a_prime = 0;
            while(divisor < running_n) {
                if (!(running_n % divisor)) {
                    not_a_prime = 1;
                    break;
                }
                divisor++;
            };
            if (!not_a_prime) {
                count++;
            }
        }
        running_n++;
    };
    return count + 3;
}

int main(int argc, char *argv[])
{
    int num = 0, c = 3;

    if (argc < 2) {
        printf("Err \n");
        return 0;
    }
    num = atoi(argv[1]);
    
    printf("count = %d \n", countPrimes(num));
    return 1;

    if (num < 4) {
        printf("c = %d \n", num);
        return 0;
    }

    int t = 4;

    while (t < num) {
        c = ((t % 2) ? ((is_prime(t)) ? c+1 : c) : c); 
        t++;
    };

    printf("c = %d \n", c);

    return 0;
}

int countPrimes_2(int n) {
    int *arr, n2, i, j, k = 0;
    int count = 2;// 2 and 3
    
    if (n%2) {
        n2 = n - 1;
    } else {
        n2 = n - 2;
    }

    if (n <= 1) return 0;
    if (n <= 3 ) {
        return n-1;
    }

    n2 = n2/2;

    arr = (int *)malloc(n2 * sizeof(int));
    
    arr[0] = 3;
    for (i = 1 ; i < n2 ; i++) {
        arr[i] = arr[i-1] + 2;
        printf("%d ", arr[i]);
    }

    printf("\n");

    i = 0;
    while(i < n2) {
        j = k = arr[i];
        printf("i: %d j: %d (%d)\n", i, j, arr[j]);
        if (j == 0) { 
            printf("\t\tSkipping i: %d j: %d\n", i, j);
            i++;
            continue;
        }
        while ( j < n ) {
            if (!(j%2)) { 
                printf("\t\tSkipping even i: %d j: %d\n", i, j);
                j = j + k;
                continue; 
            }

            printf("\t\tSetting to 0 ... i: %d j: %d \n", i, j);
            arr[j] = 0;
            j = j + k;
        };

        printf("\t\t\t");
        for (int t = 0 ; t < n2 ; t++) {
            printf("%d ", arr[t]);
        }
        printf("\n************\n");

        i++; 
    }

    for (i = 1 ; i < n2 ; i++) {
        printf("%d ", arr[i]);
        if (arr[i] != 0) count++;
    }

    printf("count: %d\n", count);

    return count;
}
