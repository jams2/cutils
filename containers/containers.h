#ifndef _CONTAINERS_H
#define _CONTAINERS_H
#include <limits.h>

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
	REALLOC_ERR,
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

typedef struct _gstack {
	int sp;
	int size;
	int item_size;
	void **stack;
} Stack;

void init_stack(Stack *s, int initial_size, int item_size);
int resize_stack(Stack *s, int size);
void free_stack(Stack *s);
void push_generic(Stack *s, void *val, int *err);
void pop_generic(Stack *s, int *err);


/* Red-Black Tree
 * From Algorithms 4th Ed., Sedgewick & Wayne.
 */

#define RB_SIZE(node) ((node) == NULL ? 0 : (node)->n)
#define IS_RED(node) ((node) == NULL ? 0 : (node)->colour == RED)

enum colour { RED = 1, BLACK = 0 };

typedef struct _rbnode {
	char *key;
	void *val;
	struct _rbnode *left, *right;
	int n;  // number of nodes in this subtree
	enum colour colour;
} RBNode;

typedef struct _rbtree {
	RBNode *root;
} RedBlackTree;

void init_rb_node(RBNode *node, char *key, void *val, enum colour colour);
void init_rb_tree(RedBlackTree *tree);
void free_rb_tree(RedBlackTree *tree);
void free_rb_nodes(RBNode *node);
void free_rb_node(RBNode *node);
RBNode *rb_rotate_l(RBNode *node);
RBNode *rb_rotate_r(RBNode *node);
void rb_put(RedBlackTree *tree, char *key, void *val);
void *rb_get(RedBlackTree *tree, char *key);
RBNode *rb_put_node(RBNode *node, char *key, void *val);
void rb_flip_colours(RBNode *node);
RBNode *rb_move_red_left(RBNode *node);
void rb_delete_min(RedBlackTree *tree);
RBNode *rb_del_min(RBNode *node);
RBNode *rb_balance(RBNode *h);


/* linear probing hashtable */

#define HT_INITIAL 8
#define POSITIVE(x) (x & (INT_MAX))

typedef struct _hashtable {
	int n_keys;
	int size;
	char **keys;
	void **vals;
} HashTable;

void ht_init(HashTable *ht);
int ht_hash(char *key);
int ht_insertion_index(HashTable *ht, char *key);
int ht_get(HashTable *ht, char *key);
int ht_put(HashTable *ht, char *key, void *val);
int ht_del(HashTable *ht, char *key);
int ht_resize(HashTable *ht, int n);

#endif //_CONTAINERS_H
