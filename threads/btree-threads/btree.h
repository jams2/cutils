#ifndef btree_
#define btree_
#include <pthread.h>

#define MAXFILES 100
#define FILE_EXT ".txt"
#define MAXBUF 64
#define MAXTHREADS 4

enum T_STATUS {
  T_READY,
  T_BUSY,
};

struct thread_arg {
  int thread_id;
  char *fname;
};

struct node {
  char *key;
  int n;
  struct node *l;
  struct node *r;
  pthread_mutex_t lock;
};

struct btree {
  struct node *root;
  int n;
  pthread_mutex_t lock;
};

int get_file_paths(char *basedir, char **fnames, int *fcount);
struct btree *btree_init();
void btree_freewalk(struct node *k);
void btree_destroy(struct btree *tree);
struct node *node_init(char *key);
struct node *btree_ins_walk(struct node *current, struct node *k);
void *read_words(void *arg);
void node_destroy(struct node *k);
int btree_contains(struct btree *tree, char *key);
struct node *btree_search(struct node *k, char *key);
char *get_dir_name(char *src);

#endif
