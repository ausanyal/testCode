/*
Given a series of number form a binary tree find the minimum weight binary tree.
The weight of the node is depth * value of the element + weight of the left tree
+ weight of the right tree.

Weight of the root node is the weight of the tree . Find the minimum weight
binary tree out of all possible binary trees that are possible.

*/

#include <iostream>

using namespace std;

int computeMinWt(int *arr, int idx, int d, int n) {
  if (idx >= n)
    return 0;

  int l = computeMinWt(arr, 2 * idx + 1, d + 1, n);
  int r = computeMinWt(arr, 2 * idx + 2, d + 1, n);

  int wt = d * arr[idx] + l + r;

  return wt;
}

int main() {
  // int arr[] = {60, 10, 43, 8, 5, 20, 3, 2, 2, 1, 1};
  int arr[] = {43, 20, 10, 2, 3, 1, 8};
  int sz = sizeof(arr) / sizeof(arr[0]);
  sort(arr, arr + sz, greater<int>());
  int min_wt = computeMinWt(arr, 0, 0, sz);
  cout << min_wt << endl;
  return 0;
}
