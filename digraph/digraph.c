#include <stdlib.h>
#include "digraph.h"

void init_digraph(t_intnode *digraph[], int size)
{
    for (int i = 0; i < size; ++i) digraph[i] = NULL;
}

int add_edge(t_intnode *digraph[], int from, int to)
{
    t_intnode *current = digraph[from];
    while (current != NULL) {
	// Return -1 if it's a duplicate edge.
	if (current->val == to) return -1;
	current = current->next;
    }
    t_intnode *new = malloc(sizeof (t_intnode));
    new->val = to;
    new->next = digraph[from];
    digraph[from] = new;
    return 0;
}

int reverse(t_intnode *src[], t_intnode *dest[], int size)
{
    t_intnode *src_node;
    for (int i = 0; i < size; ++i) {
	src_node = src[i];
	while (src_node != NULL) {
	    add_edge(dest, src_node->val, i);
	    src_node = src_node->next;
	}
    }
    return 0;
}

void dfs(t_intnode *digraph[], int from, int *visited, int *edge_to)
{
    visited[from] = 1;
    t_intnode *cur = digraph[from];
    while (cur != NULL) {
	if (!visited[cur->val]) {
	    edge_to[cur->val] = from;
	    dfs(digraph, cur->val, visited, edge_to);
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

void free_digraph(t_intnode *digraph[], int size)
{
    for (int i = 0; i < size; ++i) free_nodes(digraph[i]);
}
