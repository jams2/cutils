#include <stdlib.h>
#include <stdio.h>
#include <check.h>
#include "digraph.h"

START_TEST(test_digraph_init) {
    const int SIZE = 5;
    struct intnode *digraph[SIZE];
    init_digraph(digraph, SIZE);
    for (int i = 0; i < SIZE; ++i)
	ck_assert_ptr_eq(digraph[i], NULL);
} END_TEST


Suite *digraph_suite(void) {
  Suite *s;
  TCase *tc_core;

  s = suite_create("Digraph");
  tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_digraph_init);
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
