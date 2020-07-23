#include <stdlib.h>
#include <stdio.h>
#include <check.h>
#include "digraph.h"

const int SIZE = 5;

START_TEST(test_digraph_init) {
    struct intnode *digraph[SIZE];
    init_digraph(digraph, SIZE);
    for (int i = 0; i < SIZE; ++i)
	ck_assert_ptr_eq(digraph[i], NULL);
} END_TEST

START_TEST(test_digraph_add_edge) {
    struct intnode *digraph[SIZE];
    init_digraph(digraph, SIZE);
    add_edge(digraph, 0, 1);
    ck_assert_int_eq(digraph[0]->val, 1);
    free_digraph(digraph, SIZE);
}

START_TEST(test_reverse_digraph) {
    struct intnode *digraph[SIZE];
    init_digraph(digraph, SIZE);
    struct intnode *reversed[SIZE];
    init_digraph(reversed, SIZE);
    add_edge(digraph, 0, 1);
    add_edge(digraph, 0, 3);
    add_edge(digraph, 3, 0);
    reverse(digraph, reversed, SIZE);

    ck_assert_int_eq(reversed[1]->val, 0);
    ck_assert_int_eq(reversed[3]->val, 0);
    ck_assert_int_eq(reversed[0]->val, 3);

    free_digraph(digraph, SIZE);
    free_digraph(reversed, SIZE);
}

START_TEST(test_dfs) {
    struct intnode *digraph[SIZE];
    init_digraph(digraph, SIZE);

    int visited[SIZE];
    int edge_to[SIZE];
    for (int i = 0; i < SIZE; ++i) {
	visited[i] = 0;
	edge_to[i] = -1;
    }

    add_edge(digraph, 0, 1);
    add_edge(digraph, 1, 2);
    add_edge(digraph, 2, 3);
    add_edge(digraph, 3, 4);

    dfs(digraph, 0, visited, edge_to);

    ck_assert_int_eq(1, visited[4]);
    ck_assert_int_eq(3, edge_to[4]);
    free_digraph(digraph, SIZE);
}


Suite *digraph_suite(void) {
  Suite *s;
  TCase *tc_core;

  s = suite_create("Digraph");
  tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_digraph_init);
  tcase_add_test(tc_core, test_digraph_add_edge);
  tcase_add_test(tc_core, test_reverse_digraph);
  tcase_add_test(tc_core, test_dfs);
  suite_add_tcase(s, tc_core);

  return s;
}

int main(void) {
  int no_failed = 0;
  Suite *s;
  SRunner *runner;

  s = digraph_suite();
  runner = srunner_create(s);

  srunner_run_all(runner, CK_NORMAL);
  no_failed = srunner_ntests_failed(runner);
  srunner_free(runner);
  return (no_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
