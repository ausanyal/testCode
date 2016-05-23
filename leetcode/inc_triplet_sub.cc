//https://leetcode.com/problems/increasing-triplet-subsequence/

#include <iostream>
#include <vector>
#include <time.h>

using namespace std;

#define NUM_ENTRIES 10

int get_rand_num();

class Solution {
public:
    vector<int> nums;

    void build_array(vector<int>& nums) {
      for (int i = 0; i < NUM_ENTRIES; i++) {
        nums.push_back(get_rand_num());
      }

      for (int i = 0; i < NUM_ENTRIES; i++) {
        cout << " " << nums[i];
      }
      cout << endl;
    }

    void print_first_three(std::vector<int> nums) {
      cout << "0: " << nums[0] << " 1: " << nums[1] << " 2:" << nums[2] << endl;

    }

#if 0
    class Solution {
    public:
        bool increasingTriplet(vector<int>& nums) {
            const int K=2;
            int len=nums.size(), i, j, minL[K];
            fill_n(minL, K, INT_MAX);

            for(i=0; i<len;++i)
            {
                for(j=0;j<K && nums[i]>minL[j];++j);
                if(j==K) return true;
                else minL[j] = nums[i];
            }
            return false;
        }
    };
#endif

    bool increasingTriplet(vector<int>& nums) {
      if (nums.size() < 3) {
        cout << "Too few args" << endl;
        return false;
      }

      int t_arr[3], j;
      for (int i = 0; i < 3; i++) {
        t_arr[i] = INT_MAX;
        cout << "max: " << t_arr[i] << endl;
      }

      for (int i = 0; i < nums.size(); i++) {
        for (j = 0; (j < 3) && (nums[i] > t_arr[j]); ++j);

        cout << "Storing " << nums[i] << " in " << j << endl;
        t_arr[j] = nums[i];

        if (j == 2) {
          cout << "found: " << t_arr[0] << " " << t_arr[1] << " " << t_arr[2] << endl;
          return true;
        }

      }
      return false;
    }
};

int main()
{
  Solution s;

  srand(time(NULL));

  //s.build_array(s.nums);
  s.nums.push_back(10);
  //s.nums.push_back(5);
  s.nums.push_back(1000);
  //s.nums.push_back(30);
  //s.nums.push_back(1);
  s.nums.push_back(50);
  s.nums.push_back(7);
  //s.nums.push_back(55);
  //s.nums.push_back(61);
  s.nums.push_back(51);

  s.increasingTriplet(s.nums);

  return 0;
}

int get_rand_num()
{
  return rand()%1000;
}
