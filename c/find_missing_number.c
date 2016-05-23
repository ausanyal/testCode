#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 1000

void build_arr(int *arr)
{
  int r = rand() % SIZE;
  printf("r = %d \n", r);
  int count = 0;
  for (int i = 0; i < SIZE; i++) {
    if (i == r) {
      count++;
    }
    arr[i] = count++;
  }

  //for (int i = 0; i < SIZE; i++) {
    //printf("%d \n", arr[i]);
  //}

  return;
}

int find_missing(int *arr, int size)
{
  int start = 0, end = size-1, mid = 0;

  while(start < end-1) {
    mid = (start+end) / 2;
  //  printf("start: %d (%d) end: %d(%d) mid: %d(%d) \n",
  //      start, arr[start], end, arr[end], mid, arr[mid]);
    if (arr[mid] == mid) {
      start = mid+1;
    } else {
      end = mid;
    }
  }

  if (arr[start] != start) {
    printf("missing: %d \n", arr[start]-1);
    return (arr[start]-1);
  } else if (arr[end] != end) {
    printf("..missing: %d \n", arr[end]-1);
    return arr[end]-1;
  } else {

    printf("Nothing missing \n");
    return -1;
  }

  return 0;
}

int main()
{
  srand(time(NULL));
  int arr[SIZE];

  build_arr(arr);
  find_missing(arr, SIZE);

  return 0;
}
