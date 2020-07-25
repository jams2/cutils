#ifndef DIGRAPH_H__
#define DIGRAPH_H__

typedef struct intnode {
	int val;
	struct intnode *next;
} IntNode;

typedef struct digraph {
	int V;  // num vertices
	int E;  // num edges
	IntNode **vertices;
} Digraph;

typedef struct int_queue {
	int size;
	IntNode *front;
	IntNode *back;
} IntQueue;

enum digraph_error_codes {
	OK,
	ERR_Q_POPEMPTY,
	ERR_MALLOC_INTNODE,
};

Digraph *digraph_factory(int size);

IntQueue *int_queue_factory();

IntNode *int_node_factory(int val, IntNode *next);

void free_digraph(Digraph *g);

void free_nodes(IntNode *root);

void free_int_queue(IntQueue *q);

int add_edge(Digraph *g, int src, int dest);

Digraph *reverse(Digraph *g);

void init_search_arrays(Digraph *g, int *visited, int *edge_to);

void dfs(Digraph *g, int src, int *visited, int *edge_to);

void bfs(Digraph *g, int src, int *visited, int *edge_to);

int path_length(Digraph *g, int *edge_to, int src, int dest);

int push_back(IntQueue *q, int val);

int pop_front(IntQueue *q, int *val);

#endif
