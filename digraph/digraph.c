#include <stdlib.h>
#include "digraph.h"

Digraph *digraph_factory(int size)
{
	Digraph *g;
	if ((g = malloc(sizeof(Digraph))) == NULL)
		return NULL;
	g->V = size;
	g->E = 0;
	if ((g->vertices = calloc(size, sizeof(IntNode))) == NULL)
		return NULL;
	for (int i = 0; i < size; ++i)
		g->vertices[i] = NULL;
	return g;
}

IntQueue *int_queue_factory()
{
	IntQueue *q;
	if ((q = malloc(sizeof(IntQueue))) == NULL)
		return NULL;
	q->size = 0;
	q->status = INT_Q_OK;
	q->front = NULL;
	q->back = NULL;
	return q;
}

IntNode *int_node_factory(int val, IntNode *next)
{
	IntNode *n;
	if ((n = malloc(sizeof(IntNode))) == NULL)
		return NULL;
	n->val = val;
	n->next = next;
	return n;
}

int add_edge(Digraph *g, int from, int to)
{
	IntNode *cur = g->vertices[from];
	while (cur != NULL) {
		// Return -1 if it's a duplicate edge.
		if (cur->val == to) return -1;
		cur = cur->next;
	}
	IntNode *new_node = int_node_factory(to, g->vertices[from]);
	g->vertices[from] = new_node;
	g->E++;
	return 0;
}

Digraph *reverse(Digraph *g)
{
	Digraph *reversed = digraph_factory(g->V);
	IntNode *cur;
	for (int i = 0; i < g->V; ++i) {
		cur = g->vertices[i];
		while (cur != NULL) {
			add_edge(reversed, cur->val, i);
			cur = cur->next;
		}
	}
	return reversed;
}

void init_dfs_arrays(Digraph *g, int *visited, int *edge_to)
{
	for (int i = 0; i < g->V; ++i) {
		visited[i] = 0;
		edge_to[i] = -1;
	}
}

void dfs(Digraph *g, int from, int *visited, int *edge_to)
{
	visited[from] = 1;
	IntNode *cur = g->vertices[from];
	while (cur != NULL) {
		if (!visited[cur->val]) {
			edge_to[cur->val] = from;
			dfs(g, cur->val, visited, edge_to);
		}
		cur = cur->next;
	}
}

void bfs(Digraph *g, int from, int *visited, int *edge_to)
{
	IntQueue *queue = int_queue_factory(g->V);
	free_int_queue(queue);
}

void push_back(IntQueue *q, int val)
{
	IntNode *n = int_node_factory(val, NULL);
	if (q->back == NULL && q->front == NULL) {
		q->front = n;
		q->back = n;
	} else {
		q->back->next = n;
		q->back = n;
	}
	q->size++;
}

int pop_front(IntQueue *q)
{
	if (q->size == 0) {
		q->status = INT_Q_ERR_POPEMPTY;
		return -1;
	}
	int val = q->front->val;
	IntNode *tmp = q->front;
	if (q->front == q->back)
		q->back = NULL;
	q->front = q->front->next;
	q->size--;
	free(tmp);
	return val;
}

void free_nodes(IntNode *root)
{
	IntNode *current = root;
	IntNode *tmp;
	while (current != NULL) {
		tmp = current->next;
		free(current);
		current = tmp;
	}
}

void free_digraph(Digraph *g)
{
	for (int i = 0; i < g->V; ++i)
		free_nodes(g->vertices[i]);
	free(g);
}

void free_int_queue(IntQueue *q)
{
	free_nodes(q->front);
	free(q);
}
