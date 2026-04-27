# Exercise 04: Advanced Core Dump Analysis

In this exercise, you will debug a Maze Solver that crashes under certain conditions. This exercise is designed to practice analyzing coredump using GDB.

## Goal

Your goal is to identify the root cause of a segmentation fault in the `MazeSolver` class. You will practice using GDB to inspect a crashed state and understand the crash scenario.

## Background

The program implements a recursive maze solver with a "greedy bounty" mode. When it finds a bounty ('$'), it records the path and potentially backtracks to explore other branches while keeping the bounty in its "collected" list.

The current implementation has a bug leading to a crash in certain scenarios.

## Tasks

1.  **Build the target**: Run `./build.sh` to compile the exercise.
2.  **Run the program**: Run `./build/additional_core_dump`. Observe the crash (Segmentation fault).
3. **Make sure you have a core dump**: If your system is not configured to generate core dumps, enable it:
    ```bash
    ulimit -c unlimited
    ```
    To change coredump settings inside docker there might be a need to run docker with `--privileged`
    You can load the core file:
        ```bash
        gdb ./build/additional_core_dump core
        ```
4.  **Analyze the Crash Using the Coredump**:
    * Use `bt` to see the call stack.
    * Look for suspicious values in variables. This may be an indicator of memory corruption or reading from invalidated memory.
5.  **Investigate the Code**:
    * Examine the code around problematic lines related to the suspicious values found in the previous step.
    * Try to identify the root cause of the crash.
6.  **Fix the Code**:
    * Fix the code to prevent the crash.
7.  **Verify the Fix**:
    * Run the program again and make sure it doesn't crash.

## Key GDB Commands to Use

* `bt`: Show the call stack at the crash point.
* `f <number>`: Select a frame to inspect (e.g., `f 0`).
* `p <variable>`: Print the value of a variable or expression.
* `info locals`: Show local variables in the current frame.
* `list`: Show the source code around the current line.