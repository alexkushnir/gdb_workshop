# Exercise 03: Debugging with VS Code UI

In this exercise, you will practice using the graphical debugging interface provided by Visual Studio Code.

## Goal

Your goal is to use VS Code's debugger to inspect a recursive function, explore the call stack, and monitor how variables change across different levels of recursion.

## Tasks

1.  **Build the target**: Open a terminal in this folder and run `./build.sh`. Alternatively, if you have the "CMake Tools" extension, you can let it configure and build the project for you.
2.  **Open the file**: Open `main.cpp` in VS Code.
3.  **Set a breakpoint**:
    *   Find the `Fibonacci` function.
    *   Click in the gutter (to the left of the line numbers) on the line `if (n <= 1)`. A red dot should appear.
4.  **Start Debugging**:
    *   Press `F5` or go to the "Run and Debug" side bar and click "Start Debugging".
    *   *Note: Ensure "ex03" is selected as the launch target in the status bar if you are using CMake Tools.*
5.  **Explore the UI**:
    *   **Call Stack**: Look at the "Call Stack" panel on the left. As you continue (`F5`), you will see more and more frames being added as the recursion goes deeper.
    *   **Variables**: Look at the "Variables" panel. You can see the value of `n` for the current frame. Click on previous frames in the Call Stack to see the values of `n` in those calls.
    *   **Watch**: Click the `+` in the "Watch" panel and type `n * n`. You can watch any expression here!
    *   **Debug Console**: You can type GDB commands here prefixing them with `-exec ` (e.g., `-exec info locals`).
6.  **Control Flow**:
    *   Use the floating toolbar to:
        *   **Continue** (`F5`): Run until the next breakpoint.
        *   **Step Over** (`F10`): Go to the next line without entering functions.
        *   **Step Into** (`F11`): Enter the `Fibonacci` function.
        *   **Step Out** (`Shift+F11`): Finish the current function and return to the caller.
        *   **Restart** (`Ctrl+Shift+F5`).
        *   **Stop** (`Shift+F5`).

Visual debugging is excellent for understanding complex state and deep call stacks that might be hard to track mentally in a text-only environment.
