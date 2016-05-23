#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int heap[100];
int size = 0;

void print_heap()
{
    for (int i = 0 ; i < size ; i++) {
        printf("i: %d v: %d \n", i, heap[i]);
    }
}

void swap(int p, int i)
{
    int temp = heap[i];
    heap[i] = heap[p];
    heap[p] = temp;
}

void heapify(int i)
{
    printf("heapify called with id: %d \n", i);
    if(i == 0) return;
    int p = (i-1)/2;
    if (heap[p] > heap[i]) swap(p, i);
    heapify(p);
}

void heap_ins(int i)
{
    heap[size++] = i;
    printf("inserted %d new size: %d \n", i, size);
    heapify(size-1);

    print_heap();

}

void trickle_down(int i)
{
    //printf("trickle down called with idx: %d size: %d \n", i, size);
    if (i == size-1) return;

    int l = (2*i) + 1;
    int r = (2*i) + 2;
    int smallest = i;
    if (l < size) { 
        if (heap[l] < heap[i]) smallest = l; 
    }
    if (r < size) { 
        if (heap[r] < heap[smallest]) smallest = r; 
    }
    //printf("i: %d (%d) l: %d(%d) r: %d(%d) sm: %d(%d)\n", 
    //        i, heap[i], l, heap[l], r, heap[r], smallest, heap[smallest]);
    if(smallest != i) {
        swap(i, smallest);
        trickle_down(smallest);
    }
}

int extract_min()
{
    int min = heap[0];
    swap(0, size-1);
    heap[size-1] = -1;
    size--;
    trickle_down(0);
    return min;
}

int main()
{
    FILE *fp = fopen("heap_srt_input.txt", "r");
    memset(heap, 0, sizeof(heap));
    int i =0;
    while(1) {
        fscanf(fp, "%d", &i);
        if (i == -1) {
            break;
        }
        heap_ins(i);
    }

    printf(" min = %d size: %d \n", extract_min(), size);
    printf(" min = %d size: %d \n", extract_min(), size);
    printf(" min = %d size: %d \n", extract_min(), size);
    printf(" min = %d size: %d \n", extract_min(), size);
    printf(" min = %d size: %d \n", extract_min(), size);
    printf(" *********min = %d size: %d \n", extract_min(), size);
    printf(" *********min = %d size: %d \n", extract_min(), size);
    printf(" *********min = %d size: %d \n", extract_min(), size);
    printf(" *********min = %d size: %d \n", extract_min(), size);
    print_heap();

    return 0;

}
