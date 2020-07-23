#include <stdlib.h>
#include "digraph.h"

t_digraph *digraph_factory(int size)
{
    t_digraph *g = malloc(sizeof (t_digraph));
    g->V = size;
    g->E = 0;
    g->vertices = calloc(size, sizeof (t_intnode));
    for (int i = 0; i < size; ++i)
	g->vertices[i] = NULL;
    return g;
}

int add_edge(t_digraph *g, int from, int to)
{
    t_intnode *cur = g->vertices[from];
    while (cur != NULL) {
	// Return -1 if it's a duplicate edge.
	if (cur->val == to) return -1;
	cur = cur->next;
    }
    t_intnode *new = malloc(sizeof (t_intnode));
    new->val = to;
    new->next = g->vertices[from];
    g->vertices[from] = new;
    return 0;
}

t_digraph *reverse(t_digraph *g)
{
    t_digraph *reversed = digraph_factory(g->V);
    t_intnode *cur;
    for (int i = 0; i < g->V; ++i) {
	cur = g->vertices[i];
	while (cur != NULL) {
	    add_edge(reversed, cur->val, i);
	    cur = cur->next;
	}
    }
    return reversed;
}

void dfs(t_digraph *g, int from, int *visited, int *edge_to)
{
    visited[from] = 1;
    t_intnode *cur = g->vertices[from];
    while (cur != NULL) {
	if (!visited[cur->val]) {
	    edge_to[cur->val] = from;
	    dfs(g, cur->val, visited, edge_to);
	}
	cur = cur->next;
    }
}

void free_nodes(t_intnode *root)
{
    t_intnode *current = root;
    t_intnode *tmp;
    while (current != NULL) {
	tmp = current->next;
	free(current);
	current = tmp;
    }
}

void free_digraph(t_digraph *g)
{
    for (int i = 0; i < g->V; ++i) free_nodes(g->vertices[i]);
    free(g);
}
