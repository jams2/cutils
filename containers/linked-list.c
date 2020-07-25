#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "linked-list.h"

void init_list(LinkedList *list)
{
	list->len = 0;
	list->head = NULL;
	list->tail = NULL;
}

void free_list(LinkedList *list, void (*free_func)(void *))
{
	ListNode *tmp;
	while (list->head != NULL) {
		tmp = list->head;
		list->head = tmp->next;
		if (free_func)
			(*free_func)(tmp->item);
		free(tmp->item);
		free(tmp);
	}
}

void free_node(ListNode *node, void (*free_func)(void *))
{
	if (free_func)
		(*free_func)(node->item);
	free(node->item);
	free(node);
}

ListNode *make_node(void *val, int *node_err)
{
	ListNode *n;
	if ((n = malloc(sizeof(*n))) == NULL ||
	    (n->item = malloc(sizeof(*val))) == NULL) {
		*node_err = LL_MALLOC_ERR;
		return n;
	}
	memcpy(n->item, val, sizeof(*val));
	return n;
}

int len(LinkedList *list)
{
	return list->len;
}

int append(LinkedList *list, void *val)
{
	int node_err;
	ListNode *node;
	if ((node = make_node(val, &node_err)) == NULL)
		return node_err;
	node->next = NULL;
	if (list->tail == NULL) {
		list->head = list->tail = node;
	} else {
		list->tail->next = node;
		list->tail = node;
	}
	++list->len;
	return LL_OK;
}

int prepend(LinkedList *list, void *val)
{
	int node_err;
	ListNode *node;
	if ((node = make_node(val, &node_err)) == NULL)
		return node_err;
	node->next = list->head;
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
	ListNode *node = list->head->next;
	ListNode *prev = list->head;
	for (int i = 1; i != index; ++i) {
		prev = node;
		node = node->next;
	}
	prev->next = node->next;
	void *item = node->item;
	free(node);
	--list->len;
	return item;
}

int main() { return 0; }
