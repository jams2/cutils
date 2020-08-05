#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "hashtable.h"

void ht_init(HashTable *ht)
{
	ht->size = HT_INITIAL;
	ht->sizemask = HT_INITIAL - 1;
	ht->n_keys = 0;
	ht->keys = calloc(HT_INITIAL, sizeof(*ht->keys));
	ht->vals = calloc(HT_INITIAL, sizeof(*ht->vals));
	for (int i = 0; i < HT_INITIAL; ++i) {
		ht->keys[i] = NULL;
		ht->vals[i] = NULL;
	}
}

int ht_insertion_index(HashTable *ht, char *key)
{
	int i = ht_hash(key) & ht->sizemask;
	while (ht->keys[i] != NULL)
		i = (i + 1) & ht->sizemask;
	return i;
}

uint64 ht_hash(char *key)
{
	unsigned char k[] = "TESTKEY";
	return siphash((unsigned char *) key, strlen(key), k);
}

int ht_put(HashTable *ht, char *key, void *val)
{
	if (ht == NULL || key == NULL)
		return -1;

	if (ht->n_keys >= ht->size / 2)
		ht_resize(ht, ht->size * 2);

	int i = ht_insertion_index(ht, key);
	ht->keys[i] = key;
	ht->vals[i] = val;
	ht->n_keys++;
	return 0;
}

int ht_resize(HashTable *ht, int n)
{
	char **oldkeys = ht->keys;
	void **oldvals = ht->vals;
	int oldsize = ht->size;
	if ((ht->keys = calloc(n, sizeof(*ht->keys))) == NULL
	    || (ht->vals = calloc(n, sizeof(*ht->vals))) == NULL)
		return -1;
	ht->size = n;
	ht->sizemask = n - 1;

	int i, j;
	for (i = 0; i < n; ++i)
		ht->keys[i] = ht->vals[i] = NULL;

	/* rehash all entries into new arrays */
	for (i = 0; i < oldsize; ++i) {
		if (oldkeys[i] == NULL)
			continue;
		j = ht_insertion_index(ht, oldkeys[i]);
		ht->keys[j] = oldkeys[i];
		ht->vals[j] = oldvals[i];
	}
	free(oldkeys);
	free(oldvals);
	return 0;
}
