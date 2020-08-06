#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "hashtable.h"

#ifdef HT_DEBUG
int collisions = 0;
#endif

static int ht_init_seed();
static int ht_put_index(HashTable *ht, char *key);
static int ht_get_index(HashTable *ht, char *key);
static void ht_next_index(HashTable *ht, uint64 *i, uint64 *perturb);
static int seed_initialized;
static char hash_seed[SEED_SIZE];

void ht_init(HashTable *ht)
{
	if (!seed_initialized)
		ht_init_seed();
	ht->size = ht_initial_size;
	ht->sizemask = ht_initial_size - 1;
	ht->n_keys = 0;
	ht->keys = calloc(ht_initial_size, sizeof(*ht->keys));
	ht->vals = calloc(ht_initial_size, sizeof(*ht->vals));
	for (int i = 0; i < ht_initial_size; ++i) {
		ht->keys[i] = NULL;
		ht->vals[i] = NULL;
	}
}

static int ht_init_seed()
{
	FILE *fp;
	if ((fp = fopen("/dev/urandom", "r")) == NULL)
		return -1;
	fread(hash_seed, sizeof(char), SEED_SIZE, fp);
	seed_initialized = 1;
	return 0;
}

static int ht_put_index(HashTable *ht, char *key)
{
	uint64 i, perturb;
	i = perturb = ht_hash(key);
	ht_next_index(ht, &i, &perturb);
	while (ht->keys[i] != NULL) {
		if (strcmp(ht->keys[i], key) == 0)
			return i;
		#ifdef HT_DEBUG
		collisions++;
		#endif
		ht_next_index(ht, &i, &perturb);
	}
	return i;
}

static int ht_get_index(HashTable *ht, char *key)
{
	uint64 i, perturb;
	i = perturb = ht_hash(key);
	ht_next_index(ht, &i, &perturb);
	while (ht->keys[i] != NULL) {
		if (strcmp(ht->keys[i], key) == 0)
			return i;
		ht_next_index(ht, &i, &perturb);
	}
	return -1;
}

static void ht_next_index(HashTable *ht, uint64 *i, uint64 *perturb)
{
	*perturb >>= PERTURB_SHIFT;
	*i = ((*i * 5) + 1 + *perturb) & ht->sizemask;
}

uint64 ht_hash(char *key)
{
	return siphash((unsigned char *) key, strlen(key), (unsigned char *) hash_seed);
}

int ht_put(HashTable *ht, char *key, void *val)
{
	if (ht == NULL || key == NULL)
		return -1;

	if (ht->n_keys >= ht->size / 2)
		ht_resize(ht, ht->size * 2);

	int i = ht_put_index(ht, key);
	ht->keys[i] = key;
	ht->vals[i] = val;
	ht->n_keys++;
	return 0;
}

int ht_del(HashTable *ht, char *key)
{
	if (ht->n_keys < 1 || key == NULL)
		return -1;

	/* First get the index of the key. We need to hang on to perturb
	 * and i so we can rehash the rest of the cluster if necessary.
	 */
	uint64 i, perturb;
	i = perturb = ht_hash(key);
	ht_next_index(ht, &i, &perturb);
	while (ht->keys[i] != NULL && strcmp(ht->keys[i], key) != 0)
		ht_next_index(ht, &i, &perturb);
	if (ht->keys[i] == NULL)
		return -1;

	ht->keys[i] = NULL;
	if (ht->vals[i] != NULL) {
		free(ht->vals[i]);
		ht->vals[i] = NULL;
	}
	ht->n_keys -= 1;

	/* Rehash if there's a cluster. We need to follow the chain
	 * created by the insertion recurrence.
	 */
	char *k;
	void *v;
	ht_next_index(ht, &i, &perturb);
	while (ht->keys[i] != NULL) {
		k = ht->keys[i];
		v = (void *) ht->vals[i];
		ht->keys[i] = NULL;
		ht->vals[i] = NULL;
		ht_put(ht, k, v);
		ht_next_index(ht, &i, &perturb);
	}

	/* Half the size if it's 12.5% full or less (Sedgewick). */
	if (ht->n_keys > 0 && ht->n_keys <= ht->size / 8)
		ht_resize(ht, ht->size/2);

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
		j = ht_put_index(ht, oldkeys[i]);
		ht->keys[j] = oldkeys[i];
		ht->vals[j] = oldvals[i];
	}
	free(oldkeys);
	free(oldvals);
	return 0;
}

void *ht_get(HashTable *ht, char *key)
{
	int i;
	if ((i = ht_get_index(ht, key)) == -1)
		return NULL;
	return ht->vals[i];
}

#ifdef HT_DEBUG
int main() {
	char *keys[2048];
	char key[24];
	int i = 0;
	int j = i;
	char c;
	FILE *fp = fopen("sowpods.txt", "r");

	while ((c = getc(fp)) != EOF && i < 2048) {
		if ((c == ' ' || c == '\n' || c == '\t' || c == '\r') || j == 23) {
			if (j == 0) continue;
			key[j] = '\0';
			keys[i] = malloc(24 * sizeof(char));
			strcpy(keys[i++], key);
			j = 0;
		} else {
			key[j++] = c;
		}
	}
	fclose(fp);

	HashTable ht;
	ht_init(&ht);
	for (int i = 0; i < 2048; ++i) {
		ht_put(&ht, keys[i], NULL);
	}

	printf("%d collisions\n", collisions);
	free(ht.keys);
	free(ht.vals);
	for (i = 0; i < 2048; ++i)
		free(keys[i]);
}
#endif
