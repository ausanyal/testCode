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

typedef struct node_ {
  int data;
  struct node_ *l;
  struct node_ *r;
  struct node_ *ra;
  node_(int x) { data = x; }
} node_t;

node_t *get_node(int data) {
  node_t *n = new node_t(data);
  n->l = n->r = n->ra = NULL;
  return n;
}

int main() { node_t *root = NULL; }
