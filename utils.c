#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

// Safe string duplication
char *safe_string_copy(const char *source)
{
    if (source == NULL)
        return NULL;

    size_t len = strlen(source) + 1;
    char *copy = malloc(len);
    if (copy == NULL)
    {
        print_error("Memory allocation failed for string copy");
        return NULL;
    }
    strcpy(copy, source);
    return copy;
}

// Safe memory allocation with error handling
void *safe_malloc(size_t size)
{
    void *ptr = malloc(size);
    if (ptr == NULL && size > 0)
    {
        print_error("Memory allocation failed ");
    }
    return ptr;
}

// Safe reallocation
void *safe_realloc(void *ptr, size_t new_size)
{
    void *new_ptr = realloc(ptr, new_size);
    if (new_ptr == NULL && new_size > 0)
    {
        print_error("Memory reallocation failed ");
        // Original pointer is still valid
    }
    return new_ptr;
}

// Remove spaces and tabs from string
void remove_spaces(char *str)
{
    if (str == NULL)
        return;

    int i, j = 0;
    for (i = 0; str[i] != '\0'; i++)
    {
        if (str[i] != ' ' && str[i] != '\t')
        {
            str[j++] = str[i];
        }
    }
    str[j] = '\0';
}

// Convert string to double with error checking
int string_to_double(const char *str, double *result)
{
    if (str == NULL || result == NULL)
        return 0;

    char *endptr;
    *result = strtod(str, &endptr);

    // Check if conversion was successful
    return (*endptr == '\0' && endptr != str);
}
// Error and warning printing functions
void print_error(const char *message)
{
    if (message != NULL)
    {
        fprintf(stderr, "Error : %s\n", message);
    }
}

void print_warning(const char *message)
{
    if (message != NULL)
    {
        fprintf(stderr, "Warning : %s\n", message);
    }
}

// Clear input buffer
void clear_input_buffer(void)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
    {
        // Clear buffer
    }
}