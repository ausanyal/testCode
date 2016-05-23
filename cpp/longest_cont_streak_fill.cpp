/*
Given an array of 0s and 1s, and k, Find the longest continuous streak of 1s
after flipping k 0s to 1s.

E.x array is {1,1,0,0,1,1,1,0,1,1}
k = 1 (which means we can flip ‘k’ one 0 to 1)

Answer: 6 (if we flip 0 at index 7, we get the longest continuous streak of 1s
having length 6)
*/

#include <iostream>

using namespace std;

void getLongest(int *arr, int start, int sz, int k, int *maxCount) {

  int t = k, currCount = 0, lastFilled = 0;

  cout << "start= " << start << " mc = " << *maxCount << endl;

  for (int i = start; i < sz; i++) {
    if (arr[i]) {

      currCount++;
      cout << __LINE__ << ": i = " << i << " currCount = " << currCount << endl;

      if (currCount > *maxCount)
        *maxCount = currCount;
    } else {
      cout << __LINE__ << ": i = " << i << " currCount = " << currCount << endl;
      if (t) {

        if (t == k)
          lastFilled = i;

        t--;
        currCount++;
        cout << __LINE__ << ": i = " << i << " currCount = " << currCount
             << endl;

        if (currCount > *maxCount)
          *maxCount = currCount;

      } else {
        cout << __LINE__ << ": lf = " << lastFilled << endl;
        getLongest(arr, lastFilled + 1, sz, k, maxCount);
        return;
      }
    }
  }
}

int main() {
  int arr[] = {1, 1, 0, 0, 1, 1, 1, 0, 1, 1};
  int maxCount = 0;
  getLongest(arr, 0, sizeof(arr) / sizeof(arr[0]), 1, &maxCount);
  cout << "maxCount: " << maxCount << endl;
  return 0;
}
