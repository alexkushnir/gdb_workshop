#!/bin/bash
# run_demo.sh - Runs Valgrind on the demonstration project

EXECUTABLE="./build/valgrind_demo"

if [ ! -f "$EXECUTABLE" ]; then
    echo "Executable $EXECUTABLE not found. Please run ./build.sh first."
    exit 1
fi

if ! command -v valgrind &> /dev/null; then
    echo "Error: valgrind is not installed."
    exit 1
fi

echo "======================================================"
echo "Running Valgrind: Full Leak Check"
echo "======================================================"
valgrind --leak-check=full $EXECUTABLE leak

echo -e "\n\n======================================================"
echo "Running Valgrind: Track Origins (for uninit variables)"
echo "======================================================"
valgrind --track-origins=yes $EXECUTABLE uninit

echo -e "\n\n======================================================"
echo "Running Valgrind: Invalid Free"
echo "======================================================"
valgrind $EXECUTABLE free

echo -e "\n\n======================================================"
echo "Running Valgrind: Heap Out-of-Bounds"
echo "======================================================"
valgrind $EXECUTABLE bounds
