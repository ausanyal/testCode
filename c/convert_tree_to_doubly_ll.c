#include <stdio.h>
#include <stdlib.h>

typedef struct node_ {
    int val;
    struct node_ *l;
    struct node_ *r;
}node_t;

typedef struct cont_ {
    node_t *val;
    struct cont_ *n;
} cont_t;

#define ALLOC_NODE(n, v) {\
    n = (node_t *)calloc(1, sizeof(node_t));\
    n->l = NULL;\
    n->r = NULL;\
    n->val = v;\
}

#define ALLOC_CONT(c, val) {\
    c = (cont_t *)calloc(1, sizeof(cont_t));\
    c->n = NULL;\
    c->val = val;\
}

void btree_insert(node_t **head, int val)
{
    node_t *t = *head;
    node_t *n;


    if (*head == NULL) {
        ALLOC_NODE(n, val);
        *head = n;
        printf("Head(now %p) %d = NULL\n", *head, (*head)->val);
        if (n->val == 2) {
            printf("n: %p l: %p r: %p \n", n, n->l, n->r);
        }
        return;
    }

    printf("head: %d (%p)...inserting %d\n", t->val, t, val);
    
    if(val < t->val) {
        printf("Going left\n");
        btree_insert(&t->l, val);
    } else {
        printf("Going right\n");
        btree_insert(&t->r, val);
        
    }

}

void dfs_print_tree(node_t *head) {

    if (!head) {
        return;
    }

    if (head->l) {
        dfs_print_tree(head->l);
    }

    if (head->r) {
        dfs_print_tree(head->r);
    }

    printf("%d \n", head->val);
}

void print_cont(cont_t *h, cont_t *t)
{
    while(h != NULL) {
        if (h == t) {
            printf("\t\ttail  ==> %s.%d: h: %d \n", __func__, __LINE__, h->val->val);
        } else {
            printf("\t\tnode  ==> %s.%d: h: %d \n", __func__, __LINE__, h->val->val);

        }
        h = h->n;
    }
}

void enqueue_node(cont_t **head, cont_t **tail, node_t *val)
{   
    cont_t *c;
    ALLOC_CONT(c, val);

    if (*head == NULL) {
        *head = *tail = c;
        return;
    }

    (*tail)->n = c;
    *tail = c;

    printf("\t\tenqueue_node  enqueue_node  enqueue_node \n");
    print_cont(*head, *tail);
}

node_t *dequeue_node(cont_t **head, cont_t **tail)
{

    printf("\t\tdequeue_node  dequeue_node dequeue_node \n");
    print_cont(*head, *tail);

    if (!(*head)) {
        return NULL;
    }

    if (*head == *tail) {
        node_t *n = (*head)->val;
        *head = NULL;
        *tail = NULL;
        free(*head);
    printf("\t\tafter dequeue_node  dequeue_node dequeue_node *head %p *tail %p\n", *head, *tail);
    print_cont(*head, *tail);
        return n;
    }

    cont_t *c = *head;
    node_t *n = c->val;
    *head = c->n;
    c->n = NULL;
    free(c);
    c = NULL;

    printf("\t\tafter dequeue_node  dequeue_node dequeue_node *head %p *tail %p\n", *head, *tail);
    print_cont(*head, *tail);

    return n;
}

void bfs_print_tree(cont_t **h, cont_t **t)
{

    int count = 0;
    while(1) {
        if (count == 5) return;
        count++;
        node_t *n = dequeue_node(h, t);
        printf("%s.%d: Dequeued val: %p (%d)\n", __func__, __LINE__, n, n->val);
            printf("%p %p %p\n", n, n->l, n->r);
        if (!n) {
            return;
        }

        if (n->l) {
            printf("%s.%d: Left \n", __func__, __LINE__);
            enqueue_node(h, t, n->l);
        }

        if (n->r) {
            printf("%s.%d: Right \n", __func__, __LINE__);
            enqueue_node(h, t, n->r);
        }

            printf("%p %p %p\n", n, n->l, n->r);
    }
}

int main()
{
    node_t *head = NULL;
    btree_insert(&head, 10);
    btree_insert(&head, 30);
    btree_insert(&head, 20);
    btree_insert(&head, 2);
    btree_insert(&head, 17);

    dfs_print_tree(head);

    cont_t *h = NULL, *t = NULL;
    enqueue_node(&h, &t, head);
    bfs_print_tree(&h, &t);

    return 0;
}
