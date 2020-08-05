#ifndef linked_list_h_
#define linked_list_h_

#define node_int_val(node_ptr) (*(int *)node_ptr->item)

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

#endif
