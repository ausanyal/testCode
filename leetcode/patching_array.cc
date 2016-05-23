// https://leetcode.com/problems/patching-array/
#include <iostream>
#include <vector>
#include <string>
#include <stdlib.h>

using namespace std;

class Solution {
public:
  void print_arr(vector<int> &n) {
    // cout << "arr: \n";
    for (int k = 0; k < n.size(); k++) {
      cout << ">>" << n[k] << " ";
    }
    cout << endl;
  }

  int minPatches(vector<int> &nums, int n) {
    long miss = 1;
    int i = 0, added = 0;

    while (miss <= n) {
      // cout << "miss : " << miss << " i:" << i << " nums[i]: " << nums[i]
      //     << endl;
      if ((i < nums.size()) && (nums[i] <= miss)) {
        // cout << "<<< num to be added: " << nums[i] << endl;
        miss += nums[i++];
      } else {
        cout << ">>> miss to be added: " << miss << endl;
        miss += miss;
        added++;
      }
    }

    return added;
  }
};

// void permut(std::string)

int main() {
  Solution s;
  vector<int> nums;
  nums.push_back(1);
  nums.push_back(2);
  nums.push_back(31);
  nums.push_back(33);
  // nums.push_back(5);
  int added = s.minPatches(nums, 2147483647);
  cout << "Added: " << added << endl;
  return 0;
}
