# Exercise 04: GDB Catchpoints

In this exercise, you will practice using **catchpoints**, which are special breakpoints that trigger on events like exceptions, process forks, or system calls.

## Goal

Your goal is to use GDB to "catch" C++ exceptions as they are thrown and caught, allowing you to inspect the state of the program at the exact moment an error occurs.

## Tasks

1.  **Build the target**: Run `./build.sh` to compile the exercise.
2.  **Start GDB**: Start GDB with the compiled executable:
    `gdb ./build/ex04`
3.  **Catching Throws**:
    *   Instead of setting a breakpoint on a line, type:
        `catch throw`
    *   Type `run`. GDB will stop the execution the moment an exception is thrown, *before* the stack is unwound to find a handler.
    *   Use `bt` (backtrace) to see exactly where the exception originated.
4.  **Catching Catches**:
    *   You can also catch when an exception is handled:
        `catch catch`
    *   Continue execution (`c`). GDB will stop when the exception enters a `catch` block.
5.  **Conditional Catchpoints**:
    *   Just like breakpoints, catchpoints can be made conditional or used with `info breakpoints`.
6.  **Other Events**:
    *   Try `catch fork` if the program used `fork()`.
    *   Try `catch load` to stop when a shared library is loaded.

Catchpoints are powerful for debugging "silent" failures where an exception is thrown and caught deep in a library, but you need to know *why* it happened in the first place.
