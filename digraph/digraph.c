#include <stdio.h>
#include <stdlib.h>
#include "digraph.h"

Digraph *digraph_factory(int size)
{
	Digraph *g;
	LinkedList *list;
	if ((g = malloc(sizeof(*g))) == NULL)
		return NULL;
	g->V = size;
	g->E = 0;
	if ((g->vertices = calloc(size, sizeof(*g->vertices))) == NULL)
		return NULL;
	for (int i = 0; i < size; ++i) {
		list = malloc(sizeof(*list));
		init_list(list, sizeof(int));
		g->vertices[i] = list;
	}
	return g;
}

void free_digraph(Digraph *g)
{
	for (int i = 0; i < g->V; ++i)
		free_list(g->vertices[i], NULL);
	free(g->vertices);
	free(g);
}

int add_edge(Digraph *g, int src, int dest)
{
	LinkedList *list = g->vertices[src];
	ListNode *cur = list->head;
	while (cur != NULL) {
		if (*(int *)cur->item == dest)
			return LL_INSERT_DUPLICATE;
		cur = cur->next;
	}
	int list_err = 0;
	if ((list_err = ll_append(list, &dest)) != LL_OK)
		return list_err;
	g->E++;
	return 0;
}

Digraph *reverse(Digraph *g)
{
	Digraph *reversed = digraph_factory(g->V);
	ListNode *cur;
	for (int i = 0; i < g->V; ++i) {
		cur = g->vertices[i]->head;
		while (cur != NULL) {
			add_edge(reversed, *(int *)cur->item, i);
			cur = cur->next;
		}
	}
	return reversed;
}

void init_search_arrays(Digraph *g, int *visited, int *edge_to)
{
	for (int i = 0; i < g->V; ++i) {
		visited[i] = 0;
		edge_to[i] = -1;
	}
}

void dfs(Digraph *g, int src, int *visited, int *edge_to)
{
	ListNode *cur = g->vertices[src]->head;
	int val;
	visited[src] = 1;
	while (cur != NULL) {
		val = *(int *)cur->item;
		if (!visited[val]) {
			edge_to[val] = src;
			dfs(g, val, visited, edge_to);
		}
		cur = cur->next;
	}
}

void bfs(Digraph *g, int src, int *visited, int *edge_to)
{
	Queue *q = malloc(sizeof(*q));
	init_queue(q, sizeof(int));
	enqueue_int(q, src);
	ListNode *cur;
	int vertex, adj;
	visited[src] = 1;
	while (q->len > 0) {
		vertex = dequeue_int(q);
		cur = g->vertices[vertex]->head;
		while (cur != NULL) {
			adj = *(int *)cur->item;
			if (!visited[adj]) {
				visited[adj] = 1;
				edge_to[adj] = vertex;
				enqueue_int(q, adj);
			}
			cur = cur->next;
		}
	}
	free_queue(q, NULL);
}

int path_length(Digraph *g, int *edge_to, int src, int dest)
{
	int l = 1;
	int v = edge_to[dest];
	for (; v != src && v != -1; v = edge_to[v], ++l);
	return v == src ? l : -1;
}

int enqueue_int(Queue *queue, int val)
{
	return enqueue(queue, &val);
}

int dequeue_int(Queue *queue)
{
	int q_err = 0;
	int *val = dequeue(queue, &q_err);
	if (val == NULL) /* error handling */
		;
	int ret = *val;
	free(val);
	return ret;
}
