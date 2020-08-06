#ifndef hashtable_h_
#define hashtable_h_
#include <limits.h>
#include "siphash.h"

#define ht_initial_size 16

/* this idea from the python dict implementation:
 * https://github.com/python/cpython/blob/master/Objects/dictobject.c
 */
#define PERTURB_SHIFT 5
#define SEED_SIZE (64 / sizeof(char))


/* the implementation depends on the size being a power of 2 */
typedef struct _hashtable {
	int n_keys;
	int size;
	uint64 sizemask;
	char **keys;
	void **vals;
} HashTable;

void ht_init(HashTable *ht);
uint64 ht_hash(char *key);
void *ht_get(HashTable *ht, char *key);
int ht_put(HashTable *ht, char *key, void *val);
int ht_del(HashTable *ht, char *key);
int ht_resize(HashTable *ht, int n);

#endif
