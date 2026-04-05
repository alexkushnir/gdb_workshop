# Exercise 01: Breakpoints and Displaying Data

In this exercise, you will practice the fundamentals of debugging with GDB: setting breakpoints, stepping through code, and examining variables.

## Goal

Your goal is to pause the execution of the `ex01` executable at various points, inspect the `inventory` data, and observe how quantities and totals are calculated.

## Tasks

1.  **Build the target**: Run `./build.sh` to compile the exercise.
2.  **Start GDB**: Start GDB with the compiled executable (which will be located in the `build/` directory).
3.  **Set breakpoints**:
    *   Set a breakpoint at the `main` function.
    *   Set a breakpoint at the `process_order` function.
    *   Set a breakpoint at the line inside `calculate_total` where `total` is accumulated.
4.  **Run the program**: Run the program until you hit your first breakpoint.
5.  **Step and Next**:
    *   Use `next` (`n`) to step over lines within `main`.
    *   Use `step` (`s`) to step into functions like `process_order` and `calculate_total`.
6.  **Print & Display Variables**:
    *   When stopped in `process_order`, `print` the value of `item` (`p item`).
    *   Use the `display` command to automatically show `item.quantity` or `item.name` every time execution stops.
    *   When inside the loop of `calculate_total`, print the `total` accumulated so far.
7.  **Explore**: Experiment with `step` vs. `next`, and see how variables change as you step through the loops.

Take your time to get comfortable with `b` (break), `n` (next), `s` (step), `c` (continue), `p` (print), and `display`. These are the bread and butter of everyday GDB usage!
