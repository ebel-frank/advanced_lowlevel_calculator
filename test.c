#include "minunit.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "calculator.h"
#include "history.h"
#include "utils.h"

int tests_run = 0;

// History Management Tests
MU_TEST(test_init_history_creates_valid_structure)
{
    CalculationHistory hist;
    mu_assert(init_history(&hist) == HISTORY_SUCCESS, "init_history should succeed ");
    mu_assert(hist.count == 0, "initial count should be 0");
    mu_assert(hist.capacity > 0, "initial capacity should be > 0");
    mu_assert(hist.calculations != NULL, "calculations array should not be NULL");
    cleanup_history(&hist);
}

MU_TEST(test_add_calculation_grows_array_correctly)
{
    CalculationHistory hist;
    init_history(&hist);

    int initial_capacity = hist.capacity;

    // Add calculations beyond initial capacity
    for (int i = 0; i < initial_capacity + 3; i++)
    {
        char expr[32];
        sprintf(expr, "%d + %d", i, i);
        char res[32];
        sprintf(res, "%d", i * 2);
        mu_assert(add_calculation(&hist, expr, res, 0) == HISTORY_SUCCESS, "add_calculation should succeed ");
    }

    mu_assert(hist.capacity > initial_capacity, " array should have grown");
    mu_assert(hist.count == initial_capacity + 3, " count should match additions ");

    cleanup_history(&hist);
}

MU_TEST(test_arithmetic_operations)
{
    CalcResult error;

    // Test basic operations
    mu_assert("5 + 3 should equal 8", add(5.0, 3.0) == 8.0);
    mu_assert("10 - 4 should equal 6", subtract(10.0, 4.0) == 6.0);
    mu_assert("7 * 6 should equal 42", multiply(7.0, 6.0) == 42.0);

    // Test division
    double result = divide(15.0, 3.0, &error);
    mu_assert(result == 5.0 && error == CALC_SUCCESS, "15 / 3 should equal 5");

    // Test division by zero
    result = divide(5.0, 0.0, &error);
    mu_assert(error == CALC_DIVISION_BY_ZERO, " division by zero should return error ");
}

MU_TEST(test_file_persistence_roundtrip)
{
    CalculationHistory hist1, hist2;
    init_history(&hist1);
    init_history(&hist2);

    // Add test data to first history
    add_calculation(&hist1, "5 + 3", "8", 0);
    add_calculation(&hist1, "10 / 0", "Division by zero!", 1);
    add_calculation(&hist1, "7 * 6", "42", 0);

    // Save to file
    mu_assert(save_history_to_file(&hist1, "test_history.csv") == HISTORY_SUCCESS, "save should succeed ");

    // Load into second history
    mu_assert(load_history_from_file(&hist2, "test_history.csv") == HISTORY_SUCCESS, "load should succeed ");

    // Verify data matches
    mu_assert(hist1.count == hist2.count, " loaded count should match saved count ");

    for (int i = 0; i < hist1.count; i++)
    {
        mu_assert(strcmp(hist1.calculations[i].expression_str,
                         hist2.calculations[i].expression_str) == 0,
                  "expressions should match");
        mu_assert(strcmp(hist1.calculations[i].result, hist2.calculations[i].result) == 0, " results should match ");
        mu_assert(hist1.calculations[i].is_error == hist2.calculations[i].is_error, " error flags should match ");
    }

    cleanup_history(&hist1);
    cleanup_history(&hist2);
    remove("test_history.csv"); // Clean up test file
}

// Test power function behavior
MU_TEST(test_power_function)
{
    CalcResult err;

    // 2^10 == 1024
    double p = power(2.0, 10, &err);
    mu_assert(err == CALC_SUCCESS, "power should succeed for positive exponent");
    mu_assert_double_eq(1024.0, p);

    // Any number to power 0 should be 1
    p = power(5.0, 0, &err);
    mu_assert(err == CALC_SUCCESS, "power with exponent 0 should succeed");
    mu_assert_double_eq(1.0, p);
}

// Test expression parsing
MU_TEST(test_parse_expression_valid_and_invalid)
{
    double out = 0.0;
    char err_msg[128] = {0};

    // valid expression
    CalcResult r = parse_expression("5 * 4", &out, err_msg);
    mu_assert(r == CALC_SUCCESS, "parse_expression should succeed for '5 * 4'");
    mu_assert_double_eq(20.0, out);

    // Leading and multiple spaces and unary minus
    mu_assert(parse_expression("   -5   *   2  ", &out, err_msg) == CALC_SUCCESS, "parse should handle whitespace and unary minus");
    mu_assert_double_eq(-10.0, out);

    // invalid expression should return an error code
    out = 0.0;
    r = parse_expression("not an expr", &out, err_msg);
    mu_assert(r == CALC_INVALID_INPUT, "parse_expression should return CALC_INVALID_INPUT for bad input");
}

// Test validation helpers for numbers and operators
MU_TEST(test_validation_helpers)
{
    // is_valid_number should accept plain integers and decimal numbers
    mu_assert(is_valid_number("123") == 1, "'123' should be a valid number");
    mu_assert(is_valid_number("-3.14") == 1, "'-3.14' should be a valid number");
    mu_assert(is_valid_number("12a") == 0, "'12a' should NOT be a valid number");

    // is_valid_operator should accept standard operators
    mu_assert(is_valid_operator('+') == 1, "+ should be valid operator");
    mu_assert(is_valid_operator('/') == 1, "/ should be valid operator");
    mu_assert(is_valid_operator('x') == 0, "x should NOT be a valid operator");
}

// Test history clear and replay behavior
MU_TEST(test_history_clear_and_replay)
{
    CalculationHistory hist;
    init_history(&hist);

    // Add a single calculation and ensure count increments
    mu_assert(add_calculation(&hist, "2 + 2", "4", 0) == HISTORY_SUCCESS, "add_calculation should succeed");
    mu_assert(get_history_count(&hist) == 1, "history count should be 1 after addition");

    // Replay the calculation and verify the returned result string
    char replay_result[128] = {0};
    mu_assert(replay_calculation(&hist, 0, replay_result) == HISTORY_SUCCESS, "replay_calculation should succeed for valid index");
    mu_assert_string_eq("4", replay_result);

    // Clear history and ensure count is zero
    mu_assert(clear_history(&hist) == HISTORY_SUCCESS, "clear_history should succeed");
    mu_assert(get_history_count(&hist) == 0, "history count should be 0 after clear");

    cleanup_history(&hist);
}

// string_to_double edge cases: empty, whitespace, garbage
MU_TEST(test_string_to_double_edge_cases)
{
    double out;

    mu_assert(string_to_double("", &out) == 0, "empty string should not parse as number");
    mu_assert(string_to_double("   ", &out) == 0, "whitespace-only should not parse as number");
    mu_assert(string_to_double("12abc", &out) == 0, "trailing garbage should fail parse");
}

// Run all tests
int main(int argc, char **argv)
{
    MU_RUN_TEST(test_init_history_creates_valid_structure);
    MU_RUN_TEST(test_add_calculation_grows_array_correctly);
    MU_RUN_TEST(test_arithmetic_operations);
    MU_RUN_TEST(test_power_function);
    MU_RUN_TEST(test_parse_expression_valid_and_invalid);
    MU_RUN_TEST(test_validation_helpers);
    MU_RUN_TEST(test_history_clear_and_replay);
    MU_RUN_TEST(test_file_persistence_roundtrip);
    MU_RUN_TEST(test_string_to_double_edge_cases);
    
    MU_REPORT();
    return MU_EXIT_CODE;
}