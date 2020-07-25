#include <stdlib.h>
#include "digraph.h"

Digraph *digraph_factory(int size)
{
	Digraph *g;
	if ((g = malloc(sizeof(*g))) == NULL)
		return NULL;
	g->V = size;
	g->E = 0;
	if ((g->vertices = calloc(size, sizeof(*g->vertices))) == NULL)
		return NULL;
	for (int i = 0; i < size; ++i)
		g->vertices[i] = NULL;
	return g;
}

IntQueue *int_queue_factory()
{
	IntQueue *q;
	if ((q = malloc(sizeof(*q))) == NULL)
		return NULL;
	q->size = 0;
	q->front = NULL;
	q->back = NULL;
	return q;
}

IntNode *int_node_factory(int val, IntNode *next)
{
	IntNode *n;
	if ((n = malloc(sizeof(*n))) == NULL)
		return NULL;
	n->val = val;
	n->next = next;
	return n;
}

int add_edge(Digraph *g, int src, int dest)
{
	IntNode *cur = g->vertices[src];
	while (cur != NULL) {
		// Return -1 if it's a duplicate edge.
		if (cur->val == dest) return -1;
		cur = cur->next;
	}
	IntNode *new_node = int_node_factory(dest, g->vertices[src]);
	g->vertices[src] = new_node;
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

void init_search_arrays(Digraph *g, int *visited, int *edge_to)
{
	for (int i = 0; i < g->V; ++i) {
		visited[i] = 0;
		edge_to[i] = -1;
	}
}

void dfs(Digraph *g, int src, int *visited, int *edge_to)
{
	visited[src] = 1;
	IntNode *cur = g->vertices[src];
	while (cur != NULL) {
		if (!visited[cur->val]) {
			edge_to[cur->val] = src;
			dfs(g, cur->val, visited, edge_to);
		}
		cur = cur->next;
	}
}

void bfs(Digraph *g, int src, int *visited, int *edge_to)
{
	IntQueue *q = int_queue_factory();
	push_back(q, src);
	IntNode *cur;
	int *v = malloc(sizeof(*v));
	visited[src] = 1;
	while (q->size > 0) {
		pop_front(q, v);
		cur = g->vertices[*v];
		while (cur != NULL) {
			if (!visited[cur->val]) {
				visited[cur->val] = 1;
				edge_to[cur->val] = *v;
				push_back(q, cur->val);
			}
			cur = cur->next;
		}
	}
	free(v);
	free_int_queue(q);
}

int path_length(Digraph *g, int *edge_to, int src, int dest)
{
	int l = 1;
	int v = edge_to[dest];
	for (; v != src && v != -1; v = edge_to[v], ++l);
	return v == src ? l : -1;
}

int push_back(IntQueue *q, int val)
{
	IntNode *n;
	if ((n = int_node_factory(val, NULL)) == NULL)
		return ERR_MALLOC_INTNODE;
	if (q->back == NULL && q->front == NULL) {
		q->front = n;
		q->back = n;
	} else {
		q->back->next = n;
		q->back = n;
	}
	q->size++;
	return 0;
}

int pop_front(IntQueue *q, int *val)
{
	if (q->size == 0)
		return ERR_Q_POPEMPTY;
	*val = q->front->val;
	IntNode *tmp = q->front;
	if (q->front == q->back)
		q->back = NULL;
	q->front = q->front->next;
	q->size--;
	free(tmp);
	return 0;
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
