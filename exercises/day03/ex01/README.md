# Exercise 01: Debugging Multithreaded Applications

In this exercise, you will practice debugging a multithreaded C++ application and learn how to use GDB's thread-specific commands.

## Goal

Your goal is to inspect a program that suffers from a data race due to unsynchronized access to a shared variable, and use GDB to switch between and examine different threads.

## Tasks

1.  **Build the target**: Run `./build.sh` to compile the exercise.
2.  **Observe the Issue**: Run the compiled executable (in `build/`) without GDB, using `for i in {1..900}; do ./build/ex01; done | sort | uniq -c` . You will likely see that the printed `sum` has different values, indicating a data race.
3.  **Start GDB**: Start GDB with the compiled executable: `gdb ./build/ex01`.
4.  **Set Breakpoints**:
    *   Set a breakpoint at the beginning of `main`.
    *   Set a breakpoint on the end of `main` function (line 25)
    *   Set a breakpoint inside the thread function (`b func`)
    *   Set non-stop mode: `set non-stop on`.
5.  **Run**: Type `run`. The program will stop at `main`. Continue (`c`) to hit the breakpoint inside `func`.
6.  **Inspect Threads**:
    *   When stopped in `func`, type `info threads`. Notice the list of active threads, their IDs, and what function they are currently executing. The current thread is marked with an asterisk (`*`).
    *   Locate the thread where the value of the `a` parameter is 0. 
    *   Switch to that thread using `thread <ID>` (e.g., `thread 2`).
    *   Print the value of the shared `sum` pointer and the local `a` parameter in the new thread.
7.  **Advancing to `*sum += a;`, stopping after loading sum, and before storing it back to memory**:
    *   Add display of the next machine instruction `display/i $pc`. 
    *   Use `stepi` (step into machine instruction) to step into the instruction `*sum += a;`.
    *   Repeat stepi until `addsd` is the next insruction. Be careful not to pass this point!
    *   Continue execution for all other threads until they finish, using the following command sequence:
    ``` 
    set $thr_main=1 # Make sure the main thread has ID 1
    set $thr_0=<ID from paragraph 6>
    set $thr_idx=11
    while ($thr_idx > 0)
      if ($thr_idx != $thr_main && $thr_idx != $thr_0)
        thread $thr_idx
        cont
      end
      set $thr_idx = $thr_idx - 1
    end
    ```
8. **Examine the main thread:**
    * Switch to main thread using `thread 1`.
    * You'll notice that `bt` doesn't show any backtrace. This is because we are using non-stop mode, and the main thread is not on a breakpoint.
    * To pause the main thread execution, without altering the execution flow or state, type `interrupt`.
    * Now you can use `bt` to see the backtrace of the main thread (you'll probably see that the main is stuck on `join()`).
    * Move to the frame of the `main` function using `frame` command.
    * Print `sum` value in the main thread (4500?).
    * Type `continue` to continue execution of the main thread to send it back to wait on `join`. 

9.  **Continue Execution**: 
    * Now switch back to the only worker thread that is left. 
    * Type `continue` to continue execution of that thread until it finishes. 
    * Now you should be at the breakpoint on the end of `main` function. Check the value of `sum` variable.

10. **Finish the program**: 
    * Switch to main thread using `thread 1`. 
    * Type `continue` to continue execution of the program and observe the output. 


---

What we've learned?
---
- `set non-stop on` : Allows the debugger to continue executing threads even if one or more threads are stopped at breakpoints.
- `info threads` : Displays a list of all threads in the program.
- `thread <ID>` : Switches to the specified thread.
- `display/i $pc` : Displays the next machine instruction to be executed.
- `stepi` : Steps into the next machine instruction.
- `interrupt` : Interrupts the execution of the program.
- `frame` : Moves to the specified frame.

