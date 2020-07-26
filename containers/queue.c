#include <stdlib.h>
#include "containers.h"

void init_queue(Queue *queue, int item_size)
{
	init_list(queue, item_size);
}

void free_queue(Queue *queue, void (*free_func)(void *))
{
	free_list(queue, free_func);
}

int enqueue(Queue *queue, void *val)
{
	return ll_append(queue, val);
}

void *dequeue(Queue *queue, int* q_err)
{
	return remove_head(queue, q_err);
}
