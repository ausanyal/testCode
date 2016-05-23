//https://leetcode.com/problems/odd-even-linked-list/

#include <stdio.h>
#include <stdlib.h>

#define LIST_SIZE 21

struct ListNode {
    int val;
    struct ListNode *next;
};

typedef struct ListNode Node;
void build_list(Node **head);
void print_list(Node *head);

void alternate_list(struct ListNode **head) {
    Node *a, *b, *p = NULL, *t;
    
    if (!(*head) || !(*head)->next) {
        return;
    } 

    a = *head;
    b = (*head)->next;
    t = b->next;

    while(a && b) {
        b->next = a;
        a->next = t;
        if (a == *head) *head = b;
        printf("a and b swapped\n");
        print_list(*head);
        if (p) {
            p->next = b;
        }
        p = a;
        a = t;
        if (a) b = a->next;
        if (a && b) t = b->next;
    }

    return;
}

int main()
{
    Node *head, *n;
    build_list(&head); 
    alternate_list(&head);
    n = head;
    print_list(head);
    return 0;
}

void build_list(Node **head)
{

    Node *n, *prev;

    for (int i = 0; i < LIST_SIZE; i++) {
        n = (Node *)malloc(sizeof(Node));
        n->val = i+1;
        if (i == 0) *head = n;
        else prev->next = n;
        prev = n;
    }
    n->next = NULL;
    
    print_list(*head);
}

void print_list(Node *head)
{

    Node *n;
    n = head;
    while(n != NULL) {
        printf("%d ", n->val);
        n = n->next;
    }
    printf("\n");

}
