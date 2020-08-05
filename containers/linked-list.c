#include <stdlib.h>
#include <string.h>
#include "linked-list.h"
#include "containers-common.h"

void init_list(LinkedList *list, int item_size)
{
	list->len = 0;
	list->item_size = item_size;
	list->head = NULL;
	list->tail = NULL;
}

void free_list(LinkedList *list, void (*free_func)(void *))
{
	ListNode *tmp;
	while (list->head != NULL) {
		tmp = list->head;
		list->head = tmp->next;
		free_node(tmp, free_func);
	}
	free(list);
}

void free_node(ListNode *node, void (*free_func)(void *))
{
	if (free_func)
		(*free_func)(node->item);
	free(node->item);
	free(node);
}

ListNode *make_node(void *val, int item_size, ListNode *next, int *node_err)
{
	ListNode *n;
	if ((n = malloc(sizeof(*n))) == NULL ||
	    (n->item = malloc(item_size)) == NULL) {
		*node_err = LL_MALLOC_ERR;
		return n;
	}
	memcpy(n->item, val, item_size);
	n->next = next;
	return n;
}

int len(LinkedList *list)
{
	return list->len;
}

int ll_append(LinkedList *list, void *val)
{
	int node_err;
	ListNode *node;
	if ((node = make_node(val, list->item_size, NULL, &node_err)) == NULL)
		return node_err;
	if (list->tail == NULL) {
		list->head = list->tail = node;
	} else {
		list->tail->next = node;
		list->tail = node;
	}
	++list->len;
	return LL_OK;
}

int ll_prepend(LinkedList *list, void *val)
{
	int node_err;
	ListNode *node;
	if ((node = make_node(val, list->item_size, list->head, &node_err)) == NULL)
		return node_err;
	if (list->tail == NULL)
		list->head = list->tail = node;
	else
		list->head = node;
	++list->len;
	return LL_OK;
}

void *remove_head(LinkedList *list, int *list_err)
{
	if (list->len == 0) {
		*list_err = LL_REMOVE_EMPTY;
		return NULL;
	}
	if (list->tail == list->head)
		list->tail = NULL;
	void *item = list->head->item;
	ListNode *tmp = list->head;
	list->head = list->head->next;
	free(tmp);
	--list->len;
	return item;
}

void *remove_tail(LinkedList *list, int *list_err)
{
	return remove_node(list, list->len - 1, list_err);
}

void *remove_node(LinkedList *list, int index, int *list_err)
{
	if (list->len == 0) {
		*list_err = LL_REMOVE_EMPTY;
		return NULL;
	} else if (index > list->len || index < 0) {
		*list_err = LL_OUT_OF_BOUNDS;
		return NULL;
	} else if (index == 0) {
		return remove_head(list, list_err);
	}
	ListNode *tmp;
	ListNode *cur = list->head;
	for (int i = 1; i != index; ++i, cur = cur->next)
		;
	tmp = cur->next;
	cur->next = tmp->next;
	void *item = tmp->item;
	free(tmp);
	--list->len;
	return item;
}
