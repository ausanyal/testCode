#include <stdio.h>
#include <stdlib.h>

typedef struct node_ {
    int val;
    struct node_ *left;
    struct node_ *right;

} node_t;

#define MALLOC_NODE(n) (node_t *)malloc((n) * sizeof(node_t))

void 
insert_node(node_t **head, int val)
{
    node_t *node;

    if (*head == NULL) {
        printf("val = %d head == NULL \n", val);
        *head = MALLOC_NODE(1);
        (*head)->val = val;
        (*head)->left = NULL;
        (*head)->right = NULL;
    } else {
        if (val <= (*head)->val) {
            printf("val %d inserting left of current head %d\n", val, (*head)->val);
            insert_node(&((*head)->left), val);
        } else {
            printf("val %d inserting right of current head %d\n", val, (*head)->val);
            insert_node(&((*head)->right), val);
        }
    }
}

void print_dfs(node_t *node)
{
    if (node == NULL) {
        return;
    }

    if (node->left) {
        print_dfs(node->left);
    }
    if (node->right) {
        print_dfs(node->right);
    }
    printf("%d \n", node->val);
}

typedef struct node_stack_ {
    node_t *nodes[100];
    int current_top;
} node_stack_t;

node_stack_t node_stack = {0};
void 
stack_push(node_t *node)
{
    //printf("pushing %d currrent_top %d\n", node->val, node_stack.current_top);
    node_stack.nodes[node_stack.current_top++] = node;
}

node_t *
stack_pop()
{
    return (node_stack.nodes[--node_stack.current_top]);
}

void print_dfs_stack(node_t *node)
{
    int current_top = 0;

    if (node == NULL) {
        return;
    }
    
    stack_push(node);
    if(node->right) {
        print_dfs_stack(node->right);
    }
    if(node->left) {
        print_dfs_stack(node->left);
    }
    
    node_t *node_temp;
    node_temp = stack_pop();
    printf(".. %d \n", node_temp->val);

}

#define VAL(a) (a)->val

void
print_list(node_t *head);

void
print_list_rev(node_t *end);

void
convert_to_doubly_ll(node_t *head)
{
    node_t *a, *b, *c, *list_head, *list_end;
    static int done = 0;

    printf(" ************** Called with head %d *********** \n", VAL(head));
    if (!done) {
        done = 1;
        a = head;
        while(a->left) {
            a = a->left;
        }
        list_head = a;

        a = head;
        while(a->right) {
            a = a->right;
        }
        list_end = a;
        printf("list_head %d list_end %d\n", VAL(list_head), VAL(list_end));
    }

    a = head;

    if (a->left) {
        if (a->left->right) {
            b = a->left->right;
            while(b->right) {
                b = b->right; // rightmost node in a's tree
            }
            b->right = a;
            c = a->left;
            a->left = b;
            printf("linking %d->left = %d ; %d->right = %d \n", VAL(a), VAL(b), VAL(b), VAL(a));
            convert_to_doubly_ll(c);
        } else {
            printf("linking %d->right = %d \n", VAL(a->left), VAL(a));
            a->left->right = a;
        }
    }

    if (a->right) { 
        if (a->right->left) {
            b = a->right->left;
            while(b->left) {
                b = b->left; // leftmost node in a's tree
            }
            b->left = a;
            c = a->right;
            a->right = b;
            printf("linking %d->right = %d ; %d->left = %d \n", VAL(a), VAL(b), VAL(b), VAL(a));
            convert_to_doubly_ll(c);
        } else {
            printf("linking %d->left = %d \n", VAL(a->right), VAL(a));
            a->right->left = a;
        }
    }
    
    print_list(list_head);
    print_list_rev(list_end);
}

void
print_list(node_t *head)
{
    while (head) {
        printf("%d \n", head->val);
        head = head->right;
    }
}

void
print_list_rev(node_t *end)
{
    while (end) {
        printf("%d \n", end->val);
        end = end->left;
    }
}

int main()
{
    int num = 0;
    node_t *head = NULL;

    while(num != -1) {
        scanf("%d", &num);
        if (num == -1) {
            break;
        }
        insert_node(&head, num);
    }

    printf("********************\n");
    print_dfs(head);
    printf("********************\n");
    print_dfs_stack(head);

    printf("********************\n");
    convert_to_doubly_ll(head);

    return 0;
}
