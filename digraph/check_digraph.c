#include <stdlib.h>
#include <stdio.h>
#include <check.h>
#include "digraph.h"


const int SIZE = 5;


START_TEST(test_digraph_factory) {
    t_digraph *g = digraph_factory(SIZE);

    ck_assert_int_eq(5, g->V);

    for (int i = 0; i < SIZE; ++i)
	ck_assert_ptr_null(g->vertices[i]);

    free_digraph(g);
} END_TEST


START_TEST(test_digraph_add_edge) {
    t_digraph *g = digraph_factory(SIZE);

    add_edge(g, 0, 1);
    ck_assert_int_eq(g->vertices[0]->val, 1);

    free_digraph(g);
}

START_TEST(test_reverse_digraph) {
    t_digraph *g = digraph_factory(SIZE);
    add_edge(g, 0, 1);
    add_edge(g, 0, 3);
    add_edge(g, 3, 0);

    t_digraph *reversed = reverse(g);

    ck_assert_int_eq(0, reversed->vertices[1]->val);
    ck_assert_int_eq(0, reversed->vertices[3]->val);
    ck_assert_int_eq(3, reversed->vertices[0]->val);

    free_digraph(g);
    free_digraph(reversed);
}

START_TEST(test_dfs) {
    t_digraph *g = digraph_factory(SIZE);

    int visited[SIZE];
    int edge_to[SIZE];
    for (int i = 0; i < SIZE; ++i) {
	visited[i] = 0;
	edge_to[i] = -1;
    }

    add_edge(g, 0, 1);
    add_edge(g, 1, 2);
    add_edge(g, 2, 3);
    add_edge(g, 3, 4);

    dfs(g, 0, visited, edge_to);

    ck_assert_int_eq(1, visited[4]);
    ck_assert_int_eq(3, edge_to[4]);
    free_digraph(g);
}


Suite *digraph_suite(void) {
  Suite *s;
  TCase *tc_core;

  s = suite_create("Digraph");
  tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_digraph_factory);
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
