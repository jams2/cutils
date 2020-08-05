#ifndef queue_h_
#define queue_h_

#ifndef linked_list_h_
#include "linked-list.h"
#endif

typedef LinkedList Queue;

void init_queue(Queue *queue, int item_size);
void free_queue(Queue *queue, void (*free_func)(void *));
int enqueue(Queue *queue, void *val);
void *dequeue(Queue *queue, int* q_err);

#endif
