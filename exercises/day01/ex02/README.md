# Exercise 02: Working with GDB TUI

In this exercise, you will practice using the Text User Interface (TUI) in GDB to visualize code, variables, and optionally assembly or registers.

## Goal

Your goal is to use GDB TUI to watch a bubble sort algorithm sort a vector of integers, observing how the TUI makes it easier to understand the context of the execution.

## Tasks

1.  **Build the target**: Run `./build.sh` to compile the exercise.
2.  **Start GDB with TUI**: Start GDB with the compiled executable and enable TUI right away by starting it with the `-tui` flag:
    `gdb -tui ./build/ex02`
    *(Alternatively, you can press `Ctrl-X` then `A` inside a regular GDB session).*
3.  **Basic TUI Navigation**:
    *   Set a breakpoint at `main` and type `run`.
    *   Notice how the top screen shows your source code and highlights your current line.
    *   Use the Up and Down arrow keys. You'll notice they scroll the source code window instead of command history! 
    *   To look at the command history, use `Ctrl-P` and `Ctrl-N` (since the arrow keys are bound to the source window).
4.  **TUI Layouts**:
    *   Type `layout split` to see both assembly and source code.
    *   Type `layout regs` to see the CPU registers along with the source or assembly.
    *   Use `focus cmd` to make sure your keyboard inputs go to the command window, or `focus src` to send them to the source window for scrolling.
    *   Type `layout src` to go back to just the source view.
5.  **Debugging the Sort**:
    *   Set a breakpoint inside `BubbleSort` (`b BubbleSort`).
    *   Use `next` (`n`) and `step` (`s`) to step through the swapping logic. Focus on how much easier it is to understand the context when you can see the loops around your current execution line.
    *   Use `tui reg general` (while in the register layout) to see how CPU registers change as the loop progresses.
6.  **Refreshing**: If the screen ever gets garbled by program output overlapping with GDB's UI, press `Ctrl-L` or type `refresh` to redraw the screen.

TUI is an incredibly powerful feature when you don't have a full IDE but need to quickly understand the control flow of unfamiliar C/C++ code.
