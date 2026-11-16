# Basic Calculator

## Overview
This is a simple command-line calculator written in C. It performs basic arithmetic operations such as addition, subtraction, multiplication, division and exponent. The calculator also keeps track of your history and allows you replay a past calculation.

## Prerequisites
To build and run the calculator, you need:
- A C compiler (e.g., `gcc`)
- GNU Make
- A Unix-like environment (Linux, macOS, or WSL on Windows)


## Project design and decisions
This section explains the structure, key design decisions, and expected behaviors.

Files and responsibilities
- `main.c` — CLI parsing and interactive loop. Reads user input, calls `calculator` functions, and records results using the history module.
- `calculator.c` / `calculator.h` — Core arithmetic operations. Each operation is implemented as a function that takes numeric inputs and returns a result. Division returns an error code or uses a defined behavior for divide-by-zero cases.
- `history.c` / `history.h` — Simple CSV-backed history. Provides functions to append an operation to `history.csv` and to read recent entries. It also allows users to save history is a custom file and replay past calculations.
- `utils.c` / `utils.h` — Helper functions for parsing, input validation, and small utilities shared across modules.
- `test.c` — Test cases using the included `minunit` framework. Unit tests target `calculator`, `history` and `utils` functions.

Design choices
- Single-responsibility modules: each .c/.h pair has a small, testable responsibility which simplifies unit testing and maintenance.
- CSV history: chosen for simplicity and human-readability. The format is a single line per operation: timestamp,expression,result,is_error.
- Error handling: functions return error codes for exceptional cases (e.g., divide-by-zero). Tests assert correct error codes and program behavior on invalid input.
- Minimal dependencies: no external libraries besides the small `minunit` test runner included in the repo.

Edge cases considered
- Division by zero — calculator returns an error code and the CLI prints a helpful message.
- Invalid numeric input — parsed by `utils` with validation; user is re-prompted on invalid input.
- File write errors (history CSV) — functions return an error code which the CLI surfaces to the user.

Testing notes
- Unit tests exercise each calculator operation with normal and boundary inputs, plus tests for parsing/validation helpers.


## Building the Project
Follow these steps to build the calculator:

1. **Clone or download the project** to your local machine.
2. **Navigate to the project directory**:
   ```bash
   cd path/to/project
   ```
3. **Compile the main program**:
   Run the following command to build the calculator application:
   ```bash
   make
   ```
   This will generate an executable named `app` in the project directory.

4. **Compile the tests**:
   - To build the unit tests:
     ```bash
     make test
     ```
     This generates an executable named `test`.

   - To build the memory tests:
     ```bash
     make memtest
     ```
     This generates an executable named `memapp`.

## Running the Program
1. **Run the calculator**:
   After building, execute the calculator with:
   ```bash
   ./app
   ```
   Follow the on-screen prompts to perform arithmetic operations.

2. **Run the tests**:
   - To run unit tests:
     ```bash
     ./test
     ```

## Cleaning Up
To remove the compiled executables (`app`, `unit_test`, `int_test`), run:
```bash
make clean
```
