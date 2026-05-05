# Exercise 02a: Thread Cancellation

In this exercise, you will explore how to debug applications that use `pthread_cancel` for thread management and how to handle asynchronous thread termination in GDB.

## Goal

Understand the behavior of `pthread_cancel` and learn how to use GDB to identify which threads are being cancelled and why.

## Background

The application simulates a task runner where each task has a maximum allowed execution time. If a task exceeds this limit, a timer (using `boost::asio::steady_timer`) triggers a cancellation request for the thread executing that task.

The threads are configured with:
- `PTHREAD_CANCEL_ENABLE`: Cancellation is allowed.
- `PTHREAD_CANCEL_ASYNCHRONOUS`: Cancellation can happen at any time, not just at cancellation points.

## Tasks

1.  **Build the target**: Use the provided `build.sh` to compile the project:
    `bash
    ./build.sh
    `
2.  **Run the application**: Execute `build/ex02a`. Observe the output. Some tasks will finish normally, while others will be reported as "Timer expired" and the task will be cancelled.

3.  **Debug with GDB**:
    *   Start GDB: `gdb ./build/ex02a`.
    *   Set a breakpoint on `pthread_cancel`: `b pthread_cancel`.
    *   Run the program.
    *   When the breakpoint is hit, use `info threads` to see which threads are active.
    *   Use `backtrace` to see which part of the code (the timer handler) is requesting the cancellation.
    *   Identify which task index is associated with the cancellation request.

4.  **Catching the Cancellation**:
    *   Threads that are cancelled often receive a signal or exit with a specific status.
    *   Try to use `catch throw` (if it were C++ exceptions) or simply observe the thread exit messages in GDB.
    *   Since `PTHREAD_CANCEL_ASYNCHRONOUS` is used, the thread can be stopped anywhere. Use `info threads` repeatedly to see threads disappearing.

## Questions to Consider
- We want to use GDB to simulate different time sharing between threads. Try to simulate a state where the thread finished the task exactly on timeout, creating a valid logical race condition. Use GDB to validate that your assumption about the race condition is correct.
- What happens to the `std::latch` if a thread is cancelled before it calls `count_down()`? (Look at how `timerHandler` handles this).
- Is `PTHREAD_CANCEL_ASYNCHRONOUS` safe for code that allocates memory or uses locks?
- How does GDB report a thread that has been cancelled?

---

### Key GDB Commands learned:
- `b pthread_cancel`: Break on the function that initiates cancellation.
- `info threads`: List all active threads and their status.
- `thread apply all bt`: Show backtraces for all threads to see where they are when a cancellation is pending.
