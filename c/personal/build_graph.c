#include <stdio.h>

typedef struct node_ {
    int num_links;
    struct node_ *links;
} node_t;

int node_id;

void build_node()
{
    int node_val, num_children;

    printf("Enter node val:");
    scanf("%d", &node_val);

    printf("Enter num children");
    scanf("%d", &num_children);

}

int main()
{
    char ch;
    printf("Enter new node(y/n):");
    if ((ch = getchar()) ==  'y') {
        printf("Entering new node. Id: %d \n", node_id);
    }

    build_node();

    return 0;
}
