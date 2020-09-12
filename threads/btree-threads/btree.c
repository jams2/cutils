#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <errno.h>
#include <assert.h>
#include <sys/sysinfo.h>
#include "btree.h"

static volatile int TOTAL_WORDS = 0;
static volatile int files_read = 0;
static int thread_status[MAXTHREADS];
static pthread_mutex_t cond_lock = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond;
static struct btree *TREE;

struct btree *
btree_init()
{
  struct btree *tree;
  if ((tree = malloc(sizeof(*tree))) == NULL)
    return NULL;
  tree->root = NULL;
  tree->n = 0;
  pthread_mutex_init(&tree->lock, NULL);
  return tree;
}

void
btree_destroy(struct btree *tree)
{
  if (tree->root != NULL)
    btree_freewalk(tree->root);
  pthread_mutex_destroy(&tree->lock);
  free(tree);
}

void
btree_freewalk(struct node *k)
{
  if (k == NULL)
    return;
  free(k->key);
  pthread_mutex_destroy(&k->lock);
  btree_freewalk(k->l);
  btree_freewalk(k->r);
  free(k);
}

int
btree_contains(struct btree *tree, char *key)
{
  pthread_mutex_lock(&tree->lock);
  int has_key = btree_search(tree->root, key) != NULL;
  pthread_mutex_unlock(&tree->lock);
  return has_key;
}

struct node *
btree_search(struct node *k, char *key)
{
  if (k == NULL)
    return NULL;
  int cmp = strcasecmp(k->key, key);
  if (cmp == 0)
    return k;
  else if (cmp < 0)
    return btree_search(k->r, key);
  else
    return btree_search(k->l, key);
}

int
btree_insert(struct btree *tree, struct node *k)
{
  pthread_mutex_lock(&tree->lock);
  if (tree->root == NULL) {
    tree->root = k;
    tree->n += 1;
    pthread_mutex_unlock(&tree->lock);
    return 1;
  }
  pthread_mutex_unlock(&tree->lock);
  struct node *current = tree->root;
  int cmp;
  for (;;) {
    cmp = strcasecmp(current->key, k->key);
    if (cmp == 0) {
      node_destroy(k);
      return 0;
    } else if (cmp < 0) {
      pthread_mutex_lock(&current->lock);
      if (current->r == NULL) {
	current->r = k;
	pthread_mutex_unlock(&current->lock);
	break;
      } else {
	pthread_mutex_unlock(&current->lock);
	current = current->r;
      }
    } else {
      pthread_mutex_lock(&current->lock);
      if (current->l == NULL) {
	current->l = k;
	pthread_mutex_unlock(&current->lock);
	break;
      } else {
	pthread_mutex_unlock(&current->lock);
	current = current->l;
      }
    }
  }
  pthread_mutex_lock(&tree->lock);
  tree->n += 1;
  pthread_mutex_unlock(&tree->lock);
  return 1;
}

struct node *
btree_ins_walk(struct node *current, struct node *k)
{
  if (current == NULL) {
    k->n = 1;
    return k;
  }

  int cmp = strcasecmp(current->key, k->key);
  if (cmp < 0) {
    current->r = btree_ins_walk(current->r, k);
  } else if (cmp > 0) {
    current->l = btree_ins_walk(current->l, k);
  } else {
    node_destroy(k);
    return current;
  }

  if (current->l == NULL && current->r != NULL)
    current->n = 1 + current->r->n;
  else if (current->r == NULL && current->l != NULL)
    current->n = 1 + current->l->n;
  else if (current->r != NULL && current->l != NULL)
    current->n = 1 + current->l->n + current->r->n;
  return current;
}

struct node *
node_init(char *key)
{
  struct node *k;
  if ((k = malloc(sizeof(*k))) == NULL)
    return NULL;
  k->key = key;
  k->l = NULL;
  k->r = NULL;
  k->n = 1;
  pthread_mutex_init(&k->lock, NULL);
  return k;
}

void
node_destroy(struct node *k)
{
  pthread_mutex_destroy(&k->lock);
  free(k->key);
  free(k);
}

int
get_file_paths(char *basedir, char **paths, int *fcount)
{
  char *dot;
  DIR *dir;
  struct dirent *dir_ent;
  int dir_len = strlen(basedir);
  int f_len;

  if ((dir = opendir(basedir)) == NULL)
    return -1;

  while ((dir_ent = readdir(dir)) != NULL) {
    if (strcmp(dir_ent->d_name, ".") == 0 || strcmp(dir_ent->d_name, "..") == 0)
      continue;
    if ((dot = rindex(dir_ent->d_name, '.')) == NULL)
      continue;
    if (strcasecmp(dot, FILE_EXT) != 0)
      continue;
    f_len = strlen(dir_ent->d_name) + 1;
    if ((paths[*fcount] = calloc(dir_len + f_len, sizeof(**paths))) == NULL)
      return -2;
    sprintf(paths[*fcount], "%s", basedir);
    strcat(paths[(*fcount)++], dir_ent->d_name);
  }

  closedir(dir);
  return 0;
}

void *
read_words(void *arg)
{
  int thread_id = ((struct thread_arg *) arg)->thread_id;
  char *fname = ((struct thread_arg *) arg)->fname;
  FILE *fp;
  char *buf = calloc(MAXBUF, sizeof(*buf));
  char c, *key;
  int rc, i = 0;
  struct node *k;
  if ((fp = fopen(fname, "r")) == NULL) {
    free(buf);
    rc = -1;
    pthread_exit((void *) &rc);
  }
  while ((c = fgetc(fp)) != EOF) {
    if (!isalpha(c) || i == MAXBUF - 2) {
      if (i == 0)
	continue;
      else if (i == 1)
	continue;
      TOTAL_WORDS++;
      if ((key = calloc(i + 1, sizeof(*key))) == NULL) {
	rc = -2;
	fclose(fp);
	free(buf);
	pthread_exit((void *) &rc);
      }
      snprintf(key, i + 1, "%s", buf);
      k = node_init(key);
      btree_insert(TREE, k);
      i = 0;
    } else {
      buf[i++] = c;
    }
  }
  rc = 0;
  fclose(fp);
  free(buf);
  pthread_mutex_lock(&cond_lock);
  thread_status[thread_id] = T_READY;
  files_read++;
  pthread_cond_signal(&cond);
  pthread_mutex_unlock(&cond_lock);
  pthread_exit((void *) &rc);
}

char *
get_dir_name(char *src)
{
  int i;
  char *dirname;
  if (src[strlen(src) - 1] != '/') {
    dirname = calloc(strlen(src) + 2, sizeof(*dirname));
    for (i = 0; i < (int) strlen(src); ++i)
      dirname[i] = src[i];
    dirname[i++] = '/';
    dirname[i] = '\0';
  } else {
    dirname = calloc(strlen(src) + 1, sizeof(*dirname));
    for (i = 0; i < (int) strlen(src); ++i)
      dirname[i] = src[i];
    dirname[i] = '\0';
  }
  return dirname;
}

void
get_file_names(char *src, char *dirname, char **paths, int *file_count)
{
  switch (get_file_paths(dirname, paths, file_count)) {
  case 0: break;
  case -1:
    fprintf(stderr, "couldn't open %s\n", src);
    exit(1);
  case -2:
    fprintf(stderr, "out of memory\n");
    exit(1);
  }
}

int
main(int argc, char *argv[])
{
  char *paths[MAXFILES];
  int i, files_assigned, file_count = 0;
  struct timespec starttime;
  struct timespec endtime;
  pthread_t thread[MAXTHREADS];
  struct thread_arg thread_arg[MAXTHREADS];
  int thread_err;

  if (argc != 2) {
    fprintf(stderr, "usage: btree DIRECTORY\n");
    exit(1);
  }

  char *dirname = get_dir_name(argv[1]);

  get_file_names(argv[1], dirname, paths, &file_count);

  if (file_count == 0) {
    printf("no .%s files in %s\n", FILE_EXT, argv[1]);
    exit(0);
  }

  if ((TREE = btree_init()) == NULL) {
    fprintf(stderr, "btree_init returned NULL\n");
    exit(1);
  }

  for (i = 0; i < MAXTHREADS; ++i) {
    thread_status[i] = T_READY;
    thread_arg[i].thread_id = i;
  }

  if (clock_gettime(CLOCK_REALTIME, &starttime) != 0)
    exit(errno);

  for (files_assigned = 0, i = 0; i < MAXTHREADS && files_read < file_count; ++i) {
    thread_arg[i].fname = paths[files_assigned++];
    thread_status[i] = T_BUSY;
    if ((thread_err = pthread_create(&thread[i], NULL, read_words, &thread_arg[i])) != 0) {
      fprintf(stderr, "couldn't create thread\n");
      exit(thread_err);
    }
  }

  pthread_mutex_lock(&cond_lock);
  while (files_read < file_count) {
    // find a free thread
    for (i = 0; i < MAXTHREADS && files_read < file_count; ++i) {
      if (thread_status[i] == T_READY) {
	thread_arg[i].fname = paths[files_assigned++];
	thread_status[i] = T_BUSY;
	pthread_create(&thread[i], NULL, read_words, &thread_arg[i]);
      }
    }
    pthread_cond_wait(&cond, &cond_lock);
  }
  pthread_mutex_unlock(&cond_lock);

  if (clock_gettime(CLOCK_REALTIME, &endtime) != 0)
    exit(errno);

  long int elapsed = endtime.tv_nsec - starttime.tv_nsec;
  long double seconds = elapsed / 1e9;
  if (seconds > 0)
    printf("inserted %d words in %Lf seconds\n", TREE->n, seconds);

  for (i = 0; i < file_count; ++i)
    free(paths[i]);
  btree_destroy(TREE);
  return 0;
}
