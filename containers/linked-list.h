enum ll_status {
	LL_OK,
	LL_TYPE_MISMATCH,
	LL_MALLOC_ERR,
	LL_REMOVE_EMPTY,
	LL_OUT_OF_BOUNDS,
};

typedef struct _node {
	void *item;
	struct _node *next;
} ListNode;

typedef struct _linkedlist {
	ListNode *head;
	ListNode *tail;
	int len;
} LinkedList;

void init_list(LinkedList *list);

void free_list(LinkedList *list, void (*free_func)(void *));

ListNode *make_node(void *val, int *node_err);

int len(LinkedList *list);

int append(LinkedList *list, void *val);

int prepend(LinkedList *list, void *val);

void *remove_head(LinkedList *list, int *list_err);

void *remove_tail(LinkedList *list, int *list_err);

void *remove_node(LinkedList *list, int index, int *list_err);
