#ifndef HISTORY_H
#define HISTORY_H

#include "calculator.h"

#define INITIAL_HISTORY_CAPACITY 5
#define MAX_EXPRESSION_LENGTH 256
#define DEFAULT_HISTORY_FILE "history.csv"

typedef enum
{
    HISTORY_SUCCESS = 0,
    HISTORY_MEMORY_ERROR = -1,
    HISTORY_FILE_ERROR = -2,
    HISTORY_INVALID_INDEX = -3
} HistoryResult;

// Structure to manage the dynamic history array
typedef struct
{
    Calculation *calculations; // Dynamic array
    int count;                 // Current number of entries
    int capacity;              // Current allocated capacity
} CalculationHistory;

// Core history management functions
HistoryResult init_history(CalculationHistory *hist);
HistoryResult add_calculation(CalculationHistory *hist, const char *expr,
                              const char *result, int is_error);
void cleanup_history(CalculationHistory *hist);
HistoryResult parse_csv_line(const char *line, Calculation *calc);

// Display functions
void display_history(const CalculationHistory *hist);
void display_history_entry(const CalculationHistory *hist, int index);

// File operations
HistoryResult save_history_to_file(const CalculationHistory *hist, const char *filename);
HistoryResult load_history_from_file(CalculationHistory *hist, const char *filename);

// History management commands
HistoryResult clear_history(CalculationHistory *hist);
HistoryResult replay_calculation(const CalculationHistory *hist, int index, char *result);

// Utility functions
void format_timestamp(time_t timestamp, char *buffer, size_t buffer_size);
int get_history_count(const CalculationHistory *hist);

#endif // HISTORY_H