#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct node_ {
    int l;
    char **s;
}node_t;

char *key_letters(char k)
{

    switch(k) {
        case '2':
            return "abc";
        case '3':
            return "def";
        case '4':
            return "ghi";
        default:
            return NULL;
    }
    return NULL;
}

#define ALLOC_NODE(node, nws, wl) {\
    printf("allocing %lu \n", nws);\
    node = (node_t *)calloc(1, sizeof(node_t));\
    node->s = (char **)calloc(1, nws * sizeof(char *));\
    node->l = nws;\
    for (int k = 0; k < nws; k++) {\
        node->s[k] = (char *)calloc(1, wl*sizeof(char));\
    }\
}

void print_sw(node_t *n)
{
    for (int i = 0; i < n->l ; i++) {
        printf("\t\t\t print_sw : %d: %s \n", i, n->s[i]);
    }
}

void append_str(char *dest, char *s2, char s1)
{
    dest[0] = s1;
    strcpy(dest+1, s2);
}

node_t *all_combs(char *s, int idx)
{
    printf("s: %s idx: %d \n", s, idx);
    node_t *n;
    if(idx >= strlen(s)) {
        ALLOC_NODE(n, (unsigned long)1,1);
        n->s[0] = "\0";
        n->l = 1;
        printf("base\n");
        return n;
    }
    
    node_t *n2;
    node_t *sub_words = all_combs(s, idx+1);
    print_sw(sub_words);
    char *kls = key_letters(s[idx]);
    ALLOC_NODE(n2, strlen(kls) * sub_words->l, 3);
    printf("...s: %s idx: %d kls: %s\n", s, idx, kls);

    int i2 = 0;
    for (int i = 0; i < sub_words->l; i++) {
        for (int j = 0; j < strlen(kls); j++) {
            append_str(n2->s[i2], sub_words->s[i], kls[j]);
            printf("n2->s[%d] = %s \n ", i2, n2->s[i2]);
            i2++;
        }
    }

    return n2;
}

int main(int agrc, char *argv[])
{
    all_combs(argv[1], 0);

    return 1;
}
