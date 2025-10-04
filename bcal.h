#ifndef BCAL_H
#define BCAL_H

// Function prototypes
void display_welcome (void);

void display_help (void);

double get_number (char *prompt, int *start_idx, int len);

char get_operator (char *prompt, int *start_idx, int len);

char* perform_calculation (double num1 , char op , double num2);

double multiply(double a, double b);

double divide(double a, double b);

double power(double base , int exponent); /*only exponent > 0*/

int validate_input (char* input, double *first, char *operator, double *second);

void trim_newline(char *input);

void clear_input_buffer (void);

#endif