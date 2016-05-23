//https://leetcode.com/problems/self-crossing/

#include <iostream>
#include <set>
#include <vector>
#include <time.h>

using namespace std;

#define NUM_RANGE 6
#define ARR_SZ 4
#define PRINT_COORDINATES(str) {\
  cout << str << endl;\
  cout << "cm = x: " << cm.first << " y: " << cm.second << endl;\
  cout << "rl = x: " << rl.first << " y: " << rl.second << endl;\
  cout << "ru = x: " << ru.first << " y: " << ru.second << endl;\
  cout << "lu = x: " << lu.first << " y: " << lu.second << endl;\
  cout << "ll = x: " << ll.first << " y: " << ll.second << endl;\
}

int gen_rand(int modulo);

class Solution {
public:

    vector<int> arr;
    typedef struct edge_ {
      pair<int, int> p1; // point1
      pair<int, int> p2; // point2
    } edge;

    struct comp_edge {
      bool operator() (const edge &a, const edge &b) {
        int p1_x = (a.p1.first < a.p2.first) ? a.p1.first : a.p2.first;
        int p2_x = (b.p1.first < b.p2.first) ? b.p1.first : b.p2.first;
        return (p1_x < p2_x);
      }
    };

    typedef std::multiset<edge, comp_edge> edges;
    typedef std::multiset<edge>::iterator it;
    typedef pair<int, int> pt;

    edges all_edges;
    pt prev_p = make_pair(0, 0);
    edge e;

    void print_all_edges() {
      cout << "Edges: \n";
      for (it i = all_edges.begin(); i != all_edges.end(); i++) {
        cout << i->p1.first << "," << i->p1.second << " -> "
             << i->p2.first << "," << i->p2.second << endl;
      }
    }

    void print_misc(string str, int val)
    {
      cout << endl << str << ": Inserting: " << val << ": "
        << e.p1.first << "," << e.p1.second << " -> "
        << e.p2.first << "," << e.p2.second << endl;
    }

    void makeRandArr(vector<int> &x);

    bool check_cross_h(pt ref_p1, pt ref_p2)
    {
      pt p1, p2;
      for (it i = all_edges.begin(); i != all_edges.end(); i++) {
        if (i->p1.first <= i->p2.first) {
          p1 = i->p1;
          p2 = i->p2;
        } else {
          p1 = i->p2;
          p2 = i->p1;
        }

        cout << "ref_p1: " << ref_p1.first << "," << ref_p1.second << endl;
        cout << "ref_p2: " << ref_p2.first << "," << ref_p2.second << endl;
        cout << "    p1: " << p1.first << "." << p1.second << endl;
        cout << "    p2: " << p2.first << "," << p2.second << endl;

        if ( (ref_p1.first >= p1.first) &&
             (ref_p1.first <= p2.first) ) {

          if ( (ref_p1.second <= p1.second) &&
               (ref_p2.second >= p1.second)) {
                 cout << "Collision \n";
               }
        }
      }
      return false;
    }

    bool isSelfCrossing(vector<int>& x) {

      int curr_move = 0;

      for (int i = 0 ; i < x.size() ; i++) {

        curr_move = (i%4);

        if (curr_move == 0) { // North

          e.p1 = prev_p;
          e.p2.first = prev_p.first;
          e.p2.second = prev_p.second + x[i];

          ve.insert(e);
          prev_p = e.p2;

          print_misc("North", x[i]);
          print_ve();

          check_cross_h(e.p1, e.p2);

        } else if (curr_move == 1) { // East

          e.p1 = prev_p;
          e.p2.first = prev_p.first - x[i];
          e.p2.second = prev_p.second;
          all_edges.insert(e);
          prev_p = e.p2;

          print_misc("East", x[i]);
          print_he();

        } else if (curr_move == 2) { // South

          e.p1 = prev_p;
          e.p2.first = prev_p.first;
          e.p2.second = prev_p.second - x[i];

          ve.insert(e);
          prev_p = e.p2;

          print_misc("South", x[i]);
          print_ve();

        } else { // West

          e.p1 = prev_p;
          e.p2.first = prev_p.first + x[i];
          e.p2.second = prev_p.second;
          all_edges.insert(e);
          prev_p = e.p2;

          print_misc("West", x[i]);
          print_he();

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
