#ifndef HASHTABLE_H
#define HASHTABLE_H


#define HASH_TABLE_KEY_VALUE_PAIR_EXISTS -1
#define HASH_TABLE_NO_SPACE_FOR_NODE     -2

typedef struct hashtab *hashtab_ptr;

hashtab_ptr ht_init(float loadfactor, unsigned int (*hash)(void *key, unsigned int size), 
		int (*cmp)(void *a, void *b));
int  ht_insert(hashtab_ptr ht, void *key, void *value);
int  ht_search(hashtab_ptr ht, void *key, void **value);
int ht_free(hashtab_ptr ht, void (*freekey)(void *k), void (*freeval)(void *v));
void ht_print(hashtab_ptr ht, void (*val2str)(void *k, void *v, char *b));

#endif
