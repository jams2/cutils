#include "containers.h"
#include <stdlib.h>
#include <stdio.h>
#include <check.h>

LinkedList *list;
Queue *queue;

void ll_setup_once()
{
	list = malloc(sizeof(*list));
}

void ll_teardown_once()
{
	free_list(list, NULL);
}

void ll_setup_all()
{
	init_list(list, sizeof(int));
}

void ll_teardown_all() {;}

void queue_setup_once()
{
	queue = malloc(sizeof(*queue));
}

void queue_teardown_once()
{
	free_queue(queue, NULL);
}

void queue_setup_all()
{
	init_queue(queue, sizeof(int));
}

void queue_teardown_all() {;}


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
} END_TEST


START_TEST(test_remove_tail_empty)
{
	int err;
	int *val = remove_tail(list, &err);
	ck_assert_int_eq(err, LL_REMOVE_EMPTY);
	ck_assert_ptr_null(val);
} END_TEST


START_TEST(test_remove_head_nonempty)
{
	int val = 5;
	ll_append(list, &val);
	int err = -1;
	int *res = remove_head(list, &err);
	ck_assert_int_eq(val, *res);
	ck_assert_int_eq(-1, err);
} END_TEST


START_TEST(test_remove_tail_nonempty)
{
	int val = 5;
	ll_append(list, &val);
	int err = -1;
	int *res = remove_tail(list, &err);
	ck_assert_int_eq(val, *res);
	ck_assert_int_eq(-1, err);
} END_TEST


START_TEST(test_remove_node_first)
{
	int val = 5;
	int err;
	ll_append(list, &val);
	val = 6;
	ll_append(list, &val);
	int res = *(int *)remove_node(list, 0, &err);
	ck_assert_int_eq(5, res);
	ck_assert_int_eq(6, node_int_val(list->head));
} END_TEST


START_TEST(test_remove_node_mid)
{
	for (int i = 0; i < 100; ++i)
		ll_append(list, &i);
	int err;
	int res = *(int *)remove_node(list, 50, &err);
	ck_assert_int_eq(50, res);
	ck_assert_int_eq(99, list->len);
} END_TEST


START_TEST(test_enqueue)
{
	int val = 5;
	enqueue(queue, &val);
	ck_assert_int_eq(5, node_int_val(queue->head));

	val = 6;
	enqueue(queue, &val);
	ck_assert_int_eq(6, node_int_val(queue->tail));
} END_TEST


START_TEST(test_dequeue)
{
	int err;
	for (int i = 0; i < 5; ++i)
		enqueue(queue, &i);
	for (int i = 0; i < 5; ++i)
		ck_assert_int_eq(i, *(int *)dequeue(queue, &err));
} END_TEST


Suite *container_suite(void) {
	Suite *s;
	TCase *tc_linked_list;
	TCase *tc_queue;

	s = suite_create("Containers");
	tc_linked_list = tcase_create("Linked_list");
	tc_queue = tcase_create("Queue");

	tcase_add_checked_fixture(tc_linked_list, ll_setup_all, ll_teardown_all);
	tcase_add_unchecked_fixture(tc_linked_list, ll_setup_once, ll_teardown_once);
	tcase_add_checked_fixture(tc_queue, queue_setup_all, queue_teardown_all);
	tcase_add_unchecked_fixture(tc_queue, queue_setup_once, queue_teardown_once);

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
