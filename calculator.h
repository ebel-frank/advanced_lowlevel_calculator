#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <time.h>

// Structure to represent a single calculation
typedef struct
{
    char *expression_str; // Store original user input
    char *result;         // Store the output
    time_t timestamp;     // Time when calculation was performed
    int is_error;         // status flag: 0 for success , 1 for error
} Calculation;

// Error codes for calculator operations
typedef enum
{
    CALC_SUCCESS = 0,
    CALC_DIVISION_BY_ZERO = -1,
    CALC_INVALID_INPUT = -2,
    CALC_OVERFLOW = -3
} CalcResult;

// Public arithmetic functions
double add(double a, double b);
double subtract(double a, double b);
double multiply(double a, double b);
double divide(double a, double b, CalcResult *error);
double power(double base, int exponent, CalcResult *error);

// Expression parsing
CalcResult parse_expression(const char *input, double *result, char *error_msg);

// Input validation
int is_valid_number(const char *str);
int is_valid_operator(char op);

#endif // CALCULATOR_H