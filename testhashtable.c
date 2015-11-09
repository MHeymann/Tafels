#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hashtable.h"

/* type definitions and constants */
typedef struct name *name_ptr;
typedef struct name {
	char *id;
	int num;
} name_t;

#define BUFFER_SIZE 1024

/* function prototypes */
unsigned int hash(void *key, unsigned int size);
int scmp(void *v1, void *v2);
void val2str(void *key, void *value, char *buffer);

/* --- main routine --------------------------------------------------------- */

int main()
{
	char buffer[BUFFER_SIZE];
	int i = 1, ret;
	name_ptr np;
	hashtab_ptr ht;

	ht = ht_init(0.75f, hash, scmp);
	printf("Type \"search <Enter>\" to stop inserting and start searching.\n");
	printf(">> ");
	scanf("%s", buffer);
	while (strcmp(buffer, "search") != 0) {
		np = malloc(sizeof(name_t));
		np->id = strdup(buffer);
		np->num = i++;
		if ((ret = ht_insert(ht, np->id, np))) {
			printf("Not inserted...! (%i)\n", ret);
		 
			free(np->id);
			free(np);
		} else {
			printf("Insert %s with %d\n", np->id, np->num);
		}
		printf(">> ");
		scanf("%s", buffer);
	}

	ht_print(ht, val2str);

	printf("Type \"quit <Enter>\" to exit.\n");
	printf(">> ");
	scanf("%s", buffer);
	while (strcmp(buffer, "quit") != 0) {
		if (ht_search(ht, buffer, (void **) &np)) {
			printf("Found \"%s\" with %d.\n", np->id, np->num);
		} else {
			printf("Not found.\n");
		}
		printf(">> ");
		scanf("%s", buffer);
	}
	printf("\n");

	ht_free(ht, free, free);

	return EXIT_SUCCESS;
}

/* --- hash helper functions ----------------------------------------------- */

unsigned int hash(void *key, unsigned int size)
{
	char *keystr = (char *) key;
	unsigned int i, hash, length;

	hash = 0;
	length = strlen(keystr);
	for (i = 0; i < length; i++) {
		hash = (hash << 1) + keystr[i];
	}

	return (hash % size);
}

int scmp(void *v1, void *v2)
{
	return strcmp((char *) v1, (char *) v2);
}

void val2str(void *key, void *value, char *buffer)
{
	sprintf(buffer, "%s:[%d]", (char *) key, ((name_ptr) value)->num);
}

/* vim: textwidth=80 tabstop=4:
 */
