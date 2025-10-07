# Basic Calculator

## Overview
This is a small command-line calculator written in C. It performs basic arithmetic operations (addition, subtraction, multiplication, division) and includes a simple command-line UI, unit tests, and an integration test harness. The code is intentionally split into small modules to make testing and reasoning easier.

## Quick summary
- Source files: `main.c`, `calculator.c`, `history.c`, `utils.c` and headers `calculator.h`, `history.h`, `utils.h`.
- Tests: `unit_test.c`, `int_test.c` (with the bundled `munit` test runner in `munit/`).
- Output binaries (when built): `app.exe` (Windows) or `app` (Unix), `unit_test`, `int_test`.

## Prerequisites
- C compiler (tested with `gcc` / MinGW or `clang`)
- GNU Make
- On Windows: either use WSL or MinGW/MinGW-w64/MinGW32 or a developer shell that provides `make`. The project already produces `app.exe` when compiled on Windows in this workspace.

## Build (platform notes)
The project includes a `Makefile` with targets for the main program and tests. From the project root run:

On Unix / WSL / macOS:

```powershell
make
```

On Windows using PowerShell with a working `make` (MinGW or similar):

```powershell
make
```

This builds the main executable (named `app` or `app.exe` depending on your toolchain) and places it in the project root.

Useful Make targets:
- `make` or `make all` — build the main app.
- `make unit_test` — build the unit tests (executable `unit_test`).
- `make int_test` — build the integration test harness (executable `int_test`).
- `make clean` — remove built artifacts.

If your environment does not provide `make`, you can compile manually with `gcc`:

```powershell
gcc -Wall -Wextra -O2 -o app.exe main.c calculator.c history.c utils.c
gcc -Wall -Wextra -O2 -o unit_test.exe unit_test.c calculator.c history.c utils.c munit/munit.c
gcc -Wall -Wextra -O2 -o int_test.exe int_test.c calculator.c history.c utils.c munit/munit.c
```

Adjust file lists depending on which tests or program you want to build.

## Running

Run the calculator app from the project root. On Windows:

```powershell
.\app.exe
```

On Unix-like shells:

```bash
./app
```

Follow prompts printed by the program to perform operations. The program reads/writes `history.csv` to store a simple operation history (one CSV line per operation).

Run tests:

```powershell
.\unit_test.exe
.\int_test.exe
```

Or on Unix:

```bash
./unit_test
./int_test
```

The `munit` test framework included in `munit/` is compiled into the test executables; test output is printed to stdout and the test runner returns non-zero on failure.

## Project design and decisions
This section explains the structure, key design decisions, and expected behaviors.

Files and responsibilities
- `main.c` — CLI parsing and interactive loop. Presents a small menu, reads user input, calls `calculator` functions, and records results using the history module.
- `calculator.c` / `calculator.h` — Core arithmetic operations. Each operation is implemented as a function that takes numeric inputs and returns a result. Division returns an error code or uses a defined behavior for divide-by-zero cases (see Error handling).
- `history.c` / `history.h` — Simple CSV-backed history. Provides functions to append an operation to `history.csv` and to read recent entries.
- `utils.c` / `utils.h` — Helper functions for parsing, input validation, and small utilities shared across modules.
- `unit_test.c` / `int_test.c` — Test cases using the included `munit` framework. Unit tests target `calculator` functions and utilities. The integration test exercises `main`-level workflows and history persistence.

Design choices
- Single-responsibility modules: each .c/.h pair has a small, testable responsibility which simplifies unit testing and maintenance.
- CSV history: chosen for simplicity and human-readability. The format is a single line per operation: timestamp,operation,operand1,operand2,result (implementation-specific).
- Error handling: functions return integer error codes for exceptional cases (e.g., divide-by-zero). Tests assert correct error codes and program behavior on invalid input.
- Minimal dependencies: no external libraries besides the small `munit` test runner included in the repo.

Data shapes and contracts
- Calculator functions:
   - Inputs: two doubles (or ints depending on implementation) and an enum/op code for operation.
   - Outputs: result (double) and an integer status (0 for success, non-zero for errors).
- History append:
   - Input: operation metadata (timestamp string, op string, operands, result)
   - Output: boolean / status code indicating write success.

Edge cases considered
- Division by zero — calculator returns an error code and the CLI prints a helpful message. History should not record failed operations.
- Invalid numeric input — parsed by `utils` with validation; user is re-prompted on invalid input.
- File write errors (history CSV) — functions return an error code which the CLI surfaces to the user.

Testing notes
- Unit tests exercise each calculator operation with normal and boundary inputs, plus tests for parsing/validation helpers.
- Integration test verifies the CLI flow and that successful operations are persisted to `history.csv`.

Troubleshooting
- If `make` fails on Windows, ensure you have a make implementation (MSYS2/MinGW or WSL). Alternatively compile with the `gcc` commands above.
- If tests fail, run the test executable directly to see the failing assertion and trace back to the corresponding function.

Contact / Attribution
This repository was prepared as a small teaching project. If you need further documentation (detailed API comments, UML, or design diagrams) tell me which parts you'd like expanded.

## License
This repository does not include an explicit license file. Add a LICENSE file if you intend to publish or share the project publicly.

---

Additional notes (build, memory, and testing)
=============================================

Windows PowerShell / Platform notes
-----------------------------------
- If you use MinGW / MSYS2 with `make` installed, the standard `make` targets will work as described above.
- If you compile with `gcc` from MinGW, run the produced `app.exe` with PowerShell as:

```powershell
./app.exe
```

On WSL / macOS / Linux the binary is typically `app` and you run `./app`.

Memory management — known issues and suggested fixes
---------------------------------------------------
I reviewed the code for common memory-allocation patterns and found a few places that will leak memory in normal usage. These are small, low-risk fixes and I recommend applying them to make the code robust:

1. Free both string fields in history cleanup
   - Problem: `cleanup_history()` currently frees only `expression_str` for each `Calculation` and then frees the array. `result` is not freed.
   - Fix: in the loop free both `expression_str` and `result` and set them to NULL before freeing the array.

2. Free on partial allocation failure in `add_calculation`
   - Problem: `add_calculation()` allocates `expression_str`, then attempts to allocate `result`. If `result` allocation fails it returns without freeing `expression_str`.
   - Fix: on any allocation failure free previously-allocated buffers before returning.

3. Free temporary fields after parsing CSV lines
   - Problem: `load_history_from_file()` uses `parse_csv_line()` which allocates `expression_str` and `result` on a temporary `Calculation`. After calling `add_calculation()` the code frees only `expression_str` and not `result`.
   - Fix: always free both `temp_calc.expression_str` and `temp_calc.result` after use (or on error).

4. Centralize cleanup in `parse_csv_line`
   - Problem: several early-return paths in `parse_csv_line()` skip freeing the `line_copy` or fields already allocated.
   - Fix: implement a single `cleanup` label to free `line_copy`, `calc->expression_str`, and `calc->result` as needed before returning.

5. Initialize allocated history array to NULLs
   - Suggestion: use `calloc()` in `init_history()` (or `memset`) so that pointer fields are NULL by default. This makes freeing safe and easier to reason about.

Testing and verification
------------------------
Recommended steps to validate correctness and memory safety:

- Build and run unit tests (PowerShell):

```powershell
make unit_test
./unit_test.exe
```

- Build and run integration tests:

```powershell
make int_test
./int_test.exe
```

- Run the interactive app to exercise CLI flows:

```powershell
./app.exe
```

- Memory checking (recommended on WSL/Linux/macOS):

```bash
valgrind --leak-check=full ./unit_test
```

Development tips
----------------
- Prefer `calloc()` when allocating arrays of structs that contain pointer fields; this initializes pointers to NULL.
- When a function allocates multiple resources, use a single `cleanup` label to free any allocated resources before returning on errors.
- Return consistent error codes (e.g., `HISTORY_INVALID_INDEX` for invalid index cases) to make tests and callers reliable.

If you'd like, I can apply the memory-fix patches (small edits in `history.c`) and run the tests and Valgrind in a WSL environment to verify there are no leaks. Tell me if you want me to proceed with automatic fixes and verification.
