#ifndef DIGRAPH_H__
#define DIGRAPH_H__
#include <containers.h>

typedef struct digraph {
	int V;  // num vertices
	int E;  // num edges
	LinkedList **vertices;
} Digraph;

Digraph *digraph_factory(int size);

void free_digraph(Digraph *g);

int add_edge(Digraph *g, int src, int dest);

Digraph *reverse(Digraph *g);

void init_search_arrays(Digraph *g, int *visited, int *edge_to);

void dfs(Digraph *g, int src, int *visited, int *edge_to);

void bfs(Digraph *g, int src, int *visited, int *edge_to);

int path_length(Digraph *g, int *edge_to, int src, int dest);

int enqueue_int(Queue *queue, int val);

int dequeue_int(Queue *queue);

#endif
