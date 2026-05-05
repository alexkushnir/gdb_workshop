# Exercise 02c: Data Races and Thread Sanitizers

In this exercise, you will investigate how a "logical race" can turn into a dangerous "data race" when variables are not properly synchronized, and learn how to use automated tools to detect these issues.

## Goal

Identify a data race in the application using GDB, and then use Helgrind and ThreadSanitizer (TSAN) to confirm the findings.

## Background

This exercise is the successor to [Exercise 02b](../ex02b/README.md).
In the previous version, the `result` variable was an `std::atomic<int>`, which ensured that even if both threads wrote to it, the memory access was synchronized.

In this version (`ex02c`):
- **Unsynchronized `result`**: The `result` member is now a plain `int`.
- **Pre-emptive Write**: Both the `timerHandler` and the task execution thread write to `result` *before* they attempt to acquire the `guard` (the atomic boolean).
- **The Bug**: This creates a classic **data race**—two threads accessing the same memory location simultaneously, with at least one access being a write, and without proper synchronization.

## Tasks

1.  **Build the target**: Use the provided `build.sh` to compile the project:
    ```bash
    ./build.sh
    ```

2.  **Analyze the Code**: Locate the lines in `main.cpp` where `result` is updated. Notice that the update happens before the `guard.compare_exchange_strong(...)` call.


3.  **Debug with GDB**:

    *   Start GDB: `gdb ./build/ex02c`.
    *   Set a watchpoint on a specific task's result: `watch tasks.begin()->result` (You may need to find the address or use a specific task index).
    *   Try to catch the moment where two different threads write to the same `result` variable.

4.  **Use Thread Sanitizers**:

    Automated tools are often much better at catching these intermittent issues than manual debugging.
    *   **Helgrind (Valgrind)**:
        Run the program under Valgrind's Helgrind tool:
        `valgrind --tool=helgrind ./build/ex02c`
        Look for "Possible data race" errors in the output.
    *   **ThreadSanitizer (TSAN)**:
        Recompile the program with TSAN enabled (usually `-fsanitize=thread`).
        > [!TIP]
        > If you used the provided `build.sh`, you might be able to run it with a flag like `--tsan`.
        Run the resulting executable. TSAN will provide a detailed report showing the exact lines of code involved in the race.

## Questions to Consider

- Why was the race in `ex02b` considered "valid race condition" but not a "data race"?
- What are the performance gains of making `result` atomic vs. using a mutex?
- How does TSAN's output differ from Helgrind's in terms of readability and detail?

---

### Suggested Tools for Verification:
- **Helgrind**: `valgrind --tool=helgrind ./executable`
- **TSAN**: Compile with `-fsanitize=thread -g` and run normally.
- **GDB Watchpoints**: `watch -l result_variable` to catch concurrent modifications.
