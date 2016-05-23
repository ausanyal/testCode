#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int n = 0, H = 0, V = 0;
int count = 0, sz = 0;
int *visited;

typedef struct nodei_ {
    int id;
    int dx;
    int dy;
    struct nodei_ *next;
} nodet_t;

typedef struct node_ {
    int id;
    int u;
    int v;
    int x, y;
    int dx1, dy1; // delta 1 x and delta 1 y
    int dx2, dy2; // delta 2 x and delta 2 y
    int nbr_count;
    int *list;
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

int
check_count(int rx, int ry)
{
    if ((rx > H) || (ry > V)) {
        return 0;
    }

    return 1;

}

void 
append_node_list(node_t *nd_arr, int u, int v)
{
    node_t *node_u = &nd_arr[u], *node_v = &nd_arr[v];
    
    //printf("u: %d [%d] v: %d [%d] \n", u, node_u->nbr_count, v, node_v->nbr_count);
    if (node_u && node_v) {
        node_u->list[node_u->nbr_count++] = v;
        node_v->list[node_v->nbr_count++] = u;
    }
}

void recurse_walk(node_t *nd_arr, int s, int u, int v, int dx, int dy) 
{
    int cdx, cdy;

    if (visited[v]) {
        //printf("already visited v: %d \n", v);
        return;
    }

    visited[v] = 1;

    FIND_DELTA(u, v, cdx, cdy);
    dx += cdx;
    dy += cdy;
    if (!check_count(dx, dy)) {
        //printf("no path from %d -> %d \n", s, v);
        count++;
        //printf("%d ", count);
    } else {
        //printf("path from %d -> %d \n", s, v);

    }

    node_t *nv = &nd_arr[v];
    for (int i = 0; i < nv->nbr_count; i++) {
        recurse_walk(nd_arr, s, v, nv->list[i], dx, dy); 
    }
}

void span_graph(node_t *nd_arr, int s)
{
    int v, cdx, cdy;
    node_t *nd = &nd_arr[s];

    memset(visited, 0, sz);
    visited[s] = 1;

    for (int i = 0; i < nd->nbr_count; i++) {
        v = nd->list[i];
        if (v < 1) { 
            continue;
        }
        recurse_walk(nd_arr, s, s, v, 0, 0);  
    }
}

int main()
{
    FILE *fp = fopen("input.txt", "r");

    fscanf(fp, "%d %d %d", &n, &H, &V); 

    sz = (int)(n * sizeof(int));
    visited = (int *)calloc(1, sz);
    node_t *nd_arr;
    nd_arr = (node_t *)calloc(1, (n+1) * sizeof(node_t));

    int x, y;
    for (int i = 0; i < n ; i++) {
        nd_arr[i+1].id = (i+1);
        fscanf(fp, "%d %d", &x, &y);
        nd_arr[i+1].x = x;
        nd_arr[i+1].y = y;
        nd_arr[i+1].u = nd_arr[i+1].v = -1;
        nd_arr[i+1].list = (int *)calloc(1, sz);
    }

    int u, v;
    for (int i = 0; i < n-1 ; i++) {
        fscanf(fp, "%d %d", &u, &v);
        append_node_list(nd_arr, u, v);
    }

#if 0
    for (int i = 1; i <= n; i++) {
        printf("id: %d u: %d v: %d dx1: %d dy1: %d dx2: %d dy2: %d -> ", 
                nd_arr[i].id, nd_arr[i].u, nd_arr[i].v, nd_arr[i].dx1, 
                nd_arr[i].dy1, nd_arr[i].dx2, nd_arr[i].dy2);
        for (int j = 0; j < n ; j++) {
            printf(" %d ", nd_arr[i].list[j]);
        }
        printf("\n");
    }
#endif

    for (int k = 1 ; k <= n; k++) {
        span_graph(nd_arr, k);
    }

    printf("%d \n", count/2);

    return 0;
}

