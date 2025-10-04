#include "munit/munit.h"
#include "bcal.h"

/* This is just to disable an MSVC warning about conditional
 * expressions being constant, which you shouldn't have to do for your
 * code.  It's only here because we want to be able to do silly things
 * like assert that 0 != 1 for our demo. */
#if defined(_MSC_VER)
#pragma warning(disable : 4127)
#endif


static MunitResult
multiply_positive()
{
    munit_assert_double(multiply(4.0, 3.0), ==, 12.0);
    return MUNIT_OK;
}

static MunitResult
multiply_negative()
{
    munit_assert_double(multiply(-2.0, 5.0), ==, -10.0);
    return MUNIT_OK;
}

static MunitResult
multiply_zero()
{
    munit_assert_double(multiply(0.0, 100.0), ==, 0.0);
    return MUNIT_OK;
}

static MunitResult
divide_positive()
{
    munit_assert_double(divide(15.0, 3.0), ==, 5.0);
    return MUNIT_OK;
}

static MunitResult
divide_negative()
{
    munit_assert_double(divide(-10.0, 2.0), ==, -5.0);
    return MUNIT_OK;
}


static MunitResult
divide_zero()
{
    munit_assert_double(divide(5.0, 0.0), ==, 0.0);
    return MUNIT_OK;
}

static MunitResult
power_positive()
{
    munit_assert_double(power(2.0, 3.0), ==, 8.0);
    return MUNIT_OK;
}

static MunitResult
power_negative()
{
    munit_assert_double(power(2.0, -1.0), ==, -1.0);
    return MUNIT_OK;
}

static MunitResult
power_zero()
{
    munit_assert_double(power(5.0, 0.0), ==, 1.0);
    return MUNIT_OK;
}

static MunitResult
input_valid()
{
    double first, second;
    char operator;
    munit_assert_int(validate_input("5 + 3", &first, &operator, &second), !=, -1);
    return MUNIT_OK;
}

static MunitResult
input_invalid()
{
    double first, second;
    char operator;
    munit_assert_int(validate_input("abc + 5", &first, &operator, &second), ==, -1);
    return MUNIT_OK;
}

static MunitResult
input_invalid_operator()
{
    double first, second;
    char operator;
    munit_assert_int(validate_input("5 & 3", &first, &operator, &second), ==, -1);
    return MUNIT_OK;
}

static MunitResult
input_incomplete_expression()
{
    double first, second;
    char operator;
    munit_assert_int(validate_input("5 + ", &first, &operator, &second), ==, -1);
    return MUNIT_OK;
}

static MunitResult
get_number_valid()
{
    char* input = "123.45";
    int i = 0;
    int len = strlen(input);
    munit_assert_double(get_number(input, &i, len), ==, 123.45);
    return MUNIT_OK;
}

static MunitResult
get_number_invalid()
{
    char* input = "abc";
    int i = 0;
    int len = strlen(input);
    munit_assert_double(get_number(input, &i, len), ==, -1.0);
    return MUNIT_OK;
}

static MunitResult
get_operator_valid()
{
    char* input = "+";
    int i = 0;
    int len = strlen(input);
    munit_assert_char(get_operator(input, &i, len), ==, '+');
    return MUNIT_OK;
}

static MunitResult
get_operator_invalid()
{
    char* input = "&";
    int i = 0;
    int len = strlen(input);
    munit_assert_char(get_operator(input, &i, len), ==, '\0');
    return MUNIT_OK;
}

/* Creating a test suite is pretty simple.  First, you'll need an
 * array of tests: */
static MunitTest test_suite_tests[] = {
    {(char *)"/multiply/positive", multiply_positive, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    {(char *)"/multiply/negative", multiply_negative, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    {(char *)"/multiply/zero", multiply_zero, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    {(char *)"/divide/positive", divide_positive, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    {(char *)"/divide/negative", divide_negative, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    {(char *)"/divide/zero", divide_zero, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    {(char *)"/power/positive", power_positive, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    {(char *)"/power/zero", power_zero, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    {(char *)"/power/negative", power_negative, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},

    {(char *)"/input/valid", input_valid, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    {(char *)"/input/invalid", input_invalid, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    {(char *)"/input/invalid_operator", input_invalid_operator, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    {(char *)"/input/incomplete_expression", input_incomplete_expression, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    {(char *)"/get_number/valid", get_number_valid, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    {(char *)"/get_number/invalid", get_number_invalid, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    {(char *)"/get_operator/valid", get_operator_valid, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    {(char *)"/get_operator/invalid", get_operator_invalid, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},

    {NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL}};

/* If you wanted to have your test suite run other test suites you
 * could declare an array of them.  Of course each sub-suite can
 * contain more suites, etc. */
/* static const MunitSuite other_suites[] = { */
/*   { "/second", test_suite_tests, NULL, 1, MUNIT_SUITE_OPTION_NONE }, */
/*   { NULL, NULL, NULL, 0, MUNIT_SUITE_OPTION_NONE } */
/* }; */

/* Now we'll actually declare the test suite.  You could do this in
 * the main function, or on the heap, or whatever you want. */
static const MunitSuite test_suite = {
    /* This string will be prepended to all test names in this suite;
     * for example, "/example/rand" will become "/µnit/example/rand".
     * Note that, while it doesn't really matter for the top-level
     * suite, NULL signal the end of an array of tests; you should use
     * an empty string ("") instead. */
    (char *)"",
    /* The first parameter is the array of test suites. */
    test_suite_tests,
    /* In addition to containing test cases, suites can contain other
     * test suites.  This isn't necessary in this example, but it can be
     * a great help to projects with lots of tests by making it easier
     * to spread the tests across many files.  This is where you would
     * put "other_suites" (which is commented out above). */
    NULL,
    /* An interesting feature of µnit is that it supports automatically
     * running multiple iterations of the tests.  This is usually only
     * interesting if you make use of the PRNG to randomize your tests
     * cases a bit, or if you are doing performance testing and want to
     * average multiple runs.  0 is an alias for 1. */
    1,
    /* Just like MUNIT_TEST_OPTION_NONE, you can provide
     * MUNIT_SUITE_OPTION_NONE or 0 to use the default settings. */
    MUNIT_SUITE_OPTION_NONE};

#include <stdlib.h>

int main(int argc, char *argv[MUNIT_ARRAY_PARAM(argc + 1)])
{

    return munit_suite_main(&test_suite, (void *)"µnit", argc, argv);
}