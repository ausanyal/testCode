/*
http://www.practice.geeksforgeeks.org/problem-page.php?pid=106
Given an array containing both negative and positive integers. Find the
contiguous sub-array with maximum sum.

Input:
The first line of input contains an integer T denoting the number of test cases.
The description of T test cases follows. The first line of each test case
contains a single integer N denoting the size of array. The second line contains
N space-separated integers A1, A2, ..., AN denoting the elements of the array.

Output:
Print the maximum sum of the contiguous sub-array in a separate line for each
test case.

Constraints:
1 ≤ T ≤ 40
1 ≤ N ≤ 100
-100 ≤ A[i] <= 100

Example:
Input
2
3
1 2 3
4
-1 -2 -3 -4

Output
6
-1
*/

#include <iostream>

using namespace std;

int maxSubArr(int *arr, int sz) {
  int max = 0, max_so_far = 0;
  for (int i = 0; i < sz; i++) {
    max += arr[i];
    if (max < 0)
      max = 0;
    else if (max_so_far < max)
      max_so_far = max;
  }
  return max_so_far;
}

int main() {
  int tests, sz;
  cin >> tests;
  for (int t = 0; t < tests; t++) {
    cin >> sz;
    int *arr = (int *)malloc(sizeof(int));
    for (int i = 0; i < sz; i++) {
      cin >> arr[i];
      cout << maxSubArr(arr, sz) << endl;
    }
    free(arr);
  }

  return 0;
}
