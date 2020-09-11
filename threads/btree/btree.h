#ifndef btree_
#define btree_

#define MAXFILES 100
#define FILE_EXT ".txt"
#define MAXBUF 64

struct node {
	char *key;
	int n;
	struct node *l;
	struct node *r;
};

struct btree {
	struct node *root;
	int n;
};

int get_file_names(char *basedir, char **fnames, int *fcount);
struct btree *btree_init();
void btree_freewalk(struct node *k);
void btree_destroy(struct btree *tree);
struct node *node_init(char *key);
struct node *btree_ins_walk(struct node *current, struct node *k);
int read_words(char *fname, struct btree *tree);
void node_destroy(struct node *k);
int btree_contains(struct btree *tree, char *key);
struct node *btree_search(struct node *k, char *key);

#endif
