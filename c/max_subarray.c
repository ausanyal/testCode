#include <stdio.h>

int arr1[] = {0, 1 ,2 , -1, -7, 1, 2, 3 };
int arr2[] = {0, 1 ,2 , -1,  1, 2, 3 };

find_max_subarray(int arr[], int num_elems)
{
	int i;
	int curr_count = 0, max_count = 0;
	int curr_start = 0, curr_end = 0, reset = 0;
	int final_start = 0, final_end = 0;

	for (i = 0; i < num_elems; i++) {

		// Check if we need to reset the running start and end index
		if(reset) {
			curr_start = i;
			curr_end = i;
			reset = 0;
		}

		//Add to the running count the value from array 
		curr_count += arr[i];

		// Since there would be atleast 1 positive integer... we update max_count only when
		// current count exceeds 0
		if(curr_count > 0) {
			// Check if running count exceeded the maximum seen so far
			if(curr_count > max_count) {
				max_count = curr_count;
				// We update the end index as we see each time new max_count is seen
				curr_end = i;
				// Update the final values too as we go
				final_start = curr_start;
				final_end = curr_end;
			}
		}
		else {
			// this is this boundary case... we need to reset the running start ; end index and count
			reset = 1;
			curr_count = 0;
		}


	}
	printf("Max_count = %d ; start_index = %d ; end_index = %d \n\n", max_count, final_start, final_end);
}

int main()
{
    find_max_subarray(arr1, sizeof(arr1)/sizeof(int));
    find_max_subarray(arr2, sizeof(arr2)/sizeof(int));
	return 1;
}
