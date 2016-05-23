#include <stdio.h>
#include <stdlib.h>

#define TREE_SZ 10

typedef struct node_ {
    int val;
    struct node_ *l;
    struct node_ *r;

} node;

node *alloc_node(int val)
{
    node *n = (node *)malloc(sizeof(node));
    n->val = val;
    n->l = n->r = NULL;
    return n;
}

int head = 0, tail = -1;
node *nodeq[TREE_SZ + 100];

void print_nodes()
{
    for (int i = head; i <= tail; i++) {
        printf("(%d) %d \n", i, nodeq[i]->val);
    }
}

void print_tree(node *head)
{
    if (head == NULL) return;
    if (head->l) print_tree(head->l);
    if (head->r) print_tree(head->r);
    printf("%d \n", head->val);
}

void enq_node(node *n)
{
    if (tail >= (TREE_SZ + 100 -1)) {
        printf("q full, head %d tail %d \n", head, tail);
        return;
    }
    printf("enqueueing %d at %d [head: %d tail: %d] \n", 
        n->val, tail+1, head, tail+1);
    nodeq[++tail] = n; 
    return;
}

node *deq_node()
{
    if (head > tail) {
        printf("q empty\n");
        return NULL;
    }
    node *n = nodeq[head++];
    printf("dequeueing %d at %d [head: %d tail: %d] \n", 
        n->val, head-1, head-1, tail);
    return n;
}

unsigned int empty_q() {
    return (head > tail);
}

void build_tree(node **hd)
{
    static int val = 1, count = 0;
    node *t = *hd;
    
    if (*hd == NULL) {
        *hd = alloc_node(val++);
        enq_node(*hd);
        print_nodes();
    }
    while(!empty_q()) {
        node *n = deq_node();
        n->l = alloc_node(val++);
        n->r = alloc_node(val++);
        enq_node(n->l);
        count++;
        enq_node(n->r);
        count++;
        if (count >= TREE_SZ) {
            printf("All added, head %d tail %d\n", head, tail);
            break;
        }
    }
    print_tree(*hd);

}

void do_bfs(node *hd)
{
    head = 0;
    tail = -1;
    enq_node(hd);
    while(!empty_q()) {
        node *n = deq_node();
        if (n == NULL) {
            printf("Error !!. node is NULL \n");
            return;
        }
        printf("%d\n", n->val);
        if (n->l) enq_node(n->l);
        if (n->r) enq_node(n->r);
    }
}

int max(a, b)
{
    return (a > b) ? a : b;
}

int min(a, b)
{
    return (a < b) ? a : b;
}

int find_max_height(node *head)
{
    if(head == NULL) return 0;
    int h1 = find_max_height(head->l);
    int h2 = find_max_height(head->r);
    return (1 + max(h1, h2));
}

int find_min_height(node *head)
{
    if(head == NULL) return 0;
    int h1 = find_min_height(head->l);
    int h2 = find_min_height(head->r);
    return (1 + min(h1, h2));
}

void mirror_tree(node *hd)
{
    if (hd == NULL) return;

    mirror_tree(hd->l);
    mirror_tree(hd->r);

    // swap entires;
    node *temp = hd->l;
    hd->l = hd->r;
    hd->r = temp;
}

void print_all_paths(node *hd, int *arr, int len)
{
    static int num = 1;
    if (hd == NULL) return;
    arr[len++] = hd->val;
    if (!hd->l && !hd->r) {
        printf("path %d: \n", num++);
        for (int i = 0; i < len; i++) {
            printf("%d ", arr[i]);
        }
        printf("\n");
    }
    print_all_paths(hd->l, arr, len);
    print_all_paths(hd->r, arr, len);
}

void insert_node(node **root, int val)
{
    node *n = *root;

    if (*root == NULL) {
        *root = alloc_node(val);
        return;
    }
    if (val < n->val) {
        insert_node(&n->l, val);
    } else {
        insert_node(&n->r, val);
    }

}

int max_sum(node *root, int *my_max)
{
    if (root == NULL) return 0;

    int sum = max_sum(root->l, my_max) + max_sum(root->r, my_max) + root->val;
    
    printf("sum: %d max: %d\n", sum, *my_max);
    if (sum > *my_max) *my_max = sum;

    return sum;

}

int is_bst(node *root)
{
    int flag = 1;
    if (root->l) {
        flag = is_bst(root->l) && flag && (root->val >= root->l->val);
    }
    if (root->r) {
        flag = is_bst(root->r) && flag & (root->val <= root->r->val);
    }

    return flag;

}

// Do in-order traversal and remember previous node
int is_bst2(node *root)
{
    static node *prev = NULL;

    if (!root) return 1;

    if (is_bst2(root->l) == 0) return 0;

    if (prev && root->val < prev->val) return 0;
    // Remember root in prev
    prev = root;

    if (is_bst2(root->r) == 0) return 0;

    return 1;

}

int main()
{
    node *head = NULL;
    node *root = NULL;

    build_tree(&head);
    do_bfs(head);
    int max_ht = find_max_height(head);
    int min_ht = find_min_height(head);
    printf("max_ht = %d min_ht = %d \n", max_ht, min_ht);
    print_tree(head);
    printf("...\n");
    //mirror_tree(head);
    //print_tree(head);
    //printf("...\n");
    int arr[100];
    print_all_paths(head, arr, 0);

    // Build tree hd2 through user input
    insert_node(&root, -10);
    insert_node(&root, -20);
    insert_node(&root, 30);
    insert_node(&root, -5);
    insert_node(&root, 50);
    print_tree(root);
    int my_max = 0;
    max_sum(root, &my_max);
    printf("My max: %d \n", my_max);
    printf("is bst : %d \n", is_bst(root));
    printf("is bst : %d \n", is_bst2(root));
    
    return 0;
}
