// https://leetcode.com/problems/counting-bits/
#include <iostream>
#include <vector>

using namespace std;

class Solution {
public:
    vector<int> countBits(int num) {
        int curr_pow_of_2 = 2, delta;
        vector<int> arr;

        arr.push_back(0);
        if (num == 0) return arr;
        arr.push_back(1);
        if (num == 1) return arr;
        
        for (int i = 2; i <= num; i++) {
          // check if i is !0 and is power of 2
          if ((i > 1) && ( (i & (i-1)) == 0 ) ) {
            curr_pow_of_2 = i;
            arr.push_back(1);
          } else {
            delta = i - curr_pow_of_2;
            arr.push_back(1+arr[delta]);
          }
        }
        return arr;
    }
};

int main()
{
    Solution s;
    int num;
    vector<int> res;

    cout << "Enter num: ";
    cin >> num;
    cout << "Res: \n";
    res = s.countBits(num);
    for (int i = 0; i < res.size(); i++) {
      cout << res[i] << " ,";
    }
    cout << endl;

    return 0;
}
