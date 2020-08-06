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


START_TEST(test_insert)
{
	ht_put(ht, "key", NULL);
	ck_assert_int_eq(1, ht->n_keys);
} END_TEST


START_TEST(test_ht_grows)
{
	/* the hashtable should be doubled in size when we reach
	 * half the current capacity.
	 */
	for (int i = 0; i <= ht_initial_size/2; ++i)
		ht_put(ht, (char *) &i, NULL);
	ck_assert_int_eq(ht_initial_size*2, ht->size);
	ck_assert_int_eq(9, ht->n_keys);
} END_TEST


START_TEST(test_ht_del)
{
	ht_put(ht, "key", NULL);
	ht_del(ht, "key");
	ck_assert_int_eq(0, ht->n_keys);
} END_TEST


Suite *container_suite(void) {
	Suite *s;
	TCase *tc_hashtable;

	s = suite_create("Hashtable Suite");
	tc_hashtable = tcase_create("Hashtable");
	tcase_add_checked_fixture(tc_hashtable, setup_hashtable, teardown_hashtable);

	tcase_add_test(tc_hashtable, test_init_hashtable);
	tcase_add_test(tc_hashtable, test_insert);
	tcase_add_test(tc_hashtable, test_ht_grows);
	tcase_add_test(tc_hashtable, test_ht_del);
	suite_add_tcase(s, tc_hashtable);

	return s;
}


int main(void) {
	int no_failed = 0;
	Suite *s;
	SRunner *runner;

	s = container_suite();
	runner = srunner_create(s);
	srunner_set_fork_status(runner, CK_NOFORK);

	srunner_run_all(runner, CK_NORMAL);
	no_failed = srunner_ntests_failed(runner);
	srunner_free(runner);
	return (no_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
