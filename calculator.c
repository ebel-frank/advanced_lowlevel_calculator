#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "bcal.h"

#define BUFFER_SIZE 256


int main()
{
    // Display welcome message
    display_welcome();

    // Variables to hold user input and parsed values
    double first, second;
    char operator;
    char buffer[BUFFER_SIZE];

    while (1) // Infinite loop
    {
        printf("\n");
        printf(">>> ");

        int i = 0;

        // Get user input
        if (fgets(buffer, sizeof(buffer), stdin) == NULL)
        {
            printf("Error reading input.\n");
        }
        trim_newline(buffer);

        if (strcmp(buffer, "") == 0) // Check if the input is empty
        {
            printf("Error: Empty input");
        }
        else if (strcmp(buffer, "q") == 0 || strcmp(buffer, "Q") == 0)
        {
            printf("Goodbye!");
            break;
        }
        else if (strcmp(buffer, "help") == 0)
        {
            display_help();
        }
        else if (validate_input(buffer, &first, &operator, &second) == 0) {
            // Input is valid, perform calculation
            char *result = perform_calculation(first, operator, second);
            printf("%s\n", result);
            free(result); 
        }
    }
    return 0;
}