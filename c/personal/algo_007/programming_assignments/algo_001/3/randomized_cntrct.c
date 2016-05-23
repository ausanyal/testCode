#include <stdio.h>
#include <strings.h>
#include <stdlib.h>
#include <glib.h>
#include <time.h>
#include <unistd.h>

// One glist per vertex
//#define NUM_VERTICES 200
#define NUM_VERTICES 4
GList       *gl[NUM_VERTICES];
GHashTable  *gh;
int         g_eid = 0;
int         idx = 1;

#define mprintf(arg, msg...) {\
    printf("%s.%d:"arg, __FUNCTION__, __LINE__, ##msg); \
}

typedef struct node_ {
    int sv;
    int dv;
    int eid;
    int list_freed;
} node_t;

#define NSV (node)->sv
#define NDV (node)->dv
#define NEID (node)->eid
#define GHF(node) g_hash_table_find(gh, gh_predicate_func, (node));

#define DBG() printf("%d \n", __LINE__);

void init_structs();
int get_rand_num();
void print_func (gpointer data, gpointer user_data);
void print_func2 (gpointer data, gpointer user_data);
void free_func (gpointer data);
gboolean gh_key_equal_func (gconstpointer a, gconstpointer b);
gboolean gh_predicate_func (gpointer key, gpointer value, gpointer user_data);
guint gh_hash_func(gconstpointer key);
void gh_print_func (gpointer key, gpointer value, gpointer user_data);

int return_rand_eid(int ecnt)
{
    int rand_eid = get_rand_num() % ecnt;

    return rand_eid;
}

gint 
gl_find_dv (gconstpointer a, gconstpointer b)
{
    node_t *na = (node_t *)a;
    node_t *nb = (node_t *)b;

    if (na->dv < nb->dv) {
        return -1;
    }

    if (na->dv > nb->dv) {
        return 1;
    }

    return 0;
}

void
merge_to_gld (gpointer data, gpointer user_data)
{
    node_t      *a = (node_t *)data;
    GList       *gld = (GList *)user_data, *srch_gl;

    //printf("rand: %d eid: %d sv: %d dv: %d\n", get_rand_num(), a->eid, a->sv, a->dv);
    //mprintf("node to merge >> eid: %d sv: %d dv: %d\n", a->eid, a->sv, a->dv);

    node_t  *add_node, *x;
    GList   *first;

    add_node = (node_t *)calloc(1, sizeof(node_t));
    memcpy(add_node, a, sizeof(node_t));

    // We need to make new node's sv = gld's sv
    first           = g_list_first(gld);
    x               = (node_t *)first->data;
    add_node->sv    = x->sv;

    // Search in gld before adding to its adj list
    srch_gl = g_list_find_custom(gld, a, gl_find_dv);
    if (srch_gl) {
        node_t *xx = (node_t *)srch_gl->data;
        mprintf("***************** Found node in gld: eid: %d sv: %d dv: %d \n", 
                 xx->eid, xx->sv, xx->dv);
    } else { // Could not find... append to gld
        //mprintf("Did not find node in gld ... appending to gld sv: %d dv: %d\n", x->sv, x->dv);
        gld = g_list_append(g_list_first(gld), add_node);
    }

    // Adding to the edge ht
    mprintf("Analysing add node: eid: %d sv: %d dv: %d \n", 
            add_node->eid, add_node->sv, add_node->dv);

    gpointer key = GHF(add_node);
    if (key) {
        node_t *kf;
        kf = (node_t *)key;
        mprintf("Key found eid: %d sv: %d dv: %d \n", kf->eid, kf->sv, kf->dv);
        if (kf->dv == add_node->dv) {
            // Delete old edge. We will add the new one below
            mprintf(" :::::::::::::::::: Deleting node from ht.");
            mprintf(" eid: %d sv: %d dv: %d \n", kf->eid, kf->sv, kf->dv);
            g_hash_table_remove(gh, key);

            g_hash_table_insert(gh, add_node, add_node);
            mprintf(" :::::::::::::::::: Adding node into ht.");
            mprintf(" eid: %d sv: %d dv: %d \n", add_node->eid, add_node->sv, add_node->dv);
        }
    } else {

        mprintf("Key not found \n");
    }

}

void
add_gls_to_gld(GList *gls, GList *gld)
{
    g_list_foreach(g_list_next(gls), merge_to_gld, gld);
}

int *eids;
int
eid_visited(int eid)
{
    int i;
    static int current_count = -1;

    if (current_count == -1) {
        eids = (int *)malloc(g_eid * sizeof(int));
        eids[++current_count] = eid;
        printf("eid alloced \n");
        return 0;
    }

    for( i = 0 ; i < current_count ; i++) {
        if (eids[i] == eid) {
            return 1;
        } 
    }
    
    eids[++current_count] = eid;
    printf("_____________________ eid %d not visted yet (current_count %d) _________________ \n", 
            eid, current_count);
    return 0;
}

void
run_rand_contraction()
{
    int         eid, ecnt = g_eid, vcnt = NUM_VERTICES;
    node_t      snode = {0}, *node;
    gpointer    key;
    GList       *gls;
    GList       *gld;

    while (vcnt > 2) { // Repeat until we have 2 vertices left

        do {
            eid = return_rand_eid(ecnt);
        } while(eid_visited(eid));

        //printf("Rand_eid: %d ... \n", eid);
        snode.eid = eid;
        key = g_hash_table_find(gh, gh_predicate_func, &snode);
        if (key) {
            node = (node_t *)key;
            mprintf("ht node after rand: eid: %d sv: %d dv: %d \n", node->eid, node->sv, node->dv);
            // now contract:
            // 1. search 2 glists indexed with node->sv and node->dv respectively 
            gls = g_list_first(gl[node->sv]);
            gld = g_list_first(gl[node->dv]);

            node_t *temp1 = (node_t *)gls->data;
            node_t *temp2 = (node_t *)gld->data;
            if (temp1->list_freed || temp2->list_freed) {
                printf("============== List SV(%d) or DV(%d) freed ========= \n",
                        temp1->list_freed, temp2->list_freed);
                continue;
            }

            // 2. add all the vertices(v) in node->sv's glist to node->dv's glist if not there already
            // 3. for each v above add an edge node->dv -> v
            add_gls_to_gld(gls, gld);

            printf("\n-------- gls ------- \n\n");
            g_list_foreach(gls, print_func2, NULL);
            printf("\n-------- gld ------- \n\n");
            g_list_foreach(gld, print_func2, NULL);
            printf("\n\n\n");

            // 4. Mark gls as list freed
            temp1 = (node_t *)((g_list_first(gls))->data);
            temp1->list_freed = 1;
            printf("__________________ marking list sv: %d dv: %d \n", temp1->sv, temp1->dv);

            // 5. delete node from ht (deleting the edge in Q) hence reducing ecnt by 1
        }
        vcnt--;
        ecnt--;
    }
}

int main()
{
    int i;

    init_structs();

    // Print 
    printf("*** Print *** (total edges: %d) \n", g_eid);
    g_list_foreach(g_list_first(gl[1]), print_func, NULL);

    // Call random contraction here
    run_rand_contraction();

    // Free
    printf("*** Free *** \n");
    for (i = 0; i < NUM_VERTICES; i++) {
        g_list_free_full(g_list_first(gl[i]), free_func);
    }
    g_hash_table_destroy(gh); 

    return 1;
}


void init_structs()
{
    int         i, read, val, sv;
    node_t      *node, *snode;
    gpointer    key;
    int         first_entry;
    size_t      len;
    char        *line = NULL, *pch;
    FILE        *fp = NULL;

    // Init all Glists
    for (i = 0; i < NUM_VERTICES; i++) {
        gl[i] = NULL;
    }

    gh = NULL;

    gh = g_hash_table_new (NULL, /*gh_hash_func*/
                           gh_key_equal_func);

    i = 1;
    do {
        if (fp == NULL) {
            fp = fopen("input.txt", "r");
        }

        if (line != NULL) {
            free(line);
        }

        line = NULL;
        len = 0;

        read = getline(&line, &len, fp); 
        if (read == -1) {
            //exit(0);
            break;
        }

        printf("Updating vertex %d \n", i);
        printf("%s \n", line);
        pch = strtok(line, " ");
        first_entry = 1;
        while(pch != NULL) {

            node        = (node_t *) calloc (1, sizeof(node_t));
            node->eid   = -1;

            val = atoi(pch);
            if (val == 0) {
                pch = strtok(NULL, " ");
                continue;
            }

            if (first_entry) {
                // Only source vertex. eid and dv = -1
                first_entry = 0;
                sv = val;
                node->sv = sv;
                node->dv = -1;

            } else {

                // All other vertices in adj list. We get edges here
                node->sv = sv;
                node->dv = val;

                // Check if this is a new edge and increment edge... else dont
                key = GHF(node);
                if (key == NULL) {

                    node->eid = ++g_eid;
                    g_hash_table_insert(gh, node, node);
                    printf(">>> sz: %d inserting edge: id: %d sv: %d dv: %d \n", 
                            g_hash_table_size(gh), NEID, NSV, NDV);

                } else {
                    snode = (node_t *)key;
                    printf("<<< Found edge: searching sv: %d dv: %d -> id: %d sv: %d dv: %d \n", 
                            NSV, NDV, snode->eid, snode->sv, snode->dv);
                }
            }

            gl[i] = g_list_append(g_list_first(gl[i]), node);
            pch = strtok(NULL, " ");
        }

        i++;

    } while(1);

    if (fp != NULL) {
        fclose(fp);
    }

    g_eid = g_eid - 1;
}

int
get_rand_num()
{
    static int inited = 0;
    int r;

    if (!inited) {
        srand(time(NULL));
        inited = 1;
    }
    r = rand();

    return (r);
}

void
print_func (gpointer data, gpointer user_data)
{
    node_t      *a = (node_t *)data;
    node_t      *b;
    gpointer    key;

    //printf("rand: %d eid: %d sv: %d dv: %d\n", get_rand_num(), a->eid, a->sv, a->dv);
    printf("eid: %d sv: %d dv: %d\n", a->eid, a->sv, a->dv);
    key = g_hash_table_lookup(gh, a);
    if (key != NULL) {
        b = (node_t *)key;
        //printf("******************* Found ht node: eid: %d sv: %d dv: %d\n", b->eid, b->sv, b->dv); 
    }
}

void
print_func2 (gpointer data, gpointer user_data)
{
    node_t      *a = (node_t *)data;
    node_t      *b;
    gpointer    key;

    //printf("rand: %d eid: %d sv: %d dv: %d\n", get_rand_num(), a->eid, a->sv, a->dv);
    printf("eid: %d sv: %d dv: %d / ", a->eid, a->sv, a->dv);
    key = g_hash_table_lookup(gh, a);
    if (key != NULL) {
        b = (node_t *)key;
        //printf("******************* Found ht node: eid: %d sv: %d dv: %d\n", b->eid, b->sv, b->dv); 
    }
}

void
free_func (gpointer data)
{
    // noting to free 
    node_t *a = (node_t *)data;
    printf("Freeing node: eid: %d sv: %d dv: %d ", a->eid, a->sv, a->dv);
    printf("... \n");
    free(a);
    return;
}

gboolean
gh_key_equal_func (gconstpointer a, gconstpointer b)
{
    node_t *na = (node_t *)a;
    node_t *nb = (node_t *)b;

    if ( ((na->sv == nb->sv) && (na->dv == nb->dv)) || 
         ((na->sv == nb->dv) && (na->dv == nb->sv)) ) {
        return TRUE;
    }

    return FALSE;
}

gboolean
gh_predicate_func (gpointer key, gpointer value, gpointer user_data)
{
    node_t *na = (node_t *)key;
    node_t *nb = (node_t *)user_data;

    if ( ((na->sv == nb->sv) && (na->dv == nb->dv)) || 
         ((na->sv == nb->dv) && (na->dv == nb->sv)) ||
         (na->eid == nb->eid)) {
        return TRUE;
    }


    return FALSE;
}

guint
gh_hash_func(gconstpointer key)
{
    node_t  *a      = (node_t *)key;
    guint   prime1  = 9967;
    guint   prime2  = 8923;
    guint   hv1, hv2;

    hv1 = prime1 * idx;
    // remove too many high bit zero.
    hv1 = (hv1 & 0xffff) | (hv1 << 16);
    hv1 = hv1 ^ a->sv;

    hv2 = prime2 * idx;
    hv2 = (hv2 & 0xffff) | (hv2 << 16);
    hv2 = hv2 ^ a->dv;

    idx++;

    return (hv1 + hv2);
}

void gh_print_func (gpointer key, gpointer value,
                    gpointer user_data)
{
    node_t *na = (node_t *)key;
    node_t *nb = (node_t *)user_data;

    if ( ((na->sv == nb->sv) && (na->dv == nb->dv)) || 
         ((na->sv == nb->dv) && (na->dv == nb->sv)) ) {
        printf("****** Found match ****** \n");
        printf("%s: user data sv: %d dv: %d eid: %d \n", __FUNCTION__, nb->sv, nb->dv, nb->eid);
        printf("%s: node data sv: %d dv: %d eid: %d \n", __FUNCTION__, na->sv, na->dv, na->eid);
    }
}

#if 0

gint
find_func(gconstpointer a, gconstpointer b)
{
    int *ta = (int *)a;
    int *tb = (int *)b;

    if (*ta == *tb) {
        return 0;
    }

    if (*ta < *tb) {
        return -1;
    }

    return 1;
}

int main()
{
    int     c, i, arr[10];
    char    *line = NULL;
    FILE    *fp = NULL;
    GList   *node;

    for (i = 0; i < 10 ; i++) {
        arr[i] = 10 - i;
        gl = g_list_append(gl, &arr[i]);
    }

    c = 2;
    node = g_list_find_custom(g_list_first(gl), &c, find_func);
    if (node != NULL) {
        printf("node found: %d \n", *(int *)node->data);
    } else {
        printf("node not found \n");
    }

    g_list_foreach(g_list_first(gl), print_func, NULL);
    g_list_free_full(g_list_first(gl), free_func);

    

    return 1;
}
#endif
