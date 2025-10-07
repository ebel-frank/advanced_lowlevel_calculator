#ifndef UTILS_H
#define UTILS_H

// String utilities
char *safe_string_copy(const char *source);
void remove_spaces(char *str);
void trim_whitespace(char *str);
int string_to_double(const char *str, double *result);

// Input utilities
void clear_input_buffer(void);
int get_user_input(char *buffer, size_t buffer_size);
int get_integer_input(const char *prompt, int *result);

// Memory utilities
void *safe_malloc(size_t size);
void *safe_realloc(void *ptr, size_t new_size);

// Error handling utilities
void print_error(const char *message);
void print_warning(const char *message);

#endif // UTILS_H