#ifndef hashtable_h_
#define hashtable_h_
#include <limits.h>
#include "siphash.h"
#define HT_INITIAL 16
#define POSITIVE(x) (x & (INT_MAX))

typedef struct _hashtable {
	int n_keys;
	int size;
	int sizemask;
	char **keys;
	void **vals;
} HashTable;

void ht_init(HashTable *ht);
uint64 ht_hash(char *key);
int ht_insertion_index(HashTable *ht, char *key);
int ht_get(HashTable *ht, char *key);
int ht_put(HashTable *ht, char *key, void *val);
int ht_del(HashTable *ht, char *key);
int ht_resize(HashTable *ht, int n);

#endif
