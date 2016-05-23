#include <stdio.h>
#include <stdlib.h>

int **m;
int i, j;

void build_matrix() 
{
    m = (int **)malloc(i * sizeof(int *));
    for (int t = 0; t < i ; t++) {
        m[t] = (int *)malloc(j * sizeof(int));
    }
}

int get_rand()
{
    int x = rand();
    x = x%2;
    printf("x=%d\n", x);
    return x;
}

void fill_matrix()
{
    for (int r = 0; r < i; r++) {
        for (int t = 0; t < j; t++) {
            m[r][t] = get_rand();
        }
    }
}

void print_matrix()
{
    for (int r = 0; r < i; r++) {
        for (int t = 0; t < j; t++) {
            printf("%d ", m[r][t]);
        }
        printf("\n");
    }

}

int validate(int q, int w) {
    if ( (q < i) && (w < j)) {
        return 1;
    }
    return 0;
}


int find_max_sub(int x, int y, int *hops)
{
    if (m[x][y]) {
        // check sub-matrix starting from x, y
        if (m[x+1][y+1]) {
        }
    }
}

int main()
{
    scanf("%d %d", &i, &j);

    build_matrix();
    fill_matrix();
    print_matrix();

    for (int x = 0 ; x < i ; x++) {
        for (int y = 0; y < j; y++) {
            int hops = 0;
            find_max_sub(x, y, &hops);

        }
    }

    return 0;
}
