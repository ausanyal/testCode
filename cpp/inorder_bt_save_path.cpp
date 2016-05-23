/*
Binary search inorder traversal asked by Amazon
struct Node
{
int data;
Node *right.*left,*random
}

Tree should be in-order traversal and random node should keep the in-order
transversal path.
*/

#include <iostream>
#include <list>

using namespace std;

typedef struct node_ {
  int data;
  struct node_ *l;
  struct node_ *r;
  list<struct node_ *> ra;
  node_(int x) { data = x; }
} node_t;

node_t *get_node(int data) {
  node_t *n = new node_t(data);
  n->l = n->r = NULL;
  return n;
}

typedef list<struct node_ *> lnodep;
typedef list<struct node_ *>::iterator it;
void print_path(node_t *root) {

  if (!root)
    return;

  cout << "data: " << root->data << " size:" << root->ra.size() << endl;
  for (it i = root->ra.begin(); i != root->ra.end(); i++) {
    cout << (*i)->data << " ";
  }
  cout << endl;
}

lnodep inorder_path(node_t *root) {
  lnodep temp;
  if (!root)
    return temp;

  // cout << "(1) In: " << root->data << endl;

  lnodep x = inorder_path(root->l);
  if (x.begin() != x.end())
    root->ra.insert(root->ra.end(), x.begin(), x.end());

  // cout << "(2) In: " << root->data << endl;
  root->ra.push_back(root);

  lnodep y = inorder_path(root->r);
  if (x.begin() != x.end())
    root->ra.insert(root->ra.end(), y.begin(), y.end());

  // cout << "(3) In: " << root->data << endl;
  // print_path(root);

  return root->ra;
}

int main() {

  node_t *a = get_node(100);
  a->l = get_node(50);
  a->r = get_node(150);
  a->l->l = get_node(20);
  a->l->r = get_node(55);
  node_t *root = a;

  inorder_path(root);

  print_path(root);
}
