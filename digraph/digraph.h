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
	int status;
	IntNode *front;
	IntNode *back;
} IntQueue;

enum int_q_status { INT_Q_OK, INT_Q_ERR_POPEMPTY };

Digraph *digraph_factory(int size);

IntQueue *int_queue_factory();

IntNode *int_node_factory(int val, IntNode *next);

void free_digraph(Digraph *g);

void free_nodes(IntNode *root);

void free_int_queue(IntQueue *q);

int add_edge(Digraph *g, int from, int to);

Digraph *reverse(Digraph *g);

void init_dfs_arrays(Digraph *g, int *visited, int *edge_to);

void dfs(Digraph *g, int from, int *visited, int *edge_to);

void bfs(Digraph *g, int from, int *visited, int *edge_to);

void push_back(IntQueue *q, int val);

int pop_front(IntQueue *q);

#endif
