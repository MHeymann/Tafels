#include <stdio.h>
#include <stdlib.h>

#include "hashtable.h"

/*** structs and typedefs *************************************************/

typedef struct htentry *htentry_ptr;
struct htentry {
	void *key;
	void *val;
	htentry_ptr next_ptr;
} htentry_t;

struct hashtab {
	htentry_ptr *table;
	unsigned int size;
	unsigned int num_entries;
	float max_loadfactor;
	unsigned short idx; /* index into delta array */
	unsigned int (*hash)(void *key, unsigned int size);
	int (*cmp)(void *a, void *b);
} hashtab_t;

#define INITIAL_IDX 4
#define BUFFER_SIZE 1024

/**
 * this array is the difference between a power-of-two and the largest prime
 * less than that power-of-two 
 */
unsigned short delta[] = { 0, 0, 1, 1, 3, 1, 3, 1, 5, 3, 3, 9, 3, 1, 3, 19, 15,
	1, 5, 1, 3, 9, 3, 15, 3, 39, 5, 39, 57, 3, 35, 1 };

/*** function prototypes **************************************************/

static float get_lf(unsigned int num_entries, unsigned int size);
static int overfull(hashtab_ptr ht);
static int calc_htsize(int n);
static int power(int base, int exponent);
static htentry_ptr *talloc(int size);
static void rehash(hashtab_ptr ht);

/*** procedures and functions *********************************************/

hashtab_ptr ht_init(float loadfactor, unsigned int (*hash)(void *key, unsigned int size), 
		int (*cmp)(void *a, void *b))
{
	hashtab_ptr ht;
	
	ht = malloc(sizeof(hashtab_t));
	if (!ht) {
		printf("memory error\n");
		return NULL;
	}
	ht->idx = INITIAL_IDX;
	ht->size = calc_htsize((int)ht->idx);

	ht->table = talloc(ht->size);
	if (!ht->table) {
		printf("memory error from talloc in ht_init\n");
		free(ht);
		return NULL;
	}

	ht->num_entries = 0;
	ht->max_loadfactor = loadfactor;
	ht->hash = hash;
	ht->cmp = cmp;

	return ht;
}

int  ht_insert(hashtab_ptr ht, void *key, void *value)
{
	unsigned int hash = ht->hash(key, ht->size);
	htentry_ptr p;

	for (p = ht->table[hash]; p; p = p->next_ptr) {
		if (ht->cmp(p->key, key) == 0) {
			return HASH_TABLE_KEY_VALUE_PAIR_EXISTS;
		}
	}
	
	p = malloc(sizeof(htentry_t));
	if (!p) {
		printf("Memory error\n");
		return HASH_TABLE_NO_SPACE_FOR_NODE;
	}

	p->key = key;
	p->val = value;
	p->next_ptr = NULL;

	if (ht->table[hash]) {
		p->next_ptr = ht->table[hash];
		ht->table[hash] = p;
	} else {	
		ht->table[hash] = p;
	}

	ht->num_entries++;
	
	if (overfull(ht)) {
		rehash(ht);
	}

	return 0;
}

int  ht_insert_update(hashtab_ptr ht, void *key, void *value, void (*freeval)(void *))
{
	unsigned int hash = ht->hash(key, ht->size);
	void *v;
	htentry_ptr p;

	for (p = ht->table[hash]; p; p = p->next_ptr) {
		if (ht->cmp(p->key, key) == 0) {
			v = p->val;
			p->val = value;
			freeval(v);
			return HASH_TABLE_KEY_VALUE_PAIR_EXISTS;
		}
	}
	
	p = malloc(sizeof(htentry_t));
	if (!p) {
		printf("Memory error\n");
		return HASH_TABLE_NO_SPACE_FOR_NODE;
	}

	p->key = key;
	p->val = value;
	p->next_ptr = NULL;

	if (ht->table[hash]) {
		p->next_ptr = ht->table[hash];
		ht->table[hash] = p;
	} else {	
		ht->table[hash] = p;
	}

	ht->num_entries++;
	
	if (overfull(ht)) {
		rehash(ht);
	}

	return 0;
}


int  ht_search(hashtab_ptr ht, void *key, void **value)
{
	htentry_ptr p;
	int hash = ht->hash(key, ht->size);

	for (p = ht->table[hash]; p; p = p->next_ptr) {
		if (ht->cmp(p->key, key) == 0) {
			*value = p->val;
			break;
		}
	}

	return (p ? 1 : 0);
}

int ht_free(hashtab_ptr ht, void (*freekey)(void *k), void (*freeval)(void *v))
{
	unsigned int i;
	htentry_ptr p, q;

	for (i = 0; i < ht->size; i++) {
		for (p = ht->table[i]; p;) {
			freekey(p->key);
			freeval(p->val);
			q = p;
			p = p->next_ptr;
			free(q);
		}
	}

	free(ht->table);
	free(ht);

	return 0;
}

void ht_print(hashtab_ptr ht, void (*val2str)(void *k, void *v, char *b))
{
	unsigned int i;
	char buffer[BUFFER_SIZE];
	htentry_ptr p;
	
	for (i = 0; i < ht->size; i++) {
		printf("Bucket[%d]", i);
		for (p = ht->table[i]; p; p = p->next_ptr) {
			val2str(p->key, p->val, buffer);
			printf(" --> %s", buffer);

		}
		printf(" --> NULL\n");
	}
	return;
}

float get_lf(unsigned int num_entries, unsigned int size)
{
	return ((0.0f + num_entries) / size);
}

int overfull(hashtab_ptr ht)
{
	float lf;

	lf = get_lf(ht->num_entries, ht->size);

	return lf > ht->max_loadfactor;
}

int power(int base, int exponent)
{
	int i, answer = 1;

	for (i = 0; i < exponent; i++) {
		answer *= base;
	}
	
	return answer;
}

int calc_htsize(int n)
{
	int size;

	if (n > (int)(sizeof(delta) / sizeof(unsigned short) - 1)) {
		n = (int) (sizeof(delta) / sizeof(unsigned short) - 1);
	}
	size = power(2, n);

	return (size - delta[n]);
}

static htentry_ptr *talloc(int size)
{
	htentry_ptr *table;
	int i;
	table = malloc(size * sizeof(htentry_ptr));
	if (!table) {
		printf("memory error when allocating table\n");
		return NULL;
	}
	for (i = 0; i < size; i++) {
		table[i] = NULL;
	}
	return table;
}

static void rehash(hashtab_ptr ht)
{
	int old_size, i;
	htentry_ptr *oldtable; 
	htentry_ptr p, q;

	oldtable = ht->table;
	ht->idx++;

	old_size = ht->size;
	ht->size = calc_htsize(ht->idx);
	/*num_entries = ht->num_entries;*/
	ht->num_entries = 0;
	ht->table = talloc(ht->size);
	
	for(i = 0; i < old_size; i++) {
		for (p = oldtable[i]; p;) {
			ht_insert(ht, p->key, p->val);
			q = p;
			p = p->next_ptr;
			free(q);
		}
	}
	free(oldtable);

}
