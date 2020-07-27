#ifndef _CONTAINERS_H
#define _CONTAINERS_H

#define node_int_val(node_ptr) (*(int *)node_ptr->item)

/* linked list */
enum container_err {
	LL_OK,
	LL_TYPE_MISMATCH,
	LL_MALLOC_ERR,
	LL_REMOVE_EMPTY,
	LL_OUT_OF_BOUNDS,
	LL_INSERT_DUPLICATE,
	STACK_OVERFLOW,
	STACK_UNDERFLOW,
};

typedef struct _node {
	void *item;
	struct _node *next;
} ListNode;

typedef struct _linkedlist {
	ListNode *head;
	ListNode *tail;
	int len;
	int item_size;
} LinkedList;

void init_list(LinkedList *list, int item_size);
void free_list(LinkedList *list, void (*free_func)(void *));
ListNode *make_node(void *val, int item_size, ListNode *next, int *node_err);
void free_node(ListNode *node, void (*free_func)(void *));
int len(LinkedList *list);
int ll_append(LinkedList *list, void *val);
int ll_prepend(LinkedList *list, void *val);
void *remove_head(LinkedList *list, int *list_err);
void *remove_tail(LinkedList *list, int *list_err);
void *remove_node(LinkedList *list, int index, int *list_err);


/* queue */
typedef LinkedList Queue;

void init_queue(Queue *queue, int item_size);
void free_queue(Queue *queue, void (*free_func)(void *));
int enqueue(Queue *queue, void *val);
void *dequeue(Queue *queue, int* q_err);


/* stack */

typedef struct _intstack {
	int sp;
	int size;
	int *stack;
} IntStack;

void init_int_stack(IntStack *s, int size);
void free_int_stack(IntStack *s);
void push_int(IntStack *s, int val, int *err);
int pop_int(IntStack *s, int *err);

#endif
