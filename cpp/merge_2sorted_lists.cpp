/*
merge 2 sorted lists in place
*/

#include <iostream>

using namespace std;

typedef struct node_ {
  int data;
  struct node_ *next;
} node_t;

node_t *getNode(int data) {
  node_t *n = (node_t *)malloc(sizeof(node_t));
  n->data = data;
  n->next = NULL;

  return n;
}

void mergeLists(node_t *p1, node_t *p2, node_t **h) {

  node_t *t;

  if (!p1 || !p2)
    return;

  cout << "p1 " << p1->data << " p2 " << p2->data << endl;
  if (p1->data <= p2->data) {
    if (*h == NULL)
      *h = p1;
    t = p1;
    p1 = p1->next;
    t->next = p2;
    mergeLists(p1, p2, h);
  } else {
    if (*h == NULL)
      *h = p2;
    t = p2;
    p2 = p2->next;
    t->next = p1;
    mergeLists(p1, p2, h);
  }
}

int main() {
  node_t *head = NULL, *p1, *p2;

  p1 = getNode(2);
  p1->next = getNode(4);
  p1->next->next = getNode(6);
  p1->next->next->next = getNode(8);
  p1->next->next->next->next = getNode(10);

  p2 = getNode(1);
  p2->next = getNode(3);
  p2->next->next = getNode(5);

  mergeLists(p1, p2, &head);

  while (head != NULL) {
    cout << head->data << " ";
    head = head->next;
  }

  cout << endl;

  return 0;
}
