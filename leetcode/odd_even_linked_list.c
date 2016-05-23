//https://leetcode.com/problems/odd-even-linked-list/

#include <stdio.h>
#include <stdlib.h>

#define LIST_SIZE 55

struct ListNode {
    int val;
    struct ListNode *next;
};

typedef struct ListNode Node;
void build_list(Node **head);

struct ListNode* oddEvenList(struct ListNode* head) {
    Node *odd, *oi, *loi, *even, *ei, *lei, *temp;
    
    if (!head) {
        return NULL;
    } else if (head && !head->next) {
        return head;
    }

    odd = oi = head;
    even = ei = head->next;

    while(oi && ei) {
        if (oi->next) {
            temp = oi->next->next;
            oi->next = temp;
            loi = oi;
            oi = temp;
            if (oi) loi = oi;
        } 


        if (ei->next) {
            temp = ei->next->next;
            ei->next = temp;
            lei = ei;
            ei = temp;
            if (ei) lei = ei;
        } 

    }
    
    printf("loi : %d lei : %d \n", loi->val, lei->val);
    loi->next = even;
    return NULL;
}

int main()
{
    Node *head, *n;
    build_list(&head); 
    oddEvenList(head);
    n = head;
    for (int i = 0; i < LIST_SIZE; i++) {
        printf("%d ", n->val);
        n = n->next;
    }
    printf("\n");
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

    n = *head;
    while(n != NULL) {
        printf("%d ", n->val);
        n = n->next;
    }
    printf("\n");

}
