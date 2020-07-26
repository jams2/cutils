#include <stdlib.h>
#include <stdio.h>
#include <check.h>
#include "digraph.h"


#define get_int(LISTNODE) (*(int *) (LISTNODE)->item)
const int SIZE = 5;


START_TEST(test_digraph_factory) {
	Digraph *g = digraph_factory(SIZE);

	ck_assert_int_eq(5, g->V);

	for (int i = 0; i < SIZE; ++i)
		ck_assert_ptr_null(g->vertices[i]->head);

	free_digraph(g);
} END_TEST


START_TEST(test_digraph_add_edge) {
	Digraph *g = digraph_factory(SIZE);

	add_edge(g, 0, 1);
	/* ck_assert_int_eq(1, get_int(g->vertices[0]->head)); */

	free_digraph(g);
} END_TEST


START_TEST(test_reverse_digraph) {
	Digraph *g = digraph_factory(SIZE);
	add_edge(g, 0, 1);
	add_edge(g, 0, 3);
	add_edge(g, 3, 0);

	Digraph *reversed = reverse(g);

	ck_assert_int_eq(0, get_int(reversed->vertices[1]->head));
	ck_assert_int_eq(0, get_int(reversed->vertices[3]->head));
	ck_assert_int_eq(3, get_int(reversed->vertices[0]->head));

	free_digraph(g);
	free_digraph(reversed);
} END_TEST


START_TEST(test_edge_count_incremented) {
	Digraph *g = digraph_factory(SIZE);

	add_edge(g, 0, 1);
	add_edge(g, 0, 2);
	ck_assert_int_eq(2, g->E);

	free_digraph(g);
}


START_TEST(test_dfs) {
	Digraph *g = digraph_factory(SIZE);

	add_edge(g, 0, 1);
	add_edge(g, 1, 2);
	add_edge(g, 2, 3);
	add_edge(g, 3, 4);

	int visited[SIZE];
	int edge_to[SIZE];
	init_search_arrays(g, visited, edge_to);
	dfs(g, 0, visited, edge_to);

	ck_assert_int_eq(1, visited[4]);
	ck_assert_int_eq(3, edge_to[4]);
	free_digraph(g);
} END_TEST


START_TEST(test_bfs_visited) {
	Digraph *g = digraph_factory(SIZE);

	add_edge(g, 0, 1);
	add_edge(g, 1, 2);
	add_edge(g, 2, 3);
	add_edge(g, 3, 4);

	int visited[SIZE];
	int edge_to[SIZE];
	init_search_arrays(g, visited, edge_to);
	bfs(g, 0, visited, edge_to);

	for (int i = 0; i < SIZE; ++i)
		ck_assert_int_eq(1, visited[i]);
	free_digraph(g);
} END_TEST


START_TEST(test_bfs_path_length) {
	Digraph *g = digraph_factory(SIZE);
	add_edge(g, 0, 2);
	add_edge(g, 2, 4);

	int visited[SIZE];
	int edge_to[SIZE];
	init_search_arrays(g, visited, edge_to);
	bfs(g, 0, visited, edge_to);

	ck_assert_int_eq(2, path_length(g, edge_to, 0, 4));
	ck_assert_int_eq(-1, path_length(g, edge_to, 0, 3));

	free_digraph(g);
} END_TEST


START_TEST(test_bfs_path_shortest) {
	Digraph *g = digraph_factory(SIZE);

	/* shortest path: 0 -> 2 -> 4 */
	add_edge(g, 0, 2);
	add_edge(g, 2, 4);

	/* longest path: 0 -> 1 -> 2 -> 3 -> 4 */
	add_edge(g, 0, 1);
	add_edge(g, 1, 2);
	add_edge(g, 2, 3);
	add_edge(g, 3, 4);

	int visited[SIZE];
	int edge_to[SIZE];
	init_search_arrays(g, visited, edge_to);
	bfs(g, 0, visited, edge_to);

	ck_assert_int_eq(2, path_length(g, edge_to, 0, 4));

	free_digraph(g);
} END_TEST


Suite *digraph_suite(void) {
	Suite *s;
	TCase *tc_core;

	s = suite_create("Digraph");
	tc_core = tcase_create("Core");

	tcase_add_test(tc_core, test_digraph_factory);
	tcase_add_test(tc_core, test_digraph_add_edge);
	tcase_add_test(tc_core, test_edge_count_incremented);
	tcase_add_test(tc_core, test_reverse_digraph);
	tcase_add_test(tc_core, test_dfs);
	tcase_add_test(tc_core, test_bfs_visited);
	tcase_add_test(tc_core, test_bfs_path_length);
	tcase_add_test(tc_core, test_bfs_path_shortest);

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
