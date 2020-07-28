#include <stdlib.h>
#include <string.h>
#include "containers.h"


void init_rb_node(RBNode *node, char *key, void *val, enum colour colour)
{
	node->key = key;
	node->val = val;
	node->left = node->right = NULL;
	node->n = 1;
	node->colour = colour;
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

void free_rb_nodes(RBNode *node)
{
	if (node == NULL)
		return;
	free_rb_nodes(node->right);
	free_rb_nodes(node->left);
	free(node);
}

void free_rb_node(RBNode *node)
{
	free(node);
}

RBNode *rb_l_rot(RBNode *node)
{
	RBNode *tmp = node->right;
	node->right = tmp->left;
	tmp->left = node;
	tmp->colour = node->colour;
	node->colour = RED;
	tmp->n = node->n;
	node->n = 1 + RB_SIZE(node->left) + RB_SIZE(node->right);
	return tmp;
}

RBNode *rb_r_rot(RBNode *node)
{
	RBNode *tmp = node->left;
	node->left = tmp->right;
	tmp->right = node;
	tmp->colour = node->colour;
	node->colour = RED;
	tmp->n = node->n;
	node->n = 1 + RB_SIZE(node->left) + RB_SIZE(node->right);
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

RBNode *rb_put_node(RBNode *node, char *key, void *val)
{
	if (node == NULL) {
		RBNode *node = malloc(sizeof(*node));
		init_rb_node(node, key, val, RED);
		return node;
	}
	int cmp;
	if ((cmp = strcmp(key, node->key)) < 0)
		node->left = rb_put_node(node->left, key, val);
	else if (cmp > 0)
		node->right = rb_put_node(node->right, key, val);
	else
		node->val = val;

	if (IS_RED(node->right) && !IS_RED(node->left))
		node = rb_l_rot(node);
	if (IS_RED(node->left) && IS_RED(node->left->left))
		/* short circuit prevents dereferencing null */
		node = rb_r_rot(node);
	if (IS_RED(node->left) && IS_RED(node->right))
		rb_flip_colours(node);

	node->n = 1 + RB_SIZE(node->left) + RB_SIZE(node->right);
	return node;
}

void rb_flip_colours(RBNode *node)
{
	node->colour = RED;
	node->left->colour = node->right->colour = BLACK;
}
