#include <stdio.h>
#include <stdlib.h>

typedef struct node_ {
    int id;
    int u;
    int v;
    int x, y;
    int dx1, dy1; // delta 1 x and delta 1 y
    int dx2, dy2; // delta 2 x and delta 2 y
    struct node_ *next;
} node_t;

int
qsort_node_cmp(const void *a, const void *b)
{
    node_t *node_a = (node_t *)a; 
    node_t *node_b = (node_t *)b; 

    return (node_a->id - node_b->id);
}

#define FIND_DELTA(i, j, dx, dy) {\
    int ux1, uy1, ux2, uy2;\
    ux1 = nd_arr[i].x;\
    ux2 = nd_arr[j].x;\
    dx = ((ux1 >= ux2) ? (ux1 - ux2) : (ux2 - ux1));\
    uy1 = nd_arr[i].y;\
    uy2 = nd_arr[j].y;\
    dy = ((uy1 >= uy2) ? (uy1 - uy2) : (uy2 - uy1));\
}

void
setup_mbrs(node_t *nd_arr, int n)
{
    int u, v, dx1, dy1, dx2, dy2;

    for (int i = 1; i <= n; i++) {
        // check 1
        u = nd_arr[i].u;
        if ( u == -1 ) {
            nd_arr[i].dx1 = -1;
            nd_arr[i].dy1 = -1;
        } else {
            FIND_DELTA(i, u, dx1, dy1);
            nd_arr[i].dx1 = dx1;
            nd_arr[i].dy1 = dy1;
        } 

        // check 2
        v = nd_arr[i].v;
        if ( v == -1 ) {
            nd_arr[i].dx2 = -1;
            nd_arr[i].dy2 = -1;
        } else {
            FIND_DELTA(i, v, dx2, dy2);
            nd_arr[i].dx2 = dx2;
            nd_arr[i].dy2 = dy2;
        } 
    }
}

int
check_count(int rx, int ry, int H, int V)
{
    if ((rx > H) || (ry > V)) {
        return 0;
    }

    return 1;

}

int
walk_all(node_t *nd_arr, int n, int H, int V)
{
    int count = 0;

    for (int i = 1; i <= n; i++) {
        for (int j = i+1; j <= n; j++) {
            int rx = 0, ry = 0;
            for(int k = i+1; k <= j ; k++) {
                
                if (nd_arr[k-1].dx2 == -1) continue;

                rx = rx + nd_arr[k-1].dx2;
                ry = ry + nd_arr[k-1].dy2;

                printf("i %d j %d k %d nd_arr[%d].dx2: %d nd_arr[%d].dy2: %d  rx: %d ry: %d\n", 
                        i, j, k, k-1, nd_arr[k-1].dx2, k-1, nd_arr[k-1].dy2, rx, ry);

            }
            if (!check_count(rx, ry, H, V)) {
                printf("\t\tNOT possible: i: %d j: %d \trx: %d ry: %d H: %d V: %d\n", 
                        i, j, rx, ry, H, V);
                count++;
            } else {

                printf("\t\tpossible: i: %d j: %d \trx: %d ry: %d H: %d V: %d\n", 
                        i, j, rx, ry, H, V);

            }
        }
    }
    return count;
}

int main()
{
    int n = 0, H = 0, V = 0;
    FILE *fp = fopen("input.txt", "r");


    fscanf(fp, "%d %d %d", &n, &H, &V); 

    node_t *nd_arr;
    nd_arr = (node_t *)calloc(1, (n+1) * sizeof(node_t));

    int x, y;
    for (int i = 0; i < n ; i++) {
        nd_arr[i+1].id = (i+1);
        fscanf(fp, "%d %d", &x, &y);
        nd_arr[i+1].x = x;
        nd_arr[i+1].y = y;
        nd_arr[i+1].u = nd_arr[i+1].v = -1;
    }

    int u, v;
    for (int i = 0; i < n-1 ; i++) {
        fscanf(fp, "%d %d", &u, &v);
        nd_arr[u].v = v;
        nd_arr[v].u = u;
    }

    qsort(nd_arr, n, sizeof(nd_arr[0]), qsort_node_cmp);

    setup_mbrs(nd_arr, n);

    for (int i = 1; i <= n; i++) {
        printf("id: %d u: %d v: %d dx1: %d dy1: %d dx2: %d dy2: %d \n", 
                nd_arr[i].id, nd_arr[i].u, nd_arr[i].v, nd_arr[i].dx1, 
                nd_arr[i].dy1, nd_arr[i].dx2, nd_arr[i].dy2);
    }

    int count = 0;
    count = walk_all(nd_arr, n, H, V);

    printf("%d \n", count);

    return 0;
}

