# Basic Calculator

## Overview
This is a simple command-line calculator written in C. It performs basic arithmetic operations such as addition, subtraction, multiplication, and division. The project includes a main application, unit tests, and integration tests to ensure functionality.

## Prerequisites
To build and run the calculator, you need:
- A C compiler (e.g., `gcc`)
- GNU Make
- A Unix-like environment (Linux, macOS, or WSL on Windows)

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
     make unit_test
     ```
     This generates an executable named `unit_test`.
   - To build the integration tests:
     ```bash
     make int_test
     ```
     This generates an executable named `int_test`.

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
     ./unit_test
     ```
   - To run integration tests:
     ```bash
     ./int_test
     ```

## Cleaning Up
To remove the compiled executables (`app`, `unit_test`, `int_test`), run:
```bash
make clean
```
