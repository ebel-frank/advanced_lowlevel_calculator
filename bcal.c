#include "bcal.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// ANSI color codes for terminal output
#define KRED "\x1B[31m"
#define KNRM "\x1B[0m"

// Function to display welcome message
void display_welcome(void)
{
    printf("Command-Line Calculator - Part 1\n");
    printf("%sType %s'help'%s for instructions, %s'Q'%s to quit.\n", KNRM, KRED, KNRM, KRED, KNRM);
}

// Function to display help information
void display_help(void)
{
    printf("\nCommand-Line Calculator Help\n");
    printf("===========================\n");
    printf("Supported Operations :\n");
    printf("+ : Addition\n");
    printf("- : Subtraction\n");
    printf("* : Multiplication\n");
    printf("/ : Division\n");
    printf("^ : Exponentiation\n\n");

    printf("Usage: number operator number\n");
    printf("Example: 5 + 3\n\n");

    printf("Special Commands :\n");
    printf("help : Show this help message\n");
    printf("Q : Quit calculator\n\n");

    printf("Note: Only integers and simple decimals supported .\n");
    printf("Negative exponents not supported .\n");
}

// Function to parse and return a number from the input string
double get_number(char *prompt, int *start_idx, int len)
{
    // Skip leading whitespace
    while (*start_idx < len && isspace(prompt[*start_idx])) {
        (*start_idx)++;
    }
    
    char num1[32] = {};
    int num_idx = 0;
    // Check for an optional minus sign
    if (*start_idx < len && (prompt[*start_idx] == '-' || prompt[*start_idx] == '+')) {
        num1[num_idx++] = prompt[*start_idx];
        (*start_idx)++;
    }
    // num1[num_idx] = '\0';

    // Check if the current character is a digit
    if (*start_idx >= len || !isdigit(prompt[*start_idx])) {
        return -1; // Indicate error
    }

    // Loop through and parse the digits to build the number
    while (*start_idx < len && (isdigit(prompt[*start_idx]) || prompt[*start_idx] == '.')) {
        num1[num_idx++] = prompt[*start_idx];
        (*start_idx)++;
    }

    return atof(num1);
}

// Function to get the operator from the input string
char get_operator(char *prompt, int *start_idx, int len)
{
    // Skip leading whitespace
    while (*start_idx < len && isspace(prompt[*start_idx])) {
        (*start_idx)++;
    }

    // Check if the current character is a valid operator
    if (*start_idx < len && (prompt[*start_idx] == '+' || prompt[*start_idx] == '-' || prompt[*start_idx] == '*' || prompt[*start_idx] == '/' || prompt[*start_idx] == '^')) {
        char operator = prompt[*start_idx];
        (*start_idx)++;
        return operator;
    } else {
        return '\0'; // Indicate error
    }
}

// Function to perform calculation based on operator
char *perform_calculation(double num1, char op, double num2)
{
    char *result = (char *)malloc(13 * sizeof(char));
    switch (op)
    {
    case '+':
        sprintf(result, "= %lf", num1 + num2);
        break;
    case '-':
        sprintf(result, "= %lf", num1 - num2);
        break;
    case '*':
        sprintf(result, "= %lf", multiply(num1, num2));
        break;
    case '/':
        if (num2 == 0)
        {
            sprintf(result, "Error: Division by zero!");
        }
        else
        {
            sprintf(result, "= %f", divide(num1, num2));
        }
        break;
    case '^':
        if (num2 < 0)
        {
            sprintf(result, "Error: Negative exponent not supported");
        }
        else
        {
            sprintf(result, "= %f", power(num1, num2));
        }
        break;
    default:
        sprintf(result, "Error: Invalid operator %s'%c'%s", KRED, op, KNRM);
    }
    return result;
}

// Function to perform multiplication operation
double multiply(double a, double b)
{
    int result = 0;
    short is_negative = 0;
    if ((a < 0 && b > 0) || (a > 0 && b < 0))
    {
        is_negative = 1;
    }
    // Resolve both numbers to there absolute form
    a = abs(a);
    b = abs(b);
    // loop using the smallest number while adding the biggest number to result
    for (int i = 0; i < b; i++)
    {
        result = result + a;
    }
    if (is_negative)
        return -result;
    return result;
}

// Function to perform division operation
double divide(double a, double b)
{
    // Handle division by zero
    if (b == 0)
    {
        return 0.0;
    }
    double result = 0;
    short is_negative = 0;
    if ((a < 0 && b > 0) || (a > 0 && b < 0))
        is_negative = 1;
    // Resolve both numbers to there absolute form
    a = abs(a);
    b = abs(b);
    int int_part = 0;
    // Compute integer part by repeated subtraction
    while (a >= b)
    {
        a -= b;
        int_part += 1;
    }
    // Compute fractional part (long division style)
    float frac_part = 0.0;
    float place = 0.1;
    int max_digits = 10;
    double tolerance = 1e-12;
    for (int i = 1; i < max_digits && a > tolerance; i++)
    {
        a *= 10;
        int count = 0;
        while (a >= b)
        {
            a -= b;
            count += 1;
        }
        frac_part += count * place;
        place /= 10;
    }
    // Combine integer and fractional parts
    result = int_part + frac_part;
    if (is_negative)
        return -result;
    return result;
}

// Function to compute power (exponentiation)
double power(double base, int exponent)
{
    if (exponent < 0)
        return -1.0; // Indicate error for negative exponent

    exponent = abs(exponent);
    double result = 1;
    for (int i = 0; i < exponent; i++)
    {
        result = multiply(result, base);
    }
    return result;
}

// Function to validate and parse input expression
int validate_input(char *input, double *first, char *operator, double *second)
{
    int len = strlen(input);
    int i = 0;

    // Parse the first number, operator, and second number
    double temp_first = get_number(input, &i, len);
    char temp_op = get_operator(input, &i, len);
    double temp_second = get_number(input, &i, len);

    if (temp_first == -1 || temp_op == '\0' || temp_second == -1) {
        printf("Error: Invalid expression\n");
        return -1; // Indicate error
    }
    // Skip any trailing whitespace
    while (i < len && isspace(input[i])) {
        i++;
    }

    // Check for any extra characters at the end of the input string
    if (i < len) {
        printf("Error: Invalid expression\n");
        return -1; // Indicate error
    }

    *first = temp_first;
    *operator = temp_op;
    *second = temp_second;

    return 0;
}

// Function to trim trailing newline from fgets() input
void trim_newline(char *str)
{
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n')
    {
        str[len - 1] = '\0';
    }
}

void clear_input_buffer(void)
{
}
