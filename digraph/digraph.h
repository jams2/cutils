#ifndef DIGRAPH_H__
#define DIGRAPH_H__

typedef struct intnode {
    int val;
    struct intnode *next;
} t_intnode;

typedef struct digraph {
    int V;  // num vertices
    int E;  // num edges
    t_intnode **vertices;
} t_digraph;

t_digraph *digraph_factory(int size);

void free_digraph(t_digraph *g);

void free_nodes(t_intnode *root);

int add_edge(t_digraph *g, int from, int to);

t_digraph *reverse(t_digraph *g);

void dfs(t_digraph *g, int from, int *visited, int *edge_to);

#endif
