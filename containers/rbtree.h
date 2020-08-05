#ifndef rbtree_h_
#define rbtree_h_
/* Red-Black Tree
 * From Algorithms 4th Ed., Sedgewick & Wayne.
 */

#define RB_SIZE(node) ((node) == NULL ? 0 : (node)->n)
#define IS_RED(node) ((node) == NULL ? 0 : (node)->colour == RED)

enum colour { RED = 1, BLACK = 0 };

typedef struct _rbnode {
	char *key;
	void *val;
	struct _rbnode *left, *right;
	int n;  // number of nodes in this subtree
	enum colour colour;
} RBNode;

typedef struct _rbtree {
	RBNode *root;
} RedBlackTree;

void init_rb_node(RBNode *node, char *key, void *val, enum colour colour);
void init_rb_tree(RedBlackTree *tree);
void free_rb_tree(RedBlackTree *tree);
void free_rb_nodes(RBNode *node);
void free_rb_node(RBNode *node);
RBNode *rb_rotate_l(RBNode *node);
RBNode *rb_rotate_r(RBNode *node);
void rb_put(RedBlackTree *tree, char *key, void *val);
void *rb_get(RedBlackTree *tree, char *key);
RBNode *rb_put_node(RBNode *node, char *key, void *val);
void rb_flip_colours(RBNode *node);
RBNode *rb_move_red_left(RBNode *node);
void rb_delete_min(RedBlackTree *tree);
RBNode *rb_del_min(RBNode *node);
RBNode *rb_balance(RBNode *h);

#endif
