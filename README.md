# Automatic C++ Loop Parallelization Tool

**A C++ source-to-source compiler tool that automatically parallelizes `for` loops using OpenMP to leverage the power of modern multi-core processors.**

This project, developed as part of a study at Nile University, introduces a C++ tool designed to automatically inject OpenMP directives into sequential C++ code.  
Its goal is to simplify the process of parallel programming, making it accessible to developers without deep expertise in concurrency.

---

##  Table of Contents
- [About the Project](#-about-the-project)
- [How It Works](#-how-it-works)
- [Prerequisites](#prerequisites)
- [Project Report](#project-report)

---

## About the Project
The increasing number of cores in modern CPUs offers significant performance potential that is often untapped by legacy sequential code.  
Manual parallelization is a complex, time-consuming, and error-prone process.  

This tool solves that problem by:
- **Identifying `for` loops** suitable for parallel execution.
- Detecting **shared variables** declared outside the loop to prevent race conditions.
- Automatically inserting **`#pragma omp parallel for`** to distribute loop iterations across threads.
- Adding **`#pragma omp atomic`** directives to ensure thread safety when updating shared variables.

---

## How It Works
The tool performs a source-to-source transformation in these steps:

1. **Input & Reading** – The user provides a C++ source file and an output file name.
2. **Variable Detection** – Scans for variables declared outside of loops (shared variables).
3. **Loop Identification** – Detects `for` loops and inserts `#pragma omp parallel for`.
4. **Atomic Directive Injection** – Adds `#pragma omp atomic` for shared variable updates inside loops.
5. **Code Generation** – Outputs a modified C++ file with `<omp.h>` included.

**Flowchart (Example):**
[Input File] → [Analyze Variables] → [Detect Loops] → [Insert OpenMP Directives] → [Output File]


---

## Prerequisites
You need a C++ compiler with OpenMP support.  
Example (Ubuntu/Debian):
```bash
sudo apt-get install g++
```
---

## Project Report
You can read the full project report here: [View the Full Report](report/parallel project.pdf)

---

## Tools and Technologies Used
This project was built and evaluated using the following tools:

**C++:** The core language used to build the source-to-source parallelization tool.
**OpenMP:** The API used for multi-threading to parallelize for loops through compiler directives.
**g++:** The standard GNU C++ compiler, used with the -fopenmp flag to compile the parallelized code.
**Intel® VTune™ Profiler:** A performance analysis tool used to measure and compare the execution times of the sequential and parallel versions of the code.
**Git & GitHub:** Version control and hosting for the project.
