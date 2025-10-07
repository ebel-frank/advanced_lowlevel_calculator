#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "calculator.h"
#include "history.h"
#include "utils.h"

#define BUFFER_SIZE 512

// ANSI color codes for terminal output
#define KRED "\x1B[31m"
#define KNRM "\x1B[0m"

// Function prototypes for command handling
static void display_welcome(void);
static void display_help(void);
static int handle_command(CalculationHistory *hist, const char *input);
static int handle_expression(CalculationHistory *hist, const char *input);

int main()
{
    CalculationHistory history;
    char input[BUFFER_SIZE];

    // Initialize history
    if (init_history(&history) != HISTORY_SUCCESS)
    {
        print_error("Failed to initialize history");
        return 1;
    }
    // Display welcome message
    display_welcome();

    // Load previous history
    printf("Loading previous history ...\n");
    load_history_from_file(&history, DEFAULT_HISTORY_FILE);

    // Main program loop
    while (1)
    {
        printf(">>> ");

        // Get user input
        if (fgets(input, sizeof(input), stdin) == NULL)
        {
            break;
        }

        // Remove newline
        input[strcspn(input, "\n")] = '\0';

        // Check for empty input
        if (strlen(input) == 0)
        {
            print_error("Empty input ");
            continue;
        }

        // Handle quit command
        if (strcmp(input, "Q") == 0 || strcmp(input, "q") == 0)
        {
            printf("Saving history ...\n");
            if (save_history_to_file(&history, DEFAULT_HISTORY_FILE) == HISTORY_SUCCESS)
            {
                printf("History saved successfully .\n");
            }
            printf("Goodbye !\n");
            break;
        }

        // Try to handle as command first , then as expression
        if (!handle_command(&history, input))
        {
            handle_expression(&history, input);
        }
    }

    // Cleanup
    cleanup_history(&history);
    return 0;
}

static void display_welcome(void)
{
    printf("Command -Line Calculator - Part 2\n");
    printf("%sType %s'help'%s for instructions, %s'Q'%s to quit.\n\n", KNRM, KRED, KNRM, KRED, KNRM);
}

static void display_help(void)
{
    printf("\nCommand -Line Calculator Help\n");
    printf(" ============================\n");
    printf(" Supported Operations :\n");
    printf(" + : Addition \n");
    printf(" - : Subtraction \n");
    printf(" * : Multiplication \n");
    printf(" / : Division \n");
    printf(" ^ : Exponentiation \n\n");

    printf(" History Commands :\n");
    printf(" history : Show calculation history \n");
    printf(" clear : Clear current session history \n");
    printf(" save [file] : Save history to file\n");
    printf(" load [file] : Load history from file\n");
    printf(" replay N : Replay calculation number N\n\n");

    printf(" Special Commands :\n");
    printf(" help : Show this help message \n");
    printf(" Q : Save and quit calculator \n\n");

    printf(" Usage: number operator number \n");
    printf(" Examples : 5 + 3, 10-4, 7*2 , 20/4 , 2^3\n\n");
}

static int handle_command(CalculationHistory *hist, const char *input)
{
    // Handle help command
    if (strcmp(input, "help") == 0)
    {
        display_help();
        return 1;
    }

    // Handle history command
    if (strcmp(input, "history") == 0)
    {
        display_history(hist);
        return 1;
    }

    // Handle clear command
    if (strcmp(input, "clear") == 0)
    {
        if (clear_history(hist) == HISTORY_SUCCESS)
        {
            printf("History cleared for current session.\n");
        }
        return 1;
    }

    // Handle save command
    if (strncmp(input, "save", 4) == 0)
    {
        const char *filename = DEFAULT_HISTORY_FILE;

        // Check if filename specified
        if (strlen(input) > 5)
        {
            filename = input + 5; // Skip "save "
        }

        if (save_history_to_file(hist, filename) == HISTORY_SUCCESS)
        {
            printf("History saved to %s (%d entries )\n", filename,
                   get_history_count(hist));
        }
        else
        {
            printf("Error: Unable to save to %s\n", filename);
        }
        return 1;
    }

    // Handle load command
    if (strncmp(input, "load", 4) == 0)
    {
        const char *filename = DEFAULT_HISTORY_FILE;

        // Check if filename specified
        if (strlen(input) > 5)
        {
            filename = input + 5; // Skip "load "
        }

        load_history_from_file(hist, filename);
        return 1;
    }

    // Handle replay command
    if (strncmp(input, "replay", 6) == 0)
    {

        if (!is_valid_number(input + 7))
        {
            print_error("Error: Usage : replay N ( where N is calculation number )");
            return 1;
        }
        int index = atoi(input + 7) - 1; // Convert to 0- based index
        char result[50];

        HistoryResult res = replay_calculation(hist, index, result);
        if (res == HISTORY_SUCCESS)
        {
            display_history_entry(hist, index);
            printf("= %s\n", result);
        }
        else
        {
            printf("Error: Invalid history index . Only %d calculations available.\n",
                   get_history_count(hist));
        }
        return 1;
    }

    return 0; // Not a recognized command
}

static int handle_expression(CalculationHistory *hist, const char *input)
{
    double result;
    char error_msg[100] = "";
    CalcResult calc_result = parse_expression(input, &result, error_msg);

    if (calc_result == CALC_SUCCESS)
    {
        char output[50];
        snprintf(output, 50, "%f", result);
        printf("= %s\n", output);
        add_calculation(hist, input, output, 0);
    }
    else
    {
        // Handle different error types
        switch (calc_result)
        {
        case CALC_DIVISION_BY_ZERO:
            strcpy(error_msg, "Division by zero!");
            break;
        case CALC_INVALID_INPUT:
            if (strlen(error_msg) == 0)
            {
                strcpy(error_msg, "Invalid input or expression ");
            }
            break;
        case CALC_OVERFLOW:
            strcpy(error_msg, "Numerical overflow");
            break;
        default:
            strcpy(error_msg, "Unknown calculation error ");
        }

        printf("Error: %s\n", error_msg);
        add_calculation(hist, input, error_msg, 1);
    }

    return 1;
}