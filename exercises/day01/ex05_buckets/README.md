# Exercise 05: The Buckets Problem

In this exercise, you will debug a recursive algorithm designed to solve the "Bucket Filling" problem. The goal is to determine if a large bucket can be exactly filled using a set of smaller buckets.

## Goal

Your goal is to use GDB to understand the recursion in `canFillRecursive`, identify bugs in the implementation, and determine if any of the test cases themselves are incorrect. This exercise focuses on debugging recursive calls, inspecting the call stack, and comparing different versions of the code.

## Background

The `tests.cpp` program currently fails a few tests. While these tests reportedly passed in the past, their correctness is now in question. It is possible that some tests are faulty and should be amended, or that the code has regressions that never passed before.

There are two versions of the code available:
- `tests.cpp`: The current version, which fails some tests but does not crash.
- `tests_prev.cpp`: A previous version from source control that is known to crash on at least one of the tests.

It appears that an attempt was made to fix the code (resulting in `tests.cpp`), but the fix might be incomplete or incorrect. You should review the tests, debug the code, and determine if the issue lies in the implementation, the tests, or both. You can use both versions to pinpoint the issues.

## Tasks

1.  **Build the targets**: Run `./build.sh` to compile both versions of the exercise.
2.  **Run the tests**:
    *   Execute `./build/summary_exercise` (based on `tests.cpp`) and observe the failures.
    *   Execute `./build/summary_exercise_prev` (based on `tests_prev.cpp`) and observe the crash.
3.  **Start GDB**: Use GDB to investigate the failures and the crash.
4.  **Set breakpoints**:
    *   Set a breakpoint at `canFillRecursive`.
    *   Set a conditional breakpoint for a specific `big_bucket` value (e.g., `break canFillRecursive if big_bucket == 11`).
5.  **Examine recursion**:
    *   Use `backtrace` (`bt`) to see the depth of the recursion.
    *   Use `frame` (`f`) to switch between different levels of the call stack.
    *   Inspect local variables at each frame using `info locals`.
6.  **Debug and Fix**:
    *   Identify why `tests_prev.cpp` crashes.
    *   Identify why `tests.cpp` fails certain tests.
    *   Determine if any tests in `tests.cpp` are expecting the wrong result and should be amended.

## Problem Description

The problem is to find a way to fill a bucket of size `N` using buckets of sizes $\{s_1, s_2, \dots, s_k\}$.
A "Fill Plan" is a mapping from bucket size to the number of times it is used.

For example:
- Big bucket: 11
- Small buckets: {3, 2}
- One solution: 3 * 1 + 2 * 4 = 11.

Take your time to get comfortable with `bt`, `frame`, and `info locals`. These are essential for debugging complex logic and recursion!
