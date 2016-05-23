//https://leetcode.com/problems/self-crossing/

#include <iostream>
#include <vector>
#include <time.h>

using namespace std;

#define NUM_RANGE 6
#define ARR_SZ 4

int gen_rand(int modulo);

class Solution {
public:
    vector<int> arr;
    void makeRandArr(vector<int> &x);
    bool isSelfCrossing(vector<int>& x) {

      #define PRINT_COORDINATES(str) {\
        cout << str << endl;\
        cout << "cm = x: " << cm.first << " y: " << cm.second << endl;\
        cout << "rl = x: " << rl.first << " y: " << rl.second << endl;\
        cout << "ru = x: " << ru.first << " y: " << ru.second << endl;\
        cout << "lu = x: " << lu.first << " y: " << lu.second << endl;\
        cout << "ll = x: " << ll.first << " y: " << ll.second << endl;\
      }

      pair<int, int> rl, ru, ll, lu, cm;
      rl = ru = ll = lu = cm = make_pair(0, 0);
      int curr_move = 0;
      // N = 0 , W = 1 , S = 2 , E = 3 and so on..
      for (int i = 0 ; i < x.size() ; i++) {
        curr_move = (i%4);
        // going North - r.upper first(x) same as r.lower
        if (curr_move == 0) {
          // first check for collision (rl and ru to be checked)
          cm.first  = rl.first;
          cm.second = rl.second + x[i];
          cout << "\nx[" << i << "]: " << x[i] << " ";
          PRINT_COORDINATES("****** Going North *******");
          if ( (i > 2) &&
               (cm.first >= lu.first && cm.first <= ru.first) &&
               (cm.second >= ru.second)) {
            cout << "Collision !!. Move: " << i << endl;
            return true;
          }
          // Special case for north as there is a case in move 5
          // where going north may collide with (0, 0) [our starting point]
          if ((i == 4) && (cm.first == 0 && cm.second == 0) ) {
            cout << "Collision !!. Move: " << i << endl;
            return true;
          }

          ru.first = cm.first;
          ru.second = cm.second;
        }

        // going West - l.upper second(y) same as r.upper
        if (curr_move == 1) {
          cm.first  = ru.first - x[i];
          cm.second = ru.second;
          cout << "\nx[" << i << "]: " << x[i] << " ";
          PRINT_COORDINATES("****** Going West *******");
          if ( (i > 2) &&
               (cm.second >= ll.second && cm.second <= lu.second) &&
               (cm.first <= lu.first)) {
            cout << "Collision !!. Move: " << i << endl;
            return true;
          }

          lu.first = cm.first;
          lu.second = cm.second;
        }

        // going South - l.lower first(x) same as l.upper
        if (curr_move == 2) {
          cm.first  = lu.first;
          cm.second = lu.second - x[i];
          cout << "\nx[" << i << "]: " << x[i] << " ";
          PRINT_COORDINATES("****** Going South *******");
          if ( (i > 2) &&
               (cm.first >= ll.first && cm.first <= rl.first) &&
               (cm.second <= ll.second)) {
            cout << "Collision !!. Move: " << i << endl;
            return true;
          }

          ll.first  = cm.first;
          ll.second = cm.second;
        }

        // going East - r.lower second(y) same as l.lower
        if (curr_move == 3) {
          // first check for collision (rl and ru to be checked)
          cm.first = ll.first + x[i];
          cm.second = ll.second;
          cout << "\nx[" << i << "]: " << x[i] << " ";
          PRINT_COORDINATES("****** Going East *******");
          if ( (i > 2) &&
               (cm.first >= rl.first) &&
               (cm.second >= rl.second && cm.second <= ru.second) ) {
                 cout << "Collision !!. Move: " << i << endl;
                 return true;
               }

          rl.first = cm.first;
          rl.second = cm.second;
        }
      }

      return false;
    }
};

int main()
{
  Solution s;
  //s.makeRandArr(s.arr);
  s.arr.push_back(1);
  s.arr.push_back(1);
  s.arr.push_back(2);
  s.arr.push_back(1);
  s.arr.push_back(1);

  for (int i = 0; i < ARR_SZ; i++) {
    cout << s.arr[i] << ", ";
  }

  bool a = s.isSelfCrossing(s.arr);
  cout << "Is self crossing: " << a << endl;
  return 0;
}

void
Solution::makeRandArr(vector<int> &x)
{
  for (int i = 0; i < ARR_SZ; i++) {
    x.push_back(gen_rand(NUM_RANGE));
  }
  for (int i = 0; i < ARR_SZ; i++) {
    cout << x[i] << ", ";
  }
  cout << endl;

  return;
}

int gen_rand(int modulo)
{
  static int rand_seeded = 0;

  if (!rand_seeded) {
    srand(time(NULL));
    rand_seeded = 1;
  }

  int r = rand() % modulo;

  return (r?r:r+1);
}
