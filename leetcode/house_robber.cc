#include <iostream>
#include <vector>
#include <time.h>

using namespace std;

#define MAX_HOUSES 5
#define MAX_WEALTH 10



class Solution {
public:

    void build_houses(vector<int>& nums);
    int *cache;
    int rob_house(vector<int>& nums, int i) {

      if ( i < 0 || (i >= nums.size()) ) {
        cout << "i: " << i << " robbing: 0" <<  endl;
        return 0;
      }

      int wc = 0;
      cout << "\ni: " << i << " cache[" << i+2 << "]: " << cache[i+2]
                           << " cache[" << i+3 << "]: " << cache[i+3] << endl;
      cache[i+2] = (cache[i+2] == -1) ? rob_house(nums, i+2) : cache[i+2];
      cache[i+3] = (cache[i+3] == -1) ? rob_house(nums, i+3) : cache[i+3];
      cout << "************** Cache : " << endl;
      for (int i = 0; i < nums.size()+3; i++) {
        cout << " " << cache[i];
      }
      cout << endl;

      wc = max(nums[i] + cache[i+2],
               nums[i] + cache[i+3]);
      cout << "i: " << i << " wc: " << wc << endl;
      return wc;

    }

    int rob(vector<int>& nums) {
      cache = (int *)malloc((nums.size() + 3) * sizeof(int));
      for (int i = 0; i < nums.size()+3; i++) {
        //cout << "setting -1 on i: " << i <<  endl;
        cache[i] = -1;
      }
      int w1 = rob_house(nums, 0);
      int w2 = rob_house(nums, 1);
      cout << "w1: " << w1 << " w2: " << w2 << endl;
      return max(w1, w2);
    }

    int rob2(vector<int>& nums) {
      int n = nums.size();
      if (n == 0) return 0;
      if (n == 1) return nums[0];
      
      vector<int> dp(n, 0);
      dp[0] = nums[0];
      dp[1] = max(nums[0], nums[1]);
      for(int i = 2; i < n; i++) {
        dp[i] = max(nums[i] + dp[i-2], dp[i-1]);
      }

      return dp.back();
    }
};

void
Solution::build_houses(vector<int>& nums) {
  for (int i = 0; i < MAX_HOUSES; i++) {
    int r = rand() % MAX_WEALTH;
    nums.push_back(r);
  }
  for (int i = 0; i < MAX_HOUSES; i++) {
    cout << nums[i] << " ";
  }
  cout << endl;
}

int main()
{
  Solution s;
  vector<int> nums;

  srand(time(NULL));
  //s.build_houses(nums);

  nums.push_back(2);
  nums.push_back(7);
  nums.push_back(50);
  nums.push_back(1150);
  nums.push_back(10000);
  for (int i = 0; i < MAX_HOUSES; i++) {
    cout << nums[i] << " ";
  }
  cout << endl;

  int w = s.rob(nums);
  cout << "\nMax value: " << w << endl;

  w = s.rob2(nums);
  cout << "\nMax value: " << w << endl;


  return 0;
}
