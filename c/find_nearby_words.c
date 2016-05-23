#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define NUM_NEARBY_CHARS 3

char *
nearby_chars(char c)
{
    switch(c) {
        case 'g':
            return "ghf";
        case 'i':
            return "iok";
        case 't':
            return "rty";
        default:
            return NULL;
    }

    return NULL;
}

char *list[] = {"git", "got", "hit", "hot", "fit"};

int 
is_word(char *str)
{
    return 1;
    for (int i = 0; i < sizeof(list)/sizeof(list[0]); i++) {
        if (!strcmp(str, list[i])) {
            return 1;
        }
    }

    return 0;
}

typedef struct node1_ {
    struct node1_ **child;
    char ch;
    char *str;
}node1_t;

int gcount = 0;
#define alloc_children(node, nearby_len, word_len) {\
    (node->child) = (node1_t **)calloc(1, nearby_len * sizeof(node1_t *)); \
    for (int node_cnt = 0; node_cnt < nearby_len; node_cnt++) { \
        node->child[node_cnt] = (node1_t *)calloc(1, sizeof(node1_t)); \
        node->str = (char *)calloc(1, word_len * sizeof(char));\
    }\
    gcount++;\
}

void assign_children(node1_t *node, char *nchars) {
    for (int i = 0; i < NUM_NEARBY_CHARS; i++) {
        node->child[i]->ch = nchars[i];
    }
}

void
print_tree(node1_t *pnode, node1_t *node, int len)
{
    if (node == NULL) {
        return;
    }

    char *str = (char *)malloc(len * sizeof(char));

    strcpy(str, pnode->str);
    node->str = strcat(str, &node->ch);
    if (node->child) {
        for (int i = 0; i < len; i++) {
            print_tree(node, node->child[i], len);
        }
    } else {
        if (is_word(node->str)) {
            printf("\t\t %s\n", node->str);

        }
    }
}

int
find_permut(char *str)
{
    node1_t *rnode = NULL, *node, **curr_nodes;
    int len = strlen(str); // depth of tree
    char ch, *nchars;

    rnode = (node1_t *)calloc(1, sizeof(node1_t));\
    alloc_children(rnode, NUM_NEARBY_CHARS, len);
    rnode->ch = '\0';
    node = rnode;

    for (int i = 0; i < len; i++) {
        nchars = nearby_chars(str[i]);
        if (node->ch == '\0') {
            assign_children(node, nchars);
        } else {
            for (int j = 0; j < NUM_NEARBY_CHARS; j++) { // each parent
                node = curr_nodes[j];
                if (j == 0) {
                    alloc_children(node, NUM_NEARBY_CHARS, len);
                    assign_children(node, nchars);
                } else {
                    node->child = (node1_t **)calloc(1, NUM_NEARBY_CHARS * sizeof(node1_t *)); 
                    for (int l = 0; l < NUM_NEARBY_CHARS; l++) {
                       node->child[l] = curr_nodes[0]->child[l];
                   }
                }

            }
        }
        curr_nodes = node->child; 
        node = curr_nodes[0];
    }

    print_tree(rnode, rnode, len);

    return 0;
}

typedef struct node_ {
    char **sub_words;
    int nw;
} node_t;

#define alloc_sw(node, nwords, wlen) {                                      \
    node = (node_t *)malloc(sizeof(node_t));                                \
    node->nw = nwords;                                                      \
    if (nwords) {                                                           \
        node->sub_words = (char **)malloc(nwords * sizeof(char *));         \
        for (int sw_cnt = 0; sw_cnt < nwords; sw_cnt++) {                   \
            node->sub_words[sw_cnt] = (char *)malloc(wlen * sizeof(char));  \
        }                                                                   \
    }                                                                       \
}

void
append_word(char *dest, char ch, char *str)
{
   dest[0] = ch;
   strcpy(dest+1, str);
}

node_t *
num_permuts(char *str, int index)
{
    node_t *n1, *n2;

    i (index >= strlen(str)) {
        alloc_sw(n1, (unsigned long)1, (unsigned long)1);
        n1->sub_words[0] = "";

        return n1;
    }

    char *nchars;
    n1 = num_permuts(str, index+1);
    nchars = nearby_chars(str[index]);

    int nsw = n1->nw;
    alloc_sw(n2, nsw*strlen(nchars), strlen(n1->sub_words[0])+2); 

    int idx = 0;
    for (int i = 0; i < nsw ; i++) {
        for (int j = 0; j < strlen(nchars); j++) {
            append_word(n2->sub_words[idx++], nchars[j], n1->sub_words[i]);
        }
    }

    for (int k = 0; k < idx; k++) {
        printf("== idx: %d nchars %s ; sub_words[%d] = %s \n", 
                idx, nchars, k, n2->sub_words[k]);
    }
    
    return n2;
}

int main(int argc, char *argv[])
{
    //find_permut(argv[1]);
    num_permuts(argv[1], 0);

    return 0;
}
