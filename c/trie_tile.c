#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NUM_ENTRIES 10
#define THRESHOLD_WEIGHT 500

typedef struct entry_info_ {
    int prefix_len;
    int weight;
} entry_info_t;

entry_info_t entry_arr[NUM_ENTRIES];

int cmp_fn(const void *a, const void *b)
{
    entry_info_t *entry1 = (entry_info_t *)a;
    entry_info_t *entry2 = (entry_info_t *)b;

    return (entry2->weight - entry1->weight);
}

void
init_entries()
{
    int r;

    srand(time(NULL));

    for (int ent_num = 0 ; ent_num < NUM_ENTRIES ; ent_num++) {
        r = rand();
        entry_arr[ent_num].prefix_len = r % 32;
        r = rand();
        entry_arr[ent_num].weight = r % 3000;
        printf("(%d) addr: %p prefix_len: %d \t\t weight: %d \n", 
                ent_num, &entry_arr[ent_num], entry_arr[ent_num].prefix_len, entry_arr[ent_num].weight);
    }

    qsort(entry_arr, NUM_ENTRIES, sizeof(entry_info_t), cmp_fn);
    printf("*********************\n");

    for (int ent_num = 0 ; ent_num < NUM_ENTRIES ; ent_num++) {
        printf("prefix_len: %d \t\t weight: %d \n", entry_arr[ent_num].prefix_len, entry_arr[ent_num].weight);
    }
}

int main()
{
    init_entries();
    return 0;
}
