# Exercise 01: Core Dumps and Separated Symbols

In this exercise, you will learn how to debug a program using a core dump and how to load debug symbols manually when they are not part of the executable.

## Goal

Your goal is to:
1. Enable core dumps on your system.
2. Run a crashing program that has been stripped of symbols.
3. Observe how GDB provides almost no information initially.
4. Load the separate debug symbols and finally identify the cause of the crash.

## Tasks

1.  **Build the target**: Run `./build.sh`. This script will:
    *   Compile the program with debug symbols.
    *   Extract the symbols into `ex01.debug`.
    *   Strip the symbols from the main executable `ex01`.
2.  **Enable Core Dumps**: By default, Linux often disables core dumps for the current shell. Enable them:
    ```bash
    ulimit -c unlimited
    ```
    *Note: This only affects the current terminal session.*

3.  **Run the program**: 
    ```bash
    ./build/ex01
    ```
    It should crash with "Segmentation fault (core dumped)".

4.  **Find the Core File**:
    Depending on your system's configuration (`/proc/sys/kernel/core_pattern`), the core file may appear as a file named `core` or `core.<pid>` in the current directory.
    
    If it doesn't appear, your system might be using `systemd-coredump`. You can list最近的 crashes with:
    ```bash
    coredumpctl list
    ```
    And extract the latest core to a file named `core` with:
    ```bash
    coredumpctl dump -o core
    ```

5.  **Start GDB with the core dump**:
    ```bash
    gdb ./build/ex01 core
    ```

5.  **Observe the missing symbols**:
    Type `bt`. Notice how there are no function names or line numbers, just memory addresses. This is because the executable is stripped!
6.  **Load the symbols manually**:
    Inside GDB, type:
    `symbol-file ./build/ex01.debug`
7.  **Inspect the crash**: 
    Type `bt` again. Now you should see the function names and line numbers. Identify where the null pointer dereference happened.

This technique is essential for production debugging, where binaries are often stripped to save space, but symbols are kept on a separate "symbol server" or in a local archive.
