#ifndef fswalker_
#define fswalker_

#include <dirent.h>
#include <pthread.h>

#define print_err(msg, line, why)					\
  fprintf(stderr, "[%s:%d] \"%s\": %s\n", __FILE__, (line), (msg), (why))

#define MAX_THREADS 8

struct queue
{
  int n;
  struct node *head;
  struct node *tail;
  pthread_mutex_t head_lock;
  pthread_mutex_t tail_lock;
};

struct node
{
  char *key;
  struct node *next;
};

struct fscount
{
  int dirs;
  int files;
};

void fserror(char *msg, int err, int line, int do_exit);
void *count_files(void *thread_id);
int queue_init(struct queue *q);
void queue_destroy(struct queue *q);
struct node *node_init(char *key, struct node *next);
struct node *node_destroy(struct node *k);
int enqueue(struct queue *q, char *key);
char *dequeue(struct queue *q);
void thread_exit(int thread_id, void *retval);
void thread_error_exit(int thread_id, char *msg, int err, int line);

#endif
