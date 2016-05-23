/*
You are given a graph and a node in the graph. Group the nodes connected to this
node if they are also connected to each other. For example, the graph has nodes
1, 2, 3, 4, 5 where 1 is connected to 2, 3, 4; 2 and 3 are also connected to
each other, 4 is just connected to 1 and 5 is a separate node. You are given
node 1 as input. Output should be:
2 3
4
*/

#include <iostream>
#include <list>
#include <vector>

using namespace std;
typedef list<int>::iterator myList_it;

class Solution {
public:
  vector<list<int>> adj_l;

  void group_related_nodes(vector<list<int>> &adj_l, int v) {

    list<int> &l = adj_l.at(v - 1);

    vector<int> connected;

    for (myList_it it = l.begin(); it != l.end(); it++) {
      int n = *it;
      // cout << " " << n;
      connected.push_back(n);
    }
    cout << endl;

    vector<vector<int>> output;
    vector<int> result;

    while (connected.size()) {
      result.clear();
      int tmp = connected.back();
      connected.pop_back();
      result.push_back(tmp);
      // cout << __LINE__ << ": got tmp: " << tmp << endl;
      list<int> l = adj_l.at(tmp - 1);
      for (myList_it it = l.begin(); it != l.end(); it++) {
        // cout << __LINE__ << ": *it: " << *it << endl;
        for (int i = 0; i < connected.size(); i++) {
          // cout << __LINE__ << ": connected[" << i << "]: " << connected[i]
          //     << endl;
          if ((*it) == connected[i]) {
            // cout << __LINE__ << ": pushing to result: " << tmp << endl;
            result.push_back(*it);
            connected.erase(connected.begin() + i);
          }
        }
      }
      for (int j = 0; j < result.size(); j++) {
        cout << " " << result[j];
      }
      cout << endl;
    }
  }

  void build_graph(vector<list<int>> &adj_l);
};

int main() {
  Solution s;

  s.build_graph(s.adj_l);
  s.group_related_nodes(s.adj_l, 1);

  return 0;
}

void Solution::build_graph(vector<list<int>> &adj_l) {

  int nv;

  cout << "Num vertices: \n";

  cin >> nv;

  int v1, v2;
  do {
    cout << "\nEnter v1: ";
    cin >> v1;
    cout << "\nEnter v2: ";
    cin >> v2;

    if (v1) {
      try {
        list<int> &l = adj_l.at(v1 - 1);
        l.push_back(v2);
      } catch (const out_of_range &e) {
        list<int> l;
        l.push_back(v2);
        adj_l.push_back(l);
      }
    }

    if (v2) {
      try {
        list<int> &l = adj_l.at(v2 - 1);
        l.push_back(v1);
      } catch (const out_of_range &e) {
        list<int> l;
        l.push_back(v1);
        adj_l.push_back(l);
      }
    }
  } while (v1 || v2);

  for (int i = 0; i < adj_l.size(); i++) {
    cout << "Vertex: " << i + 1 << " : ";
    for (myList_it it = adj_l[i].begin(); it != adj_l[i].end(); it++) {
      cout << *it << " -> ";
    }
    cout << endl;
  }
  cout << endl;
}
