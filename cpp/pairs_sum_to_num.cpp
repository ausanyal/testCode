/*
given an integer array , find all pairs which sum to a given number. If a number
is used once, it must not be used again.
eg if input array is 6444 and sum =10
output must be just 6 4
Give an O(n) solution

*/

#include <iostream>
#include <vector>

using namespace std;

void findCombs(int *arr, int sz, int num) {
  vector<vector<int>> sol;

  vector<int> start, t;
  start.clear();

  int consumed = 0;

  for (int i = 0; i < sz; i++) {
    consumed = 0;
    t.clear();
    for (int j = 0; j < start.size(); j++) {

      if ((start[j] + arr[i]) == num) {
        t.push_back(start[j]);
        t.push_back(arr[i]);
        start.erase(start.begin() + j);
        consumed = 1;
        break;
      }
    }
    if (!consumed)
      start.push_back(arr[i]);
    else
      sol.push_back(t);
  }
  for (int k = 0; k < sol.size(); k++) {
    for (int h = 0; h < sol[k].size(); h++) {
      cout << sol[k][h] << " ";
    }
    cout << endl;
  }
}

int main() {
  int input[] = {7, 6, 5, 4, 3, 2, 1};

  findCombs(input, sizeof(input) / sizeof(input[0]), 10);

  return 0;
}
