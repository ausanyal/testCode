#include <stdio.h>
#include <stdlib.h>

typedef struct node_ {
    int val;
    struct node_ *next;
} node_t;

#define MALLOC_NODE(n) (node_t *)malloc((n) * sizeof(node_t))

void 
print_list(node_t *head);
void 
free_list(node_t *head);
    
void 
rev_list(node_t **head)
{
    node_t *a, *b, *c;

    a = *head;
    b = a->next;
    a->next = NULL;
    while(a && b) {
        c = b->next;
        b->next = a;
        a = b;
        b = c;
        printf("a %d b %d c %d\n", a?a->val:0, b?b->val:0, c?c->val:0);
    }

    *head = a;
}

void prepend_list(node_t **head, int val)
{
    node_t *node;

    printf("Adding %d \n", val);
    if (*head == NULL) {
        *head = (node_t *)malloc(sizeof(node_t));
        (*head)->val = val;
        (*head)->next = NULL;
    } else {
    
        node = MALLOC_NODE(1);
        node->val = val;
        node->next = *head;
        *head = node;
    }

}

int main()
{
    int num = 0;
    node_t *head = NULL;

    while( num != -1) {
        printf("Enter node val(-1 to end): ");
        scanf("%d", &num);
        if (num == -1) {
            break;
        }
        prepend_list(&head, num);
    }

    print_list(head);
    rev_list(&head);
    print_list(head);
    free_list(head);

    return 0;
}


void 
print_list(node_t *head)
{

    node_t *temp = head;

    while(temp != NULL) {
        printf("val = %d \n", temp->val);
        temp = temp->next;
    }
}

void 
free_list(node_t *head)
{

    node_t *temp = head, *free_node;

    while(temp != NULL) {
        free_node = temp;
        printf("val = %d \n", temp->val);
        temp = temp->next;
        free(free_node);
    }
}
