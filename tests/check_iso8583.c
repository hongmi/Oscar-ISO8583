#include <config.h>
#include <stdlib.h>
#include <stdint.h>
#include <check.h>

void setup(void)
{
    
}

void teardown(void)
{
    
}

START_TEST(test_1)
{
    ck_assert_int_eq(5, 5);
    ck_assert_str_eq("USD", "USD");
}
END_TEST

START_TEST(test_2)
{
    int *m = NULL;
    ck_assert_msg(m == NULL,
                  "NULL should be returned on attempt to create with "
                  "a invalid value");
}
END_TEST

START_TEST(test_3)
{
    ck_abort_msg("Zero is a valid Value");
}
END_TEST

Suite * iso8583_suite(void)
{
    Suite *s;
    TCase *tc_core;
    TCase *tc_limits;

    s = suite_create("ISO8583");

    /* Core test case */
    tc_core = tcase_create("Core");

    tcase_add_checked_fixture(tc_core, setup, teardown);
    tcase_add_test(tc_core, test_1);
    suite_add_tcase(s, tc_core);

    /* Limits test case */
    tc_limits = tcase_create("Limits");

    tcase_add_test(tc_limits, test_2);
    tcase_add_test(tc_limits, test_3);
    suite_add_tcase(s, tc_limits);

    return s;
}

int main(void)
{
    int number_failed;
    Suite *s;
    SRunner *sr;

    s = iso8583_suite();
    sr = srunner_create(s);

    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
