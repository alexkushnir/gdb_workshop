#!/bin/bash
# 03_helgrind/run_demo.sh - Runs the demo project under Valgrind's Helgrind tool

EXECUTABLE="./build/helgrind_demo"

if [ ! -f "$EXECUTABLE" ]; then
    echo "Executable $EXECUTABLE not found. Please run ./build.sh first."
    exit 1
fi

if ! command -v valgrind &> /dev/null; then
    echo "Error: valgrind is not installed."
    exit 1
fi

echo "============================================================"
echo "Running Helgrind: Data Race Detection"
echo "============================================================"
echo "Helgrind will analyze the program for synchronization errors."
echo "Expect reports about 'Possible data race' on 'sharedCounter'."
echo "============================================================"
echo ""

valgrind --tool=helgrind $EXECUTABLE
