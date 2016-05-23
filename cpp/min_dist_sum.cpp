/*
Find the minimum (index) distance sum of 3 words. For example: arr = {"2", "1",
"0", "2", "0", "3", "0"}, input = "1","2","3". The result should be 8 since the
2nd "2" and "1", "3"'s distance are 3, 1, 5 and abs(3,1)+abs(3,5)+abs(5,1)=8.
Implement this in O(N)
*/
#include <iostream>

using namespace std;

int minDist(char *arr, int sz, char a, char b, char c) {

  int ai = -1, bi = -1, ci = -1;
  int minDist = INT_MAX;

  for (int i = 0; i < sz; i++) {
    if (arr[i] == a)
      ai = i;
    else if (arr[i] == b)
      bi = i;
    else if (arr[i] == c)
      ci = i;
    else
      continue;

    if (ai >= 0 && bi >= 0 && ci >= 0) {
      cout << abs(ai - bi) << " " << abs(ai - ci) << " " << abs(bi - ci)
           << endl;
      minDist = min(minDist, abs(ai - bi) + abs(ai - ci) + abs(bi - ci));
    }
  }

  return minDist;
}

int main() {

  char arr[] = {'2', '1', '0', '2', '0', '3', '0'};

  int m = minDist(arr, sizeof(arr), '1', '2', '3');

  cout << "dist: " << m << endl;

  return 0;
}
