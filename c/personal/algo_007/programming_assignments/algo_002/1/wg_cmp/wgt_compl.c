#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARR_SZ (10000)
//#define ARR_SZ (32)
int debug = 0;

void print_wlt(int c);

typedef struct wl_ {
    double w;
    double l;
    double diff;
    double div;
    int c;
} wl_t;

wl_t wlt[ARR_SZ];
int count;

#define WW(i) (wlt[i].w)
#define WL(i) (wlt[i].l)
#define WD(i) (wlt[i].diff)
#define WDV(i) (wlt[i].div)
#define WC(i) (wlt[i].c)

#define dprintf(var, arg...) { \
    if (debug) { \
        printf(var, ##arg); \
    } \
}

void
load_array()
{
    FILE *fp;
    unsigned int i;

    fp = fopen("input", "r");
    //fp = fopen("test_file", "r");

    fscanf(fp, "%u\n", &count);
    dprintf("count = %d \n", count);

    for ( i = 0; i < ARR_SZ ; i++) {
        /*
        if (i == 0) {
            fscanf(fp, "%u\n", &count);
            dprintf("count = %d \n", count);
        } 
        */
        {
            fscanf(fp, "%lf %lf\n", &WW(i), &WL(i));
            WD(i) = WW(i) - WL(i);
            WDV(i) = WW(i)/WL(i);
            //WD(i) = WW(i)/WL(i);
            dprintf("WW %lf WL %lf \n", WW(i), WL(i));
        }
    }

    print_wlt(5);
}

void 
sort_wlt()
{
    int i, j,  idx = 0;
    double largest = -999999.0;
    wl_t temp;

    for ( j = 0; j < ARR_SZ; j++ ) {
        largest = -999999;
        for ( i = j; i < ARR_SZ ; i++ ) {
            if ((largest == WD(i)) && (idx != i)) {
                dprintf("\t largest %5lf: j %d %lf %lf (%lf) <--> idx %d %lf %lf (%lf)\n", 
                        largest, j, WW(i), WL(i), WD(i), idx, WW(idx), WL(idx), WD(idx));
                if (WW(i) > WW(idx)) {
                    dprintf("j %d found better weight with i %d vs idx %d (i.%lf vs idx.%lf)\n", 
                            j, i, idx, WW(i), WW(idx));
                    idx = i;
                }
            } 
            if (largest < WD(i)) {
                printf("largest %5lf: j %d %lf %lf %lf <--> idx %d %lf %lf %lf\n", 
                        largest, j, WW(i), WL(i), WD(i), idx, WW(idx), WL(idx), WD(idx));
                largest = WD(i);
                idx = i;
            }
        }
        // Swap wlt at idx with wlt at j
        memcpy(&temp, &wlt[j], sizeof(wl_t));
        memcpy(&wlt[j], &wlt[idx], sizeof(wl_t));
        memcpy(&wlt[idx], &temp, sizeof(wl_t));
        dprintf("j %5d wd %5lf ww %5lf wl %5lf \n", j, WD(j), WW(j), WL(j));
    }

    print_wlt(5);

}

double sum = 0.0;
void
compute_cmp_time()
{
    int j;

    for ( j = 0; j < ARR_SZ; j++ ) {
        if (j == 0) {
            WC(j) = WL(j); 
        } else {

            WC(j) = WL(j) + WC(j-1); 
        }
        sum = sum + (WC(j) * WW(j));
        printf("j %3d   wc %3d   wl %3lf   ww %3lf   wd %3lf   sum %3lf\n", j, WC(j), WL(j), WW(j), WD(j), sum);
    }

}

void 
wgt_cmp()
{
    sort_wlt();
    compute_cmp_time();
}

int main()
{

    load_array();
    wgt_cmp();

    return 0;
}

void
print_wlt(int c)
{
    unsigned int i;

    dprintf("************* \n");
    for ( i = 0; i < c ; i++) {
        dprintf("%5lf : %lf  %lf \n", WD(i) , WW(i), WL(i));
    }
    dprintf("***** \n");
}
