#include "calculator.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "utils.h"

// Function to parse and return a number from the input string
int get_number(char *prompt, int *start_idx, int len, double *number)
{
    // Skip leading whitespace
    while (*start_idx < len && isspace(prompt[*start_idx]))
    {
        (*start_idx)++;
    }

    char num1[32] = {};
    int num_idx = 0;
    // Check for an optional minus sign
    if (*start_idx < len && (prompt[*start_idx] == '-' || prompt[*start_idx] == '+'))
    {
        num1[num_idx++] = prompt[*start_idx];
        (*start_idx)++;
    }
    // num1[num_idx] = '\0';

    // Check if the current character is a digit
    if (*start_idx >= len || !isdigit(prompt[*start_idx]))
    {
        return -1; // Indicate error
    }

    // Loop through and parse the digits to build the number
    while (*start_idx < len && (isdigit(prompt[*start_idx]) || prompt[*start_idx] == '.'))
    {
        num1[num_idx++] = prompt[*start_idx];
        (*start_idx)++;
    }

    *number = atof(num1);
    return 0;
}

// Function to get the operator from the input string
int get_operator(char *prompt, int *start_idx, int len, char *operator)
{
    // Skip leading whitespace
    while (*start_idx < len && isspace(prompt[*start_idx]))
    {
        (*start_idx)++;
    }

    // Check if the current character is a valid operator
    if (*start_idx < len && (prompt[*start_idx] == '+' || prompt[*start_idx] == '-' || prompt[*start_idx] == '*' || prompt[*start_idx] == '/' || prompt[*start_idx] == '^'))
    {
        *operator = prompt[*start_idx];
        (*start_idx)++;
        return 0;
    }
    else
    {
        return -1; // Indicate error
    }
}

// Function to perform addition operation
double add(double a, double b)
{
    return a + b;
}

// Function to perform subtraction operation
double subtract(double a, double b)
{
    return a - b;
}

// Function to perform multiplication operation
double multiply(double a, double b)
{
    return a * b;
}

// Function to perform division operation
double divide(double a, double b, CalcResult *error)
{
    if (error == NULL)
        return 0.0;

    if (b == 0.0)
    {
        *error = CALC_DIVISION_BY_ZERO;
        return 0.0;
    }

    *error = CALC_SUCCESS;
    return a / b;
}

double power(double base, int exponent, CalcResult *error)
{
    if (error == NULL)
        return 0.0;

    // Check for invalid cases
    if (exponent < 0)
    {
        *error = CALC_INVALID_INPUT;
        return 0.0;
    }

    *error = CALC_SUCCESS;
    return pow(base, exponent);
}

// Expression parsing with robust error handling
CalcResult parse_expression(const char *input, double *result, char *error_msg)
{
    if (input == NULL || result == NULL)
    {
        return CALC_INVALID_INPUT;
    }

    // Create working copy and remove spaces
    char *expr_copy = safe_string_copy(input);
    if (expr_copy == NULL)
    {
        return CALC_INVALID_INPUT;
    }
    remove_spaces(expr_copy);

    if (strlen(expr_copy) == 0)
    {
        free(expr_copy);
        return CALC_INVALID_INPUT;
    }

    // Find operator (skip first character if it 's a sign)
    int op_pos = -1;
    int start_pos = (expr_copy[0] == '+' || expr_copy[0] == '-') ? 1 : 0;

    int len = strlen(expr_copy);
    for (int i = start_pos; i < len; i++)
    {
        if (is_valid_operator(expr_copy[i]))
        {
            op_pos = i;
            break;
        }
    }

    if (op_pos == -1)
    {
        free(expr_copy);
        return CALC_INVALID_INPUT;
    }

    // Extract operands
    char op = expr_copy[op_pos];
    expr_copy[op_pos] = '\0'; // Split string at operator
    char *num1_str = expr_copy;
    char *num2_str = expr_copy + op_pos + 1;

    // Convert operands
    double num1, num2;
    if (!string_to_double(num1_str, &num1))
    {
        // Append num1_str to the error message
        snprintf(error_msg,100, "Invalid number format: %s", num1_str);
        free(expr_copy);
        return CALC_INVALID_INPUT;
    }
    if (!string_to_double(num2_str, &num2))
    {
        snprintf(error_msg,100, "Invalid number format: %s", num2_str);
        free(expr_copy);
        return CALC_INVALID_INPUT;
    }

    // Perform operation
    CalcResult error = CALC_SUCCESS;
    switch (op)
    {
    case '+':
        *result = add(num1, num2);
        break;
    case '-':
        *result = subtract(num1, num2);
        break;
    case '*':
        *result = multiply(num1, num2);
        break;
    case '/':
        *result = divide(num1, num2, &error);
        break;
    case '^':
        *result = power(num1, num2, &error);
        break;
    default:
        error = CALC_INVALID_INPUT;
    }

    free(expr_copy);
    return error;
}

// Input validation functions
int is_valid_number(const char *str)
{
    if (str == NULL || strlen(str) == 0)
        return 0;

    int i = 0;
    int has_decimal = 0;

    // Check for sign
    if (str[0] == '-' || str[0] == '+')
    {
        i = 1;
        if (strlen(str) == 1)
            return 0; // Just a sign
    }

    int has_digit = 0;
    for (; i < (int)strlen(str); i++)
    {
        if (str[i] == '.')
        {
            if (has_decimal)
                return 0; // Multiple decimals
            has_decimal = 1;
        }
        else if (isdigit(str[i]))
        {
            has_digit = 1;
        }
        else
        {
            return 0; // Invalid character
        }
    }

    return has_digit;
}

int is_valid_operator(char op)
{
    return (op == '+' || op == '-' || op == '*' || op == '/' ||
            op == '^');
}
