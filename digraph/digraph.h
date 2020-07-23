#ifndef DIGRAPH_H__
#define DIGRAPH_H__

typedef struct intnode {
    int val;
    struct intnode *next;
} t_intnode;

extern void free_nodes(t_intnode *root);

extern void init_digraph(t_intnode *digraph[], int size);

extern void free_digraph(t_intnode *digraph[], int size);

extern int add_edge(t_intnode *digraph[], int from, int to);

int reverse(t_intnode *src[], t_intnode *dest[], int size);

void dfs(t_intnode *digraph[], int from, int *visited, int *edge_to);

#endif
