/*
This is was asked in Amazon SDE online test from Hacker rank.
Initech is a company which has CEO Bill and a hierarchy of employees. Employees
can have a list of other employees reporting to them, which can themselves have
reports, and so on. An employee with at least one report is called a manager.

Please implement the closestCommonManager method to find the closest manager
(i.e. farthest from the CEO) to two employees. You may assume that all employees
eventually report up to the CEO.

Tree structure:
Bill -> Dom, Samir, Michael
Dom -> Bob, Peter, Porter
Peter -> Milton, Nina

Sample Data:
CEO Bill has 3 employees reporting to him: {Dom, Samir, Michael}
Dom has three reports { Peter, Bob, Porter}
Samir has no reports {}
Michael has no reports {}
Peter has 2 reports {Milton, Nina}
Bob has no reports {}
Porter has no reports {}
Milton has no reports {}
Nina has no reports {}

Sample calls:
closestCommonManager(Milton, Nina) = Peter
closestCommonManager(Nina, Porter) = Dom
closestCommonManager(Nina, Samir) = Bill
closestCommonManager(Peter, Nina) = Peter
*/

#include <iostream>
#include <queue>

using namespace std;

typedef struct node_ {
  struct node_ **r;
  string name;
  int n;
} node_t;

void doBfs(node_t *root);

node_t *getNode(string name, int num) {
  node_t *n = (node_t *)malloc(sizeof(node_t));
  n->name = name;
  n->r = (node_t **)malloc(num * sizeof(node_t *));
  for (int i = 0; i < num; i++) {
    n->r[i] = NULL;
  }
  n->n = num;
  return n;
}

bool hasEmp(node_t *root, string s) {
  if (!root)
    return false;

  if (root->name == s)
    return true;

  for (int i = 0; i < root->n; i++) {
    if (hasEmp(root->r[i], s))
      return true;
  }
  return false;
}

node_t *findClosestManager(node_t *root, string s1, string s2) {
  if (!root)
    return NULL;

  node_t *closesManager = root;

  if (!hasEmp(root, s1) || !hasEmp(root, s2))
    return NULL;

  queue<node_t *> empq;
  empq.push(root);

  node_t *temp, *e;
  while (!empq.empty()) {
    temp = empq.front();
    empq.pop();
    if (temp) {
      for (int i = 0; i < temp->n; i++) {
        e = temp->r[i];
        if (hasEmp(e, s1) && hasEmp(e, s2) && e->name != s1 && e->name != s2) {
          closesManager = e;
          empq.push(e);
        }
      }
    }
  }
  return closesManager;
}

int main() {
  node_t *root, *t, *v;

  queue<node_t *> *myqs = new queue<node_t *>[5];

  root = getNode("Bill", 3);
  root->r[0] = getNode("Dom", 3);
  root->r[1] = getNode("Samir", 0);
  root->r[2] = getNode("Michael", 0);

  t = root->r[0];
  t->r[0] = getNode("Peter", 2);
  t->r[1] = getNode("Bob", 0);
  t->r[2] = getNode("Porter", 0);

  v = t->r[0];
  v->r[0] = getNode("Milton", 0);
  v->r[1] = getNode("Nina", 0);

  doBfs(root);
  node_t *x = findClosestManager(root, "Porter", "Nina");

  cout << x->name << endl;

  return 0;
}

queue<node_t *> myq;

void doBfs(node_t *root) {

  node_t *temp;

  if (!root)
    return;

  myq.push(root);

  while (!myq.empty()) {
    cout << myq.front()->name << " *** " << endl;
    temp = myq.front();
    myq.pop();
    // cout << temp->n << endl;
    int i = 0;
    while (i < temp->n && temp->r[0]) {
      // cout << "Pushing: " << temp->r[i]->name << endl;
      myq.push(temp->r[i]);
      i++;
    }
  }
  cout << endl;
}
