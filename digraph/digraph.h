#ifndef DIGRAPH_H__
#define DIGRAPH_H__

struct intnode {
    int val;
    struct intnode *next;
};

extern void free_nodes(struct intnode *root);

extern void init_digraph(struct intnode *digraph[], int size);

extern void free_digraph(struct intnode *digraph[], int size);

extern int add_edge(struct intnode *digraph[], int from, int to);

int reverse(struct intnode *src[], struct intnode *dest[], int size);

#endif
