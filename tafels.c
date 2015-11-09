#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "hashtable.h"

/*--- global/static variables --------------------------------------*/

extern hashtab_ptr ht;
static int max_operand = 10;
static int num_qs = 10;

typedef struct key {
	int op1;
	int op2;
} keys_t;

/*--- macros and defs ----------------------------------------------*/

#define MAX_LF 0.5

/*--- function prototypes ------------------------------------------*/

static void set_max_operand(int max);
static int get_operand();
static int max(int a, int b);
static int min(int a, int b);
static int combs_avail();
static float expect_repeats();
unsigned int hash(void *key, unsigned int size);
int cmp(void *v1, void *v2);
void freekey(void *k);
void freeval(void *v);

/*--- main routine -------------------------------------------------*/

int main(int argc, char *argv[])
{
	int i, op1, op2, found, answer;
	int attempts;
	keys_t *key;
	int *val;
	float av_repeats;
	hashtab_ptr ht;

	ht = ht_init(MAX_LF, hash, cmp);
	srand(time(NULL));

	if (argc == 1) {
		goto run;
	}
	for (i = 1; i < argc; i++) {
		/* TODO: handle arguments and flags */	
		printf("%s", argv[i]);
		set_max_operand(12);
	}

run:

	for (i = 0; i < num_qs; i++) {
		av_repeats = (0.0f + i) / num_qs;
		av_repeats *= expect_repeats(); 
		key = malloc(sizeof(keys_t));
		if (!key) {
			printf("Memory error\n");
			return 1;
		}
		val = malloc(sizeof(int));
		if (!val) {
			printf("Memory error\n");
			return 2;
		}
get_ops:
		op1 = get_operand();
		op2 = get_operand();
		key->op1 = op1;
		key->op2 = op2;
		*val = 1;
		if ((found = ht_search(ht, (void *)key, (void **)(&val)))) {
			if ((float)*val > (av_repeats * 3)) {
				goto get_ops;
			}
		}
		ht_insert_update(ht, (void *)key, (void *)val, freeval);
		if (found) {
			free(key);
		}
		printf("%d * %d = ", op1, op2);
		attempts = 0;
scanner:
		attempts++;
		scanf("%d", &answer);
		if (answer == op1 * op2) {
			printf("Correct!! \n");
		} else if (attempts >= 3) {
			printf("The correct answer is %d", (op1 * op2));
		} else {
			printf("Sorry, try again!\n");
			goto scanner;
		}
	}

	ht_free(ht, freekey, freeval);
	free(val);

	return 0;
}

/*--- helper routines and functions --------------------------------*/


static void set_max_operand(int max)
{
	max_operand = max - 2;
}

static int get_operand()
{
	unsigned int i;
	i = rand();

	return (int)(i % max_operand + 2);
}

static int max(int a, int b)
{
	return (a > b ? a : b);
}

static int min(int a, int b)
{
	return (a < b ? a : b);
}

static int combs_avail()
{
	return max_operand * (max_operand - 1) / 2;
}

static float expect_repeats()
{
	return (0.0f + combs_avail()) / num_qs;
}

unsigned int hash(void *key, unsigned int size)
{
	keys_t *k;
	int imin, imax;
	unsigned int hash;

	k = (keys_t *)key;
	imin = min(k->op1, k->op2);
	imax = max(k->op1, k->op2);
	hash = (unsigned int)(imin * max_operand + imax);
	hash = hash << 5;

	return hash % size;
}

int cmp(void *v1, void *v2)
{
	keys_t *k1, *k2;

	k1 = (keys_t *)v1;
	k2 = (keys_t *)v2;

	if ((min(k1->op1, k1->op2) == min(k2->op1, k2->op2)) && (max(k1->op1, k1->op2) == max(k2->op1, k2->op2))) {
		return 0;
	}
	
	return ((k1->op1 * k1->op2) - (k2->op1 * k2->op2));
}

void freekey(void *k) 
{
	keys_t *key;
	key = (keys_t *)k;
	free(key);
}

void freeval(void *v)
{
	int *val = (int *)v;
	free(val);
}
