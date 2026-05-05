# Exercise 02b: Thread Pools and Logical Races

In this exercise, you will investigate a multithreaded application that uses a thread pool and explore logical race conditions that occur even without explicit thread cancellation.

## Goal

Understand how tasks are scheduled in a thread pool and use GDB to trigger and analyze a race condition between a task's completion and its timeout timer.

## Background

This exercise is the direct successor to [Exercise 02a](../ex02a/README.md). While the previous exercise used raw `std::jthread` and the potentially dangerous `pthread_cancel` for timeouts, this version adopts a more modern and robust approach:

- **Thread Pool**: Uses `boost::asio::thread_pool` instead of manual thread management.
- **Fixed Capacity**: Tasks are submitted to a pool of a fixed size (3 threads), introducing queuing behavior.
- **Cooperative Timeout**: Instead of killing threads with `pthread_cancel`, we use an `std::atomic<bool> guard` to synchronize between the task execution and the timeout handler. The "stuck" thread is allowed to finish its task, but the result is determined by who wins the race for the atomic guard.

## Tasks

1.  **Build and Run**: Compile the project using the provided `build.sh`:
    ```bash
    ./build.sh
    ```
    Note that it now uses a thread pool, so with 7 tasks and a pool size of 3, some tasks will wait in the queue.


2.  **Analyze the Race Condition**:
    The code has a logical race condition:
    - The task finishes its work and calls `timer.cancel()`.
    - The timer expires and calls `timerHandler`.
    Both try to update the `guard` and set the `result`.

3.  **Simulate the Race with GDB**:
    Follow the suggestion from [Exercise 02a](../ex02a/README.md):
    *   Start GDB: `gdb ./build/ex02b`.
    *   Set breakpoints in both the task's completion path and the `timerHandler`:
        *   `b Task::submit::{lambda()#1}::operator()() const` (Wait, this might be tricky to name, better use line numbers or `Task.cpp` if split).
        *   Inside `main.cpp`, set a breakpoint at the `std::this_thread::sleep_for` call (line 68) and at the start of `timerHandler` (line 24).
    *   When a thread hits the `sleep_for` for a task that is close to the limit (like Task 3 with 599ms), use GDB to pause it.
    *   Wait for the timer thread to hit the `timerHandler`.
    *   Use GDB to switch between threads and observe which one "wins" the `compare_exchange_strong` race.

4.  **Observe Pool Behavior**:
    *   With `pool_size = 3`, use `info threads` to see how many worker threads are created by the pool.
    *   Notice that not all tasks start immediately.

## Questions to Consider

- How does the fixed pool size affect the timing of the tasks and their timers?
- Even if `timer.cancel()` is called, is it possible for `timerHandler` to still be executed? (Hint: check Boost.Asio documentation on `cancel()`).
- How would you use GDB's `scheduler-lock` to make simulating this race easier?
- Is the race between cancellation and finish handlers a valid race condition? (not a bug!)

---

### Key GDB Commands:
- `set scheduler-lock on`: Prevents other threads from running while you are stepping the current thread.
- `thread <ID>`: Switch context to a specific worker thread.
- `watch task.guard`: Monitor changes to the atomic guard variable.
