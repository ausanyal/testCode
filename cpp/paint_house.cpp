/*
http://www.lintcode.com/en/problem/paint-house/
There are a row of n houses, each house can be painted with one of the three
colors: red, blue or green. The cost of painting each house with a certain color
is different. You have to paint all the houses such that no two adjacent houses
have the same color.

The cost of painting each house with a certain color is represented by a n x 3
cost matrix. For example, costs[0][0] is the cost of painting house 0 with color
red; costs[1][2] is the cost of painting house 1 with color green, and so on...
Find the minimum cost to paint all houses.

Example
Given costs = [[14,2,11],[11,14,5],[14,3,10]] return 10

house 0 is blue, house 1 is green, house 2 is blue, 2 + 5 + 3 = 10
*/

#include <iostream>
#include <queue>

using namespace std;

typedef struct node_ {
  int val;
  struct node_ *l;
  struct node_ *r;
} node_t;

node_t *getNode(int val) {
  node_t *n = (node_t *)malloc(sizeof(node_t));
  n->val = val;
  n->l = NULL;
  n->r = NULL;
  return n;
}

void build_tree(int (*costs)[3], node_t *r, int i, int j) {

  int lidx = -1, ridx = -1;

  if (j >= 3)
    return;

  for (int k = 0; k < 3; k++) {
    if (k == i)
      continue;
    if (!r->l) {
      r->l = getNode(costs[j][k]);
      lidx = k;
      cout << __LINE__ << " i=" << i << " j=" << j << " k=" << k
           << " storing in lidx=" << costs[j][k] << endl;
    } else if (!r->r) {
      r->r = getNode(costs[j][k]);
      ridx = k;
      cout << __LINE__ << " i=" << i << " j=" << j << " k=" << k
           << " storing in ridx=" << costs[j][k] << endl;
    }
  }

  build_tree(costs, r->l, lidx, j + 1);
  build_tree(costs, r->r, ridx, j + 1);

  return;
}

queue<node_t *> myq;
void walk_tree(node_t *t1) {
  if (!t1)
    return;

  node_t *temp;
  myq.push(t1);

  while (!myq.empty()) {
    temp = myq.front();
    cout << temp->val << " ";
    myq.pop();
    if (temp->l)
      myq.push(temp->l);
    if (temp->r)
      myq.push(temp->r);
  }
}

int findMinCost(node_t *r) {
  int currCost = 0;

  if (!r)
    return 0;

  int lc = findMinCost(r->l);
  int rc = findMinCost(r->r);
  currCost = r->val + min(lc, rc);
  // cout << r->val << " " << lc << " " << rc << "=" << currCost << endl;
  return currCost;
}

int main() {
  int costs[3][3] = {{14, 2, 11}, {11, 14, 5}, {14, 3, 10}};
  node_t *t[3];
  int paintCost;

  for (int i = 0; i < 3; i++) {
    t[i] = getNode(costs[0][i]);
    build_tree(costs, t[i], i, 1);
    cout << "walking : " << i << ": \n";
    walk_tree(t[i]);
    cout << endl;
  }
  int minCost = INT_MAX;
  for (int i = 0; i < 3; i++) {
    paintCost = findMinCost(t[i]);
    if (paintCost < minCost)
      minCost = paintCost;
  }

  cout << "min cost = " << minCost << endl;

  return 0;
}
