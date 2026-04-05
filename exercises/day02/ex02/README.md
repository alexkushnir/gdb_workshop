# Exercise 02: Attaching to a Running Process

In this exercise, you will practice attaching GDB to a process that is already running. This is a vital skill for debugging services, long-running background tasks, or programs that seem to "hang" unexpectedly.

## Goal

Your goal is to:
1. Start a long-running process.
2. Identify its Process ID (PID).
3. Attach GDB to it, pause its execution, and inspect its current state.

## Tasks

1.  **Build the target**: Run `./build.sh` to compile the exercise.
2.  **Start the program**: 
    Run the program in your terminal. It will print its PID and then enter a loop:
    `./build/ex02`
3.  **Open another terminal**: (Or put the first one in the background).
4.  **Attach GDB**:
    You have two ways to do this:
    *   **Directly from the shell**:
        `gdb -p <PID>`
    *   **From inside GDB**:
        1. Start GDB: `gdb`
        2. Inside GDB, type: `attach <PID>`
5.  **Inspect the process**:
    Once attached, GDB will automatically pause the program (it sends a `SIGSTOP`).
    *   Type `bt` to see where it was stopped (likely inside `std::this_thread::sleep_for` or a syscall).
    *   Type `p counter` to see how many iterations it has completed.
6.  **Modify and Continue**:
    *   Change the value of a variable: `set var counter = 1000`
    *   Type `continue` (or `c`) to let the program resume.
7.  **Detach**:
    When you are done, you don't want to kill the process! 
    *   Type `detach` to let the program continue running independently of GDB.
    *   Alternatively, `quit` will ask if you want to detach first.

Attaching to processes allows you to debug issues "in-flight" without having to restart the application, which is crucial for troubleshooting production-like environments or complex system integrations.
