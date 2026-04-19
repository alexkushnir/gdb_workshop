# Home Exercise: The Secret Profile Bug

In this exercise, you will investigate a security bug in a server login process. A user who provides an incorrect password still manages to have their private profile data loaded!

## Goal

Your goal is to use GDB to figure out why `LoadUserProfile` is being called for the user `HackerBob` even though their password validation failed. We will use an advanced technique to break ONLY when the error path is taken, skipping the 100 successful logins.

## Tasks

1.  **Build the target**: Run `build.sh` to compile the exercise.
2.  **Start GDB**: Start GDB with the compiled executable:
    `gdb ./build/login`
3.  **Setup Dynamic Breakpoints**:
    *   First, set a breakpoint on the target function we want to protect, then disable it:
        ```gdb
        (gdb) b LoadUserProfile
        (gdb) disable 1
        ```
    *   Next, set a breakpoint on the "trigger" function (`ShowErrorMessage`) and configure it to enable the first breakpoint only when it is hit:
        ```gdb
        (gdb) b ShowErrorMessage
        (gdb) commands 2
        > enable 1
        > continue
        > end
        ```
4.  **Run and Investigate**:
    *   Run the program (`r`). 
    *   The execution will automatically pass through the 100 successful logins.
    *   When `HackerBob` fails, `ShowErrorMessage` is called, which enables Breakpoint 1 and continues.
    *   GDB will then stop inside `LoadUserProfile`.
5.  **Spot the Logic Error**:
    *   Look at the call stack (`bt`) to see how you got here.
    *   Step out of `LoadUserProfile` (`finish`) to see the calling code in `ProcessLogin`.
    *   Observe how the program execution continues into `LoadUserProfile` instead of returning after the error.
6.  **Fix the Bug**:
    *   Open `login.cpp` and look at the `ProcessLogin` function. Based on your GDB investigation, what needs to be changed to ensure `LoadUserProfile` is only called for successful logins?
    *   Apply your fix, re-build, and verify with GDB that the security breach is resolved.

This technique helps you reach deep logic errors that only occur after specific events, without manually stepping through thousands of successful iterations!
