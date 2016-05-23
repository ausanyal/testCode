#include <stdio.h>
#include <stdlib.h>

typedef struct node_ {
    int val;
    struct node_ *next;
} node;

void print_list(node *h);

void alloc_node(node **n, int val)
{
    *n = (node *)calloc(1, sizeof(node));
    (*n)->val = val;
    (*n)->next = NULL;
}

void insert_list(node **head, int val)
{
       printf("*head = %p\n", *head);
   if (*head == NULL) {
       alloc_node(head, val);
       printf("allocated head\n");
       printf("*head = %p\n", *head);
       return;
   }
   node *t = *head;
   while(t->next != NULL) t = t->next;
   alloc_node(&t->next, val);

   return;
}

node *c1, *c2;
void merge_lists(node *p1, node *p2)
{
    if (!p1 || !p2) return;

    node *t;

    if (p1->val <= p2->val) {
        while (p1 && p2 && (p1->val <= p2->val)) {
            t = p1;
            p1 = p1->next;
        }
        t->next = p2;

        printf("case 1\n");

        if (t->val == 8) { 
            printf("t: %d %d\n", t->val, t->next->val);
            print_list(c1);
            print_list(p1);
            print_list(c2);
            print_list(p2);
        }
        merge_lists(p1, p2);
    } else {
        t = p2;
        p2 = p2->next;
        t->next = p1;

        printf("case 2\n");

        merge_lists(p1, p2);
    }
}

int main()
{
    node *p1 = NULL, *p2 = NULL;

    insert_list(&p1, 1);
    insert_list(&p1, 3);
    insert_list(&p1, 5);
    insert_list(&p1, 7);
    insert_list(&p1, 8);

    insert_list(&p2, 2);
    insert_list(&p2, 4);
    insert_list(&p2, 6);
    insert_list(&p2, 9);

    c1 = p1;
    c2 = p2;

    print_list(p1);
    print_list(p2);
    merge_lists(p1, p2);
    print_list(p1);
    print_list(p2);
    
    return 0;

}

void print_list(node *h)
{
    while(h) {
        printf("%d ", h->val);
        h = h->next;
    }
    printf("\n");
}
