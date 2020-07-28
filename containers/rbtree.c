#include <stdlib.h>
#include <string.h>
#include "containers.h"


RBNode *rb_new_node(char *key, void *val, enum colour colour)
{
	RBNode *node = malloc(sizeof(*node));
	node->key = key;
	node->val = val;
	node->left = node->right = NULL;
	node->n = 1;
	node->colour = colour;
	return node;
}

void rb_init_tree(RedBlackTree *tree)
{
	tree->root = NULL;
}

void rb_free_tree(RedBlackTree *tree)
{
	rb_free_all_nodes(tree->root);
	free(tree);
}

void rb_free_all_nodes(RBNode *node)
{
	if (node == NULL)
		return;
	rb_free_all_nodes(node->right);
	rb_free_all_nodes(node->left);
	free(node);
}

void rb_free_node(RBNode *node)
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

RBNode *rb_put_node(RBNode *node, char *key, void *val)
{
	if (node == NULL)
		return rb_new_node(key, val, RED);
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
