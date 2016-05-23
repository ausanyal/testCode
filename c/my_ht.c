#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>


typedef struct hashent_ {
    char *key;
    char *val;
    struct hashent_ *next;
} hashent_t;

typedef struct hasht_ {
    int size;
    hashent_t **table; 
}hasht_t;

hasht_t *
hasht_create(int size)
{
    if (size < 1) return NULL;
    hasht_t *ht;

    ht = (hasht_t *)malloc(sizeof(hasht_t));
    ht->size = size;

    ht->table = (hashent_t **)malloc(size * sizeof(hashent_t *));
    for (int i = 0; i < size; i++) {
        ht->table[i] = NULL;
    }

    return ht;
}

int
hasht_hash(hasht_t *ht, char *key)
{
    unsigned long hashval = 0;
    int i = 0;

    while((hashval < ULONG_MAX) && (i <= strlen(key))) {
        hashval = hashval << 8;
        hashval = hashval + key[i];
        ++i;
    }

    return (hashval % ht->size);
}


hashent_t *
ht_new_pair(char *key, char *val)
{
    hashent_t *ent;

    ent = (hashent_t *)malloc(sizeof(hashent_t));
    ent->key = strdup(key);
    ent->val = strdup(val);
    ent->next = NULL;

    return ent;
}

void
hasht_set(hasht_t *ht, char *key, char *val)
{
    int bin = hasht_hash(ht, key);
    hashent_t *new, *next = NULL, *last = NULL;

    next = ht->table[bin];

    // Search the key in ht
    while( (next != NULL) && (next->key != NULL) && (strcmp(key, next->key) > 0) ) {
        last = next;
        next = next->next;
    }

    // replace val on same key
    if ( (next != NULL) && (next->key != NULL) && (!strcmp(next->key, key)) ) {
        free(next->val);
        next->val = strdup(val);
    } else {
        // not found... create new
        new = ht_new_pair(key, val);

        if (next == ht->table[bin]) { // case 1: head of list
            new->next = next;
            ht->table[bin] = new;
        } else if (next == NULL) {    // case 2: end of list
            last->next = new;
        } else {                      // case 3: middle of list 
            new->next = next;
            last->next = new;
        }
    }
}

char *
hasht_get(hasht_t *ht, char *key)
{
    int bin = hasht_hash(ht, key);
    hashent_t *next;

    next = ht->table[bin];

    while((next != NULL) && (next->key != NULL) && (strcmp(key, next->key) > 0)) {
        next = next->next;
    }

    if ( (next == NULL) || (next->key == NULL) || strcmp(key, next->key)) {
        return NULL;
    } else {
        return next->val;
    }
}

int main()
{
    hasht_t *hashtable = hasht_create(65536);
    hasht_set( hashtable, "key1", "inky" );
    hasht_set( hashtable, "key2", "pinky" );
    hasht_set( hashtable, "key3", "blinky" );
    hasht_set( hashtable, "key4", "floyd" );
 
    printf( "%s\n", hasht_get( hashtable, "key1" ) );
    printf( "%s\n", hasht_get( hashtable, "key2" ) );
    printf( "%s\n", hasht_get( hashtable, "key3" ) );
    printf( "%s\n", hasht_get( hashtable, "key4" ) );
    printf( "%s\n", hasht_get( hashtable, "key5" ) );

    return 0;
}
