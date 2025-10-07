#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "history.h"

HistoryResult init_history(CalculationHistory *hist)
{
    if (hist == NULL)
        return HISTORY_MEMORY_ERROR; /* Use error code */

    // Start with initial capacity
    hist->capacity = INITIAL_HISTORY_CAPACITY;
    hist->count = 0;

    // Allocate memory for the array
    hist->calculations = malloc(hist->capacity * sizeof(Calculation));
    if (hist->calculations == NULL)
    {
        return HISTORY_MEMORY_ERROR;
    }

    return HISTORY_SUCCESS;
}

HistoryResult add_calculation(CalculationHistory *hist, const char *expr,
                              const char *result, int is_error)
{
    if (hist == NULL || expr == NULL)
        return HISTORY_MEMORY_ERROR;

    // Check if we need to grow the array
    if (hist->count >= hist->capacity)
    {
        int new_capacity = hist->capacity * 2; // Double the size

        // Attempt to reallocate memory
        Calculation *temp = realloc(hist->calculations,
                                    new_capacity * sizeof(Calculation));
        if (temp == NULL)
        {
            fprintf(stderr, "Error : Unable to expand history \n");
            return HISTORY_MEMORY_ERROR;
        }

        // Success - update our structure
        hist->calculations = temp;
        hist->capacity = new_capacity;
        printf("History capacity expanded to %d entries \n", new_capacity);
    }

    // Now we have space - add the calculation
    Calculation *calc = &hist->calculations[hist->count];

    // Allocate memory for expression string
    calc->expression_str = malloc(strlen(expr) + 1);
    if (calc->expression_str == NULL)
    {
        return HISTORY_MEMORY_ERROR;
    }
    strcpy(calc->expression_str, expr);

    // Allocate memory for result string
    calc->result = malloc(strlen(result) + 1);
    if (calc->result == NULL)
    {
        free(calc->expression_str); // free previously allocated memory
        return HISTORY_MEMORY_ERROR;
    }
    strcpy(calc->result, result);

    // Set other fields
    calc->timestamp = time(NULL);
    calc->is_error = is_error;
    hist->count++;
    return HISTORY_SUCCESS;
}

HistoryResult load_history_from_file(CalculationHistory *hist,
                                     const char *filename)
{
    if (hist == NULL || filename == NULL)
    {
        return HISTORY_FILE_ERROR;
    }
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("Cannot open file '%s' for reading.\n", filename);
        return HISTORY_SUCCESS; // Not an error - file may not exist yet
    }
    char line[512]; // Buffer for reading lines
    int line_count = 0;
    int loaded_count = 0;
    // Skip header line
    if (fgets(line, sizeof(line), file) == NULL)
    {
        fclose(file);
        return HISTORY_FILE_ERROR;
    }
    // Read each data line
    while (fgets(line, sizeof(line), file) != NULL)
    {
        line_count++;
        // Remove newline character
        line[strcspn(line, "\n")] = '\0';
        if (strlen(line) == 0)
            continue; // Skip empty lines
        // Parse this line into a temporary Calculation
        Calculation temp_calc;
        if (parse_csv_line(line, &temp_calc) == HISTORY_SUCCESS)
        {
            // Add to history (this handles memory allocation )
            if (add_calculation(hist, temp_calc.expression_str,
                                temp_calc.result, temp_calc.is_error) == HISTORY_SUCCESS)
            {
                // Update timestamp ( add_calculation sets current time)
                hist->calculations[hist->count - 1].timestamp = temp_calc.timestamp;
                loaded_count++;
            }
            // Free temporary string
            free(temp_calc.expression_str);
            free(temp_calc.result);
        }
        else
        {
            fprintf(stderr, "Warning : Could not parse line %d in %s\n",
                    line_count, filename);
        }
    }
    fclose(file);
    printf("Loaded %d calculations from %s\n", loaded_count,
           filename);
    return HISTORY_SUCCESS;
}

void display_history(const CalculationHistory *hist)
{
    printf("Calculation History (%d entries):\n", hist->count);
    if (hist->count == 0)
    {
        printf("History is empty.\n");
        return;
    }
    for (int i = 0; i < hist->count; i++)
    {
        const Calculation *calc = &hist->calculations[i];
        char time_str[20];
        format_timestamp(calc->timestamp, time_str, sizeof(time_str));
        if (calc->is_error)
        {
            printf("[%d] %s = ERROR: %s (%s)\n", i + 1,
                   calc->expression_str, calc->result, time_str);
        }
        else
        {
            printf("[%d] %s = %s (%s)\n", i + 1, calc->expression_str, calc->result, time_str);
        }
    }
}

void display_history_entry(const CalculationHistory *hist, int index)
{
    const Calculation *calc = &hist->calculations[index];
    // Simulate replaying the calculation
    printf("Replaying calculation [%d]:\n", index + 1);
    char time_str[20];
    format_timestamp(calc->timestamp, time_str, sizeof(time_str));
    if (calc->is_error)
    {
        printf("[%d] %s = ERROR: %s (%s)\n", index + 1,
               calc->expression_str, calc->result, time_str);
    }
    else
    {
        printf("[%d] %s = %s (%s)\n", index + 1, calc->expression_str, calc->result, time_str);
    }
}

HistoryResult clear_history(CalculationHistory *hist)
{
    if (hist == NULL)
        return HISTORY_MEMORY_ERROR;
    cleanup_history(hist);
    hist->count = 0;
    hist->capacity = INITIAL_HISTORY_CAPACITY;
    hist->calculations = malloc(hist->capacity * sizeof(Calculation));
    return HISTORY_SUCCESS;
}

HistoryResult replay_calculation(const CalculationHistory *hist, int index, char *result)
{
    if (hist == NULL || index < 0 || index >= hist->count || result == NULL)
        return HISTORY_MEMORY_ERROR;

    const Calculation *calc = &hist->calculations[index];
    strcpy(result, calc->result);
    return HISTORY_SUCCESS;
}

void cleanup_history(CalculationHistory *hist)
{
    if (hist == NULL)
        return;
    if (hist->calculations != NULL)
    {
        // Free each expression string first
        for (int i = 0; i < hist->count; i++)
        {
            if (hist->calculations[i].expression_str != NULL)
            {
                free(hist->calculations[i].expression_str);
                hist->calculations[i].expression_str = NULL;
            }
            if (hist->calculations[i].result)
            {
                free(hist->calculations[i].result);
                hist->calculations[i].result = NULL;
            }
        }
        // Free the array itself
        free(hist->calculations);
        hist->calculations = NULL;
    }
    // Reset structure
    hist->count = 0;
    hist->capacity = 0;
}

HistoryResult save_history_to_file(const CalculationHistory *hist,
                                   const char *filename)
{
    if (hist == NULL || filename == NULL)
        return HISTORY_FILE_ERROR;
    FILE *file = fopen(filename, "w"); // Open file for writing
    if (file == NULL)
    {
        fprintf(stderr, "Error : Cannot open file '%s' for writing \n",
                filename);
        return HISTORY_FILE_ERROR;
    }
    // Write CSV header
    fprintf(file, "Timestamp ,Expression ,Result ,Error \n");
    for (int i = 0; i < hist->count; i++)
    {
        const Calculation *calc = &hist->calculations[i];
        // Write in CSV format
        fprintf(file, "%ld,\"%s\",\"%s\",%d\n",
                (long)calc->timestamp,
                calc->expression_str,
                calc->result,
                calc->is_error);
    }
    fclose(file);
    return HISTORY_SUCCESS;
}

HistoryResult parse_csv_line(const char *line, Calculation *calc)
{
    if (line == NULL || calc == NULL)
        return HISTORY_MEMORY_ERROR;
    // Create a working copy of the line ( strtok modifies the string)
    char *line_copy = malloc(strlen(line) + 1);
    if (line_copy == NULL)
        return HISTORY_MEMORY_ERROR;
    strcpy(line_copy, line);

    // Parse timestamp
    char *token = strtok(line_copy, ",");
    if (token == NULL)
    {
        free(line_copy);
        return HISTORY_FILE_ERROR;
    }
    calc->timestamp = (time_t)atol(token);

    // Parse expression ( remove quotes )
    token = strtok(NULL, ",");
    if (token == NULL)
    {
        free(line_copy);
        return HISTORY_FILE_ERROR;
    }

    // Remove quotes from expression
    if (token[0] == '"')
    {
        token++; // Skip opening quote
        int len = strlen(token);
        if (len > 0 && token[len - 1] == '"')
        {
            token[len - 1] = '\0'; // Remove closing quote
        }
    }

    // Allocate memory for expression
    calc->expression_str = malloc(strlen(token) + 1);
    if (calc->expression_str == NULL)
    {
        free(line_copy);
        return HISTORY_MEMORY_ERROR;
    }
    strcpy(calc->expression_str, token);

    // Parse result
    token = strtok(NULL, ",");
    if (token == NULL)
    {
        free(line_copy);
        return HISTORY_FILE_ERROR;
    }

    // Remove quotes from expression
    if (token[0] == '"')
    {
        token++; // Skip opening quote
        int len = strlen(token);
        if (len > 0 && token[len - 1] == '"')
        {
            token[len - 1] = '\0'; // Remove closing quote
        }
    }

    // Allocate memory for result
    calc->result = malloc(strlen(token) + 1);
    if (calc->result == NULL)
    {
        free(calc->expression_str);
        free(line_copy);
        return HISTORY_MEMORY_ERROR;
    }
    strcpy(calc->result, token);

    // Parse error flag
    token = strtok(NULL, ",");
    if (token == NULL)
    {
        free(calc->expression_str);
        free(line_copy);
        return HISTORY_FILE_ERROR;
    }
    calc->is_error = atoi(token);

    free(line_copy);
    return HISTORY_SUCCESS;
}

void format_timestamp(time_t timestamp, char *buffer, size_t buffer_size)
{
    struct tm *tm_info = localtime(&timestamp);
    strftime(buffer, buffer_size, "%Y-%m-%d %H:%M:%S", tm_info);
}

int get_history_count(const CalculationHistory *hist)
{
    if (hist == NULL)
        return 0;
    return hist->count;
}