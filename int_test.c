#include <stdio.h>
#include <stdlib.h>
#include "bcal.h"


int main() {
    // Define test inputs for various scenarios
    char *test1_inputs[] = {
        "5 + 3",
        "10 - 4",
        "7 * 2",
        "20 / 4",
        "3 ^ 2",
    };
    
    // Calculate the number of elements in the array.
    int num_inputs = sizeof(test1_inputs) / sizeof(test1_inputs[0]);
    
    printf("Test 1: Basic arithmetic\n");
    // Loop through each string in the array.
    for (int i = 0; i < num_inputs; i++) {
        double first, second;
        char operator;
        
        printf(">>> %s\n", test1_inputs[i]);
        if (validate_input(test1_inputs[i], &first, &operator, &second) == 0) {
            char *result = perform_calculation(first, operator, second);
            printf("%s\n", result);
            free(result);
        }
    }

    // Define test inputs for zero handling
    char *test2_inputs[] = {
        "0 + 5",
        "5 - 0",
        "0 * 7",
        "0 / 5"
    };
    
    // Calculate the number of elements in the array.
    num_inputs = sizeof(test2_inputs) / sizeof(test2_inputs[0]);
    
    printf("\n\nTest 2: Zero handling\n");
    // Loop through each string in the array.
    for (int i = 0; i < num_inputs; i++) {
        double first, second;
        char operator;

        printf(">>> %s\n", test2_inputs[i]);
        if (validate_input(test2_inputs[i], &first, &operator, &second) == 0) {
            char *result = perform_calculation(first, operator, second);
            printf("%s\n", result);
            free(result);
        }
    }

    // Define test inputs for error conditions
    char *test3_inputs[] = {
        "5 / 0",
        "abc + 5",
        "5 & 3",
        "2 ^ -1"
    };

    // Calculate the number of elements in the array.
    num_inputs = sizeof(test3_inputs) / sizeof(test3_inputs[0]);

    printf("\n\nTest 3: Error conditions\n");
    // Loop through each string in the array.
    for (int i = 0; i < num_inputs; i++) {
        double first, second;
        char operator;

        printf(">>> %s\n", test3_inputs[i]);
        if (validate_input(test3_inputs[i], &first, &operator, &second) == 0) {
            char *result = perform_calculation(first, operator, second);
            printf("%s\n", result);
            free(result);
        }
    }

    // Define test inputs for large numbers
    char *test4_inputs[] = {
        "999 * 999",
        "1000000 / 1000"
    };

    // Calculate the number of elements in the array.
    num_inputs = sizeof(test4_inputs) / sizeof(test4_inputs[0]);

    printf("\n\nTest 4: Large numbers\n");
    // Loop through each string in the array.
    for (int i = 0; i < num_inputs; i++) {
        double first, second;
        char operator;

        printf(">>> %s\n", test4_inputs[i]);
        if (validate_input(test4_inputs[i], &first, &operator, &second) == 0) {
            char *result = perform_calculation(first, operator, second);
            printf("%s\n", result);
            free(result);
        }
    }

    return 0;
}
