#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <pthread.h>
#include <assert.h>
#include "fswalker.h"

/* Queue of directory path names for consumption by threads. */
struct queue dir_q;

/* Count of files and directories. */
struct fscount counts = { 0, 0 };

/* Must hold when updating counts. */
pthread_mutex_t count_lock = PTHREAD_MUTEX_INITIALIZER;

/* Must hold when signalling or waiting on THREAD_DONE. */
pthread_mutex_t thread_status_lock = PTHREAD_MUTEX_INITIALIZER;

/* A thread will signal THREAD_DONE when it reads a directory that
   contains no sub directories, and set thread_done[thread_id] to 1.
   This will wake the parent, whom will check if all threads are done
   before quitting. */
pthread_cond_t THREAD_DONE;

/* Must hold when signalling or waiting on QUEUE_READY */
pthread_mutex_t ready_lock = PTHREAD_MUTEX_INITIALIZER;

/* A thread will signal QUEUE_READY after it adds a new directory
   to the queue, to wake any waiting threads. */
pthread_cond_t QUEUE_READY;

int thread_done[MAX_THREADS];

void
thread_error_exit(int thread_id, char *msg, int err, int line)
{
  char buf[256];
  if (strlen(msg) > 240)
    sprintf(buf, "(thread %d)", thread_id);
  else
    sprintf(buf, "(thread %d) %s", thread_id, msg);
  fserror(buf, err, line, 0);
  pthread_mutex_lock(&thread_status_lock);
  thread_done[thread_id] = 1;
  pthread_cond_signal(&THREAD_DONE);
  pthread_mutex_unlock(&thread_status_lock);
  pthread_exit(NULL);
}

void
thread_exit(int thread_id, void *retval)
{
  pthread_mutex_lock(&thread_status_lock);
  thread_done[thread_id] = 1;
  pthread_cond_signal(&THREAD_DONE);
  pthread_mutex_unlock(&thread_status_lock);
  pthread_exit(retval);
}

void
fserror(char *msg, int err, int line, int do_exit)
{
  switch (err) {
  case EACCES:
    print_err(msg, line,  "permission denied");
    break;
  case EBADF:
    print_err(msg, line,  "bad file descriptor");
    break;
  case EFAULT:
    print_err(msg, line, "bad address");
    break;
  case ELOOP:
    print_err(msg, line, "followed too many links");
    break;
  case ENAMETOOLONG:
    print_err(msg, line, "pathname too long");
    break;
  case EMFILE:
    print_err(msg, line,  "reached process fd limit");
    break;
  case EOVERFLOW:
    print_err(msg, line, "file too large");
  case ENFILE:
    print_err(msg, line,  "reached system fd limit");
    break;
  case ENOENT:
    print_err(msg, line,  "doesn't exist");
    break;
  case ENOMEM:
    print_err(msg, line,  "out of memory");
    break;
  case ENOTDIR:
    print_err(msg, line,  "not a directory");
    break;
  case EAGAIN:
    print_err(msg, line, "sys mutex resources exhausted");
    break;
  case EPERM:
    print_err(msg, line, "insufficient privileges");
    break;
  default:
    print_err(msg, line, "unexpected exception");
  }
  if (do_exit)
    exit(1);
}

int
queue_init(struct queue *q)
{
  if ((q->head = malloc(sizeof(*q->head))) == NULL)
    return -1;
  q->tail = q->head;
  if (pthread_mutex_init(&q->head_lock, NULL) != 0
      || pthread_mutex_init(&q->tail_lock, NULL) != 0) {
    free(q->head);
    fserror("queue_init", errno, __LINE__, 1);
  }
  q->n = 0;
  return 0;
}

void
queue_destroy(struct queue *q)
{
  struct node *k = q->head;
  while (k != q->tail) {
    k = node_destroy(k);
  }
  node_destroy(k);
  pthread_mutex_destroy(&q->head_lock);
  pthread_mutex_destroy(&q->tail_lock);
}

struct node *
node_destroy(struct node *k)
{
  struct node *next = k->next;
  free(k->key);
  free(k);
  return next;
}

struct node *
node_init(char *key, struct node *next)
{
  struct node *k;
  if ((k = malloc(sizeof(*k))) == NULL)
    return NULL;
  k->key = key;
  k->next = next;
  return k;
}

int
enqueue(struct queue *q, char *key)
{
  struct node *k;
  if ((k = node_init(key, NULL)) == NULL)
    return -1;
  pthread_mutex_lock(&q->tail_lock);
  q->tail->next = k;
  q->tail = k;
  q->n++;
  pthread_mutex_unlock(&q->tail_lock);
  return 0;
}

/* head->next becomes the new "dummy" head,
   so return head->next->key */
char *
dequeue(struct queue *q)
{
  char *key;
  struct node *tmp;
  pthread_mutex_lock(&q->head_lock);
  tmp = q->head;
  if (tmp->next == NULL) {
    pthread_mutex_unlock(&q->head_lock);
    return NULL;
  }
  key = tmp->next->key;
  q->head = tmp->next;
  free(tmp);
  q->n--;
  pthread_mutex_unlock(&q->head_lock);
  return key;
}

void *
count_files(void *thread_id)
{
  // mark the thread as busy
  int id = *(int *)thread_id;
  pthread_mutex_lock(&thread_status_lock);
  thread_done[id] = 0;
  pthread_mutex_unlock(&thread_status_lock);

  char *dir_name, *path;
  struct dirent *ent;
  DIR *dir = NULL;
  int found_subdir;
  // grab a directory name off the queue
  while ((dir_name = dequeue(&dir_q)) != NULL) {
    if ((dir = opendir(dir_name)) == NULL)
	thread_error_exit(id, "opendir", errno, __LINE__);

    pthread_mutex_lock(&count_lock);
    counts.dirs++;
    pthread_mutex_unlock(&count_lock);

    errno = found_subdir = 0;
    while ((ent = readdir(dir)) != NULL) {
      if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0
	  || ent->d_name[0] == '.')
	continue;
      if (ent->d_type == DT_REG) {
	pthread_mutex_lock(&count_lock);
	counts.files++;
	pthread_mutex_unlock(&count_lock);
      } else if (ent->d_type == DT_DIR) {
	found_subdir = 1;
	path = calloc(strlen(dir_name) + strlen(ent->d_name) + 2,
		      sizeof(*path));
	if (path == NULL)
	  thread_error_exit(id, dir_name, errno, __LINE__);
	sprintf(path, "%s", dir_name);
	if (path[strlen(path) - 1] != '/')
	  strcat(path, "/");
	strcat(path, ent->d_name);
	enqueue(&dir_q, path);
	pthread_mutex_lock(&ready_lock);
	pthread_cond_signal(&QUEUE_READY);
	pthread_mutex_unlock(&ready_lock);
      }
    }

    if (errno != 0)
      thread_error_exit(id, "readdir", errno, __LINE__);
    else if (!found_subdir)
      thread_exit(id, NULL);
  }
  if (dir != NULL)
    closedir(dir);
  thread_exit(id, NULL);
  return NULL;
}

int
main(int argc, char *argv[])
{
  struct stat statbuf;
  pthread_t thread[MAX_THREADS];
  int thread_id[MAX_THREADS];
  char *start_dir = (argc == 2) ? argv[1] : "/";

  if (stat(start_dir, &statbuf) != 0)
    fserror("stat", errno, __LINE__, 1);
  else if ((statbuf.st_mode & S_IFDIR) == 0)
    fserror(start_dir, ENOTDIR, __LINE__, 1);

  if (queue_init(&dir_q) != 0)
    fserror("queue_init", errno, __LINE__, 1);

  enqueue(&dir_q, start_dir);

  for (int i = 0; i < MAX_THREADS; ++i) {
    thread_done[i] = 0;
    thread_id[i] = i;
    pthread_create(&thread[i], NULL, count_files, &thread_id[i]);
  }

  int done = 0;
  pthread_mutex_lock(&thread_status_lock);
  while (!done) {
    for (int i = 0; i < MAX_THREADS; ++i) {
      if (!thread_done[i]) {
	pthread_cond_wait(&THREAD_DONE, &thread_status_lock);
	break;
      }
    }
    done = 1;
  }
  pthread_mutex_unlock(&thread_status_lock);

  printf("counted %d directories and %d regular files\n", counts.dirs, counts.files);
  return 0;
}
