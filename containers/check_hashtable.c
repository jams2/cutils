#include "containers.h"
#include <stdlib.h>
#include <stdio.h>
#include <check.h>
#include <assert.h>

static HashTable *ht;

void setup_hashtable()
{
	ht = malloc(sizeof(*ht));
	ht_init(ht);
}

void teardown_hashtable()
{
	free(ht->vals);
	free(ht->keys);
	free(ht);
}


START_TEST(test_init_hashtable)
{
	ck_assert_int_eq(0, ht->n_keys);
	ck_assert_int_eq(ht_initial_size, ht->size);
	ck_assert_int_eq(ht_initial_size-1, ht->sizemask);
} END_TEST


Suite *container_suite(void) {
	Suite *s;
	TCase *tc_hashtable;

	s = suite_create("Hashtable Suite");
	tc_hashtable = tcase_create("Hashtable");
	tcase_add_checked_fixture(tc_hashtable, setup_hashtable, teardown_hashtable);

	tcase_add_test(tc_hashtable, test_init_hashtable);
	suite_add_tcase(s, tc_hashtable);

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
