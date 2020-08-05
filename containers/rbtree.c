#include <stdlib.h>
#include <string.h>
#include "rbtree.h"


void init_rb_node(RBNode *h, char *key, void *val, enum colour colour)
{
	h->key = key;
	h->val = val;
	h->left = h->right = NULL;
	h->n = 1;
	h->colour = colour;
}

void init_rb_tree(RedBlackTree *tree)
{
	tree->root = NULL;
}

void free_rb_tree(RedBlackTree *tree)
{
	free_rb_nodes(tree->root);
	free(tree);
}

void free_rb_nodes(RBNode *h)
{
	if (h == NULL)
		return;
	free_rb_nodes(h->right);
	free_rb_nodes(h->left);
	free(h);
}

void free_rb_node(RBNode *h)
{
	free(h);
}

RBNode *rb_rotate_l(RBNode *h)
{
	RBNode *tmp = h->right;
	h->right = tmp->left;
	tmp->left = h;
	tmp->colour = h->colour;
	h->colour = RED;
	tmp->n = h->n;
	h->n = 1 + RB_SIZE(h->left) + RB_SIZE(h->right);
	return tmp;
}

RBNode *rb_rotate_r(RBNode *h)
{
	RBNode *tmp = h->left;
	h->left = tmp->right;
	tmp->right = h;
	tmp->colour = h->colour;
	h->colour = RED;
	tmp->n = h->n;
	h->n = 1 + RB_SIZE(h->left) + RB_SIZE(h->right);
	return tmp;
}

void rb_put(RedBlackTree *tree, char *key, void *val)
{
	tree->root = rb_put_node(tree->root, key, val);
	tree->root->colour = BLACK;
}

void *rb_get(RedBlackTree *tree, char *key)
{
	void *result = NULL;
	return result;
}

RBNode *rb_put_node(RBNode *h, char *key, void *val)
{
	if (h == NULL) {
		RBNode *node = malloc(sizeof(*node));
		init_rb_node(node, key, val, RED);
		return h;
	}
	int cmp;
	if ((cmp = strcmp(key, h->key)) < 0)
		h->left = rb_put_node(h->left, key, val);
	else if (cmp > 0)
		h->right = rb_put_node(h->right, key, val);
	else
		h->val = val;

	if (IS_RED(h->right) && !IS_RED(h->left))
		h = rb_rotate_l(h);
	if (IS_RED(h->left) && IS_RED(h->left->left))
		h = rb_rotate_r(h);
	if (IS_RED(h->left) && IS_RED(h->right))
		rb_flip_colours(h);

	h->n = 1 + RB_SIZE(h->left) + RB_SIZE(h->right);
	return h;
}

void rb_flip_colours(RBNode *h)
{
	h->colour ^= 1;
	h->left->colour ^= 1;
	h->right->colour ^= 1;
}

/*
 * Assume node is red, node->left and node->left->left are black,
 * make h->left or one of its children red.
 */
RBNode *rb_move_red_left(RBNode *h)
{
	rb_flip_colours(h);
	if (IS_RED(h->right->left)) {
		h->right = rb_rotate_r(h->right);
		h = rb_rotate_l(h);
		rb_flip_colours(h);
	}
	return h;
}

void rb_delete_min(RedBlackTree *tree)
{
	if (!IS_RED(tree->root->left) && !IS_RED(tree->root->right))
		tree->root->colour = RED;
	tree->root = rb_del_min(tree->root);
	if (tree->root->n > 0)
		tree->root->colour = BLACK;
}

RBNode *rb_del_min(RBNode *h)
{
	if (h->left == NULL)
		return NULL;
	if (!IS_RED(h->left) && !IS_RED(h->right))
		h = rb_move_red_left(h);
	h->left = rb_del_min(h->left);
	return rb_balance(h);
}

RBNode *rb_balance(RBNode *h)
{
	if (IS_RED(h->right))
		h = rb_rotate_l(h);
	if (IS_RED(h->left) && IS_RED(h->left->left))
		h = rb_rotate_r(h);
	if (IS_RED(h->left) && IS_RED(h->right))
		rb_flip_colours(h);
	h->n = 1 + RB_SIZE(h->left) + RB_SIZE(h->right);
	return h;
}
