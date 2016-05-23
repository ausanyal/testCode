#include <stdio.h>
#include <stdlib.h>

typedef struct node_ {
	int n;
	struct node_ *next;
} node_t;

void
print_list(node_t *l)
{
	while(l) {
		printf("\t%d", l->n);
		l = l->next;
	}

	printf("\n");
}

node_t *
alloc_node(int n) 
{
	node_t *p;
	
	p = (node_t *)malloc(sizeof(node_t));
	p->n = n;
	p->next = NULL;

	return p;
} 

void insert_to_list(node_t **h, int n)
{
	
	if (*h == NULL) {
		*h = alloc_node(n);
		return;
	}

	node_t *t = *h;
	while(t->next != NULL) {
		t = t->next;
	}

	t->next = alloc_node(n);

}

int main()
{
	int n1, n2, c = 0, num;
	node_t *l1 = NULL, *l2 = NULL;

	printf("list1 len: ");
	scanf("%d", &n1);
	printf("list2 len: ");
	scanf("%d", &n2);

	while (c < n1) {
		printf("c [%d]: ", c);
		scanf("%d", &num);
		insert_to_list(&l1, num);
		c++;
	}

	while (c < n2) {
		printf("c [%d]: ", c);
		scanf("%d", &num);
		insert_to_list(&l2, num);
		c++;
	}


	print_list(l1);
	print_list(l2);


	return 0;
}