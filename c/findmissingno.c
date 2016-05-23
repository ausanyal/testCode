#include <stdio.h>

int
find_missing(int *arr, int beg, int end)
{
	if ((end - beg) == 1) {
		if (arr[end] - arr[beg] != 1) {
			return (arr[beg] + 1);
		} else {
			return -1;
		}
	}
	if (end == beg) {
		if (arr[end] - arr[end-1] != 1) {
			return (arr[end-1] + 1);
		} else {
			return -1;
		} 
	}

	int mid = (beg + end) / 2;

	int n1 = find_missing(arr, beg, mid);
	if (n1 != -1) {
		return n1;
	}
	
	return find_missing(arr, mid+1, end);

}

int
find_iter_missing(int *arr, int n)
{
	for (int i = 1; i < n; ++i ) {
		if (arr[i] - arr[i-1] != 1) {
			return (arr[i-1] + 1); 
		}
	}

	return -1;
}

int main()
{
	int arr[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 11};

	int n2 = find_iter_missing(arr, sizeof(arr));
	int n1 = find_missing(arr, 0, sizeof(arr)-1);
	printf("missing = %d ; %d \n", n1, n2);

	return 1;
}