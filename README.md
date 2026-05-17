# COSC-2436 Algorithm Benchmarking Suite

A single-file C++ benchmarking project for comparing practical performance differences across several core data structures and algorithms. The program evaluates sorting algorithms, linear search performance across different storage structures, and matrix multiplication loop ordering.

This project was originally written for a COSC-2436 data structures course assignment under a single-file submission constraint. It has not yet been refactored into a multi-module build, but the current version is fully executable from one `main.cpp` file.

## Project Overview

This application benchmarks three major concepts:

1. **Merge Sort vs. Heap Sort**
   - Integer array sorting
   - Fixed-length character array sorting
   - Validation that both algorithms produce sorted output
   - Output comparison between both sorting implementations

2. **Array vs. Linked List Linear Search**
   - Searches identical record data stored in both an array and a linked list
   - Uses both found and not-found search cases
   - Validates that both search methods return equivalent results

3. **Matrix Multiplication Loop Ordering**
   - Compares `i-j-k` loop ordering against `i-k-j` loop ordering
   - Demonstrates the practical performance effect of memory access patterns
   - Validates that both multiplication approaches produce matching results within floating-point tolerance

## Why This Project Matters

The goal of this project is not only to implement algorithms, but to compare how theoretical complexity and real-world performance can differ depending on memory layout, cache behavior, data structure choice, and implementation details.

For example:

- Merge sort and heap sort share the same general `O(n log n)` complexity class, but they can behave differently in practice.
- Array and linked list linear search are both `O(n)`, but arrays often benefit from better memory locality.
- Matrix multiplication has the same mathematical result regardless of loop order, but loop ordering can significantly change runtime due to cache efficiency.

## Current Structure

The project is currently contained in a single source file:

```text
main.cpp
```

The single-file structure was intentional for the original assignment requirements. The program uses local structs and helper lambdas inside `main()` to keep the implementation self-contained while still separating responsibilities logically.

A future refactor could split the project into modules such as:

```text
src/
  main.cpp
  sorting_benchmarks.cpp
  search_benchmarks.cpp
  matrix_benchmarks.cpp
  benchmark_utils.cpp

include/
  sorting_benchmarks.h
  search_benchmarks.h
  matrix_benchmarks.h
  benchmark_utils.h
```

## Features

- Randomized dataset generation
- Multiple trial averaging
- Integer sorting benchmarks
- Character array sorting benchmarks
- Array and linked-list search comparison
- Matrix multiplication loop-order comparison
- Runtime measurement using `std::chrono`
- Validation checks for correctness
- Dynamically allocated arrays and linked lists
- Manual memory cleanup
- Clear formatted console output

## Benchmark Categories

### 1. Integer Sorting

Compares merge sort and heap sort on randomly generated integer arrays.

Tested sizes:

```text
10,000
25,000
50,000
100,000
```

For each size, the program:

- Generates random integer data
- Copies the same data into separate arrays
- Sorts one copy with merge sort
- Sorts one copy with heap sort
- Measures average runtime
- Validates sorted output
- Confirms both algorithms produce matching results

### 2. Character Array Sorting

Compares merge sort and heap sort on fixed-length C-style character arrays.

Tested sizes:

```text
10,000
25,000
50,000
100,000
```

This benchmark intentionally uses fixed-length character arrays instead of `std::string` arrays to practice lower-level data handling and lexicographic comparison with `strcmp`.

### 3. Array vs. Linked List Search

Compares linear search performance on equivalent data stored in:

- A dynamically allocated array
- A singly linked list

Tested sizes:

```text
10,000
25,000
50,000
100,000
```

Each trial performs 1,000 searches using a mix of found and not-found target IDs.

### 4. Matrix Multiplication Loop Ordering

Compares two loop orders for square matrix multiplication:

```cpp
i-j-k
i-k-j
```

Tested matrix sizes:

```text
64 x 64
128 x 128
256 x 256
512 x 512
```

This benchmark demonstrates how equivalent algorithms can produce different runtimes because of cache behavior and memory access patterns.

## Requirements

A C++ compiler with support for modern C++ features.

Recommended:

- `g++`
- `clang++`
- C++17 or newer

The program uses standard library headers only and does not require external dependencies.

## How to Build

From the project directory:

```bash
g++ -std=c++17 -O2 -Wall -Wextra -pedantic main.cpp -o benchmark_suite
```

If your source file has a different name, replace `main.cpp` with the correct filename.

For example:

```bash
g++ -std=c++17 -O2 -Wall -Wextra -pedantic "main(13).cpp" -o benchmark_suite
```

## How to Run

```bash
./benchmark_suite
```

The program will print benchmark results directly to the console.

## Example Output Format

The exact timing results will vary depending on hardware, compiler, optimization settings, and system load. The output is organized into sections similar to:

```text
============================================================
BENCHMARK 1A: INTEGER SORTING
============================================================
Array size:                     10000
Trial count:                    10

Results
------------------------------------------------------------
Merge sort average:             1.234 ms
Heap sort average:              2.345 ms
Merge sort validation:          PASS
Heap sort validation:           PASS
Sorted outputs match:           PASS
```

## Notes on Benchmark Interpretation

Benchmark results should be interpreted as practical observations, not universal claims. Runtime may change depending on:

- CPU architecture
- Cache size
- Compiler optimization level
- Operating system scheduling
- Background processes
- Dataset size
- Memory allocation behavior

The project is intended to demonstrate careful experimental comparison, validation, and performance reasoning.

## Future Improvements

Planned or possible future improvements include:

- Refactor into multiple source and header files
- Add a CMake build system
- Write benchmark results to CSV
- Add command-line options for dataset sizes and trial counts
- Add graphs or automated result visualization
- Separate algorithm implementations from benchmark runners
- Add unit tests for individual algorithms
- Add more data structures and algorithms
- Compare CPU and GPU implementations in a future expanded version

## Academic Context

This project was created for a COSC-2436 data structures course. The implementation emphasizes algorithmic understanding, validation, memory management, benchmarking discipline, and clear console reporting.

## Author

Rayla Dwight
