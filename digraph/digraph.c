#include <stdlib.h>
#include "digraph.h"

void init_digraph(struct intnode *digraph[], int size)
{
    for (int i = 0; i < size; ++i) digraph[i] = NULL;
}

int add_edge(struct intnode *digraph[], int from, int to)
{
    struct intnode *current = digraph[from];
    while (current != NULL) {
	// Return -1 if it's a duplicate edge.
	if (current->val == to) return -1;
	current = current->next;
    }
    struct intnode *new = malloc(sizeof (struct intnode));
    new->val = to;
    new->next = digraph[from];
    digraph[from] = new;
    return 0;
}

int reverse(struct intnode *src[], struct intnode *dest[], int size)
{
    struct intnode *src_node;
    for (int i = 0; i < size; ++i) {
	src_node = src[i];
	while (src_node != NULL) {
	    add_edge(dest, src_node->val, i);
	    src_node = src_node->next;
	}
    }
    return 0;
}

void free_nodes(struct intnode *root)
{
    struct intnode *current = root;
    struct intnode *tmp;
    while (current != NULL) {
	tmp = current->next;
	free(current);
	current = tmp;
    }
}

void free_digraph(struct intnode *digraph[], int size)
{
    for (int i = 0; i < size; ++i) free_nodes(digraph[i]);
}
