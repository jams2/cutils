#include "containers.h"
#include <stdlib.h>
#include <stdio.h>
#include <check.h>

LinkedList *list;
Queue *queue;
RedBlackTree *tree;

void setup_linked_list()
{
	list = malloc(sizeof(*list));
	init_list(list, sizeof(int));
}

void teardown_linked_list()
{
	free_list(list, NULL);
}

void setup_queue()
{
	queue = malloc(sizeof(*queue));
	init_queue(queue, sizeof(int));
}

void teardown_queue()
{
	free_queue(queue, NULL);
}

void setup_rb_tree()
{
	tree = malloc(sizeof(*tree));
	init_rb_tree(tree);
}

void teardown_rb_tree()
{
	free_rb_tree(tree);
}


START_TEST(test_init_list)
{
	ck_assert_int_eq(0, list->len);
	ck_assert_ptr_null(list->head);
	ck_assert_ptr_null(list->tail);
} END_TEST


START_TEST(test_make_node)
{
	int val = 1;
	int node_err = 0;
	ListNode *node = make_node(&val, sizeof(int), NULL, &node_err);
	ck_assert_int_eq(1, node_int_val(node));
	ck_assert_ptr_null(node->next);
	free_node(node, NULL);
} END_TEST


START_TEST(test_list_append_on_empty)
{
	int val = 5;
	ll_append(list, &val);
	ck_assert_int_eq(val, node_int_val(list->head));
	ck_assert_ptr_eq(list->head, list->tail);
} END_TEST


START_TEST(test_list_prepend_on_empty)
{
	int val = 5;
	ll_prepend(list, &val);
	ck_assert_int_eq(val, node_int_val(list->tail));
	ck_assert_ptr_eq(list->head, list->tail);
} END_TEST


START_TEST(test_list_append_nonempty)
{
	int val = 5;
	ll_append(list, &val);
	val = 6;
	ll_append(list, &val);
	ck_assert_int_eq(6, node_int_val(list->tail));
} END_TEST


START_TEST(test_list_prepend_nonempty)
{
	int val = 5;
	ll_append(list, &val);
	val = 6;
	ll_prepend(list, &val);
	ck_assert_int_eq(6, node_int_val(list->head));
} END_TEST


START_TEST(test_remove_head_empty)
{
	int err;
	int *val = remove_head(list, &err);
	ck_assert_int_eq(err, LL_REMOVE_EMPTY);
	ck_assert_ptr_null(val);
	free(val);
} END_TEST


START_TEST(test_remove_tail_empty)
{
	int err;
	int *val = remove_tail(list, &err);
	ck_assert_int_eq(err, LL_REMOVE_EMPTY);
	ck_assert_ptr_null(val);
	free(val);
} END_TEST


START_TEST(test_remove_head_nonempty)
{
	int val = 5;
	ll_append(list, &val);
	int err = -1;
	int *res = remove_head(list, &err);
	ck_assert_int_eq(val, *res);
	ck_assert_int_eq(-1, err);
	free(res);
} END_TEST


START_TEST(test_remove_tail_nonempty)
{
	int val = 5;
	ll_append(list, &val);
	int err = -1;
	int *res = remove_tail(list, &err);
	ck_assert_int_eq(val, *res);
	ck_assert_int_eq(-1, err);
	free(res);
} END_TEST


START_TEST(test_remove_node_first)
{
	int val = 5;
	int err;
	ll_append(list, &val);
	val = 6;
	ll_append(list, &val);
	int *res = remove_node(list, 0, &err);
	ck_assert_int_eq(5, *res);
	ck_assert_int_eq(6, node_int_val(list->head));
	free(res);
} END_TEST


START_TEST(test_remove_node_mid)
{
	for (int i = 0; i < 100; ++i)
		ll_append(list, &i);
	int err;
	int *res = remove_node(list, 50, &err);
	ck_assert_int_eq(50, *res);
	ck_assert_int_eq(99, list->len);
	free(res);
} END_TEST


START_TEST(test_enqueue)
{
	int val = 5;
	enqueue(queue, &val);
	ck_assert_int_eq(5, node_int_val(queue->head));

	val = 6;
	enqueue(queue, &val);
	ck_assert_int_eq(6, node_int_val(queue->tail));

	int *ret;
	while (queue->len > 0) {
		ret = dequeue(queue, &val);
		free(ret);
	}
} END_TEST


START_TEST(test_dequeue)
{
	int err;
	int *val;
	for (int i = 0; i < 5; ++i)
		enqueue(queue, &i);
	for (int i = 0; i < 5; ++i) {
		val = (int *)dequeue(queue, &err);
		ck_assert_int_eq(i, *val);
		free(val);
	}
} END_TEST


START_TEST(test_create_rbnode)
{
	RBNode *node = malloc(sizeof(*node));
	init_rb_node(node, "a", NULL, RED);
	ck_assert_int_eq(1, node->n);
	ck_assert_int_eq(RED, node->colour);
	ck_assert_str_eq("a", node->key);
	ck_assert_ptr_null(node->val);
	ck_assert_ptr_null(node->left);
	ck_assert_ptr_null(node->right);
	free_rb_nodes(node);
} END_TEST


START_TEST(test_rb_put_lt)
{
	rb_put(tree, "n", NULL);
	rb_put(tree, "c", NULL);
	ck_assert_str_eq("n", tree->root->key);
} END_TEST


START_TEST(test_rb_insertions)
{
	rb_put(tree, "j", NULL);
	ck_assert_int_eq(BLACK, tree->root->colour);

	/* e should be red and left of root */
	rb_put(tree, "e", NULL);
	ck_assert_str_eq("j", tree->root->key);
	ck_assert_int_eq(RED, tree->root->left->colour);

	/* p goes right, flip colours */
	rb_put(tree, "p", NULL);
	ck_assert_str_eq("p", tree->root->right->key);
	ck_assert_int_eq(BLACK, tree->root->left->colour);
	ck_assert_int_eq(BLACK, tree->root->right->colour);

	/* p becomes a 2-node */
	rb_put(tree, "o", NULL);
	ck_assert_str_eq("p", tree->root->right->key);
	ck_assert_str_eq("o", tree->root->right->left->key);
	ck_assert_int_eq(RED, tree->root->right->left->colour);

	/* inserting n causes a few rotations */
	rb_put(tree, "n", NULL);
	ck_assert_str_eq("o", tree->root->key);
	ck_assert_str_eq("j", tree->root->left->key);
	ck_assert_str_eq("p", tree->root->right->key);
	ck_assert_str_eq("n", tree->root->left->right->key);
	ck_assert_int_eq(RED, tree->root->left->colour);
} END_TEST


Suite *container_suite(void) {
	Suite *s;
	TCase *tc_linked_list;
	TCase *tc_queue;
	TCase *tc_rb_tree;

	s = suite_create("Containers");
	tc_linked_list = tcase_create("Linked_list");
	tc_queue = tcase_create("Queue");
	tc_rb_tree = tcase_create("RB Tree");

	tcase_add_checked_fixture(tc_linked_list, setup_linked_list, teardown_linked_list);
	tcase_add_checked_fixture(tc_queue, setup_queue, teardown_queue);
	tcase_add_checked_fixture(tc_rb_tree, setup_rb_tree, teardown_rb_tree);

	tcase_add_test(tc_linked_list, test_init_list);
	tcase_add_test(tc_linked_list, test_make_node);
	tcase_add_test(tc_linked_list, test_list_append_on_empty);
	tcase_add_test(tc_linked_list, test_list_prepend_on_empty);
	tcase_add_test(tc_linked_list, test_list_append_nonempty);
	tcase_add_test(tc_linked_list, test_list_prepend_nonempty);
	tcase_add_test(tc_linked_list, test_remove_head_empty);
	tcase_add_test(tc_linked_list, test_remove_head_nonempty);
	tcase_add_test(tc_linked_list, test_remove_tail_empty);
	tcase_add_test(tc_linked_list, test_remove_tail_nonempty);
	tcase_add_test(tc_linked_list, test_remove_node_first);
	tcase_add_test(tc_linked_list, test_remove_node_mid);
	suite_add_tcase(s, tc_linked_list);

	tcase_add_test(tc_queue, test_enqueue);
	tcase_add_test(tc_queue, test_dequeue);
	suite_add_tcase(s, tc_queue);

	tcase_add_test(tc_rb_tree, test_create_rbnode);
	tcase_add_test(tc_rb_tree, test_rb_put_lt);
	tcase_add_test(tc_rb_tree, test_rb_insertions);
	suite_add_tcase(s, tc_rb_tree);

	return s;
}


int main(void) {
	int no_failed = 0;
	Suite *s;
	SRunner *runner;

	s = container_suite();
	runner = srunner_create(s);

	srunner_run_all(runner, CK_NORMAL);
	no_failed = srunner_ntests_failed(runner);
	srunner_free(runner);
	return (no_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
