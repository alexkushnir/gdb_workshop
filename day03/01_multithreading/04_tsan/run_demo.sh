#!/bin/bash
# 04_tsan/run_demo.sh - Executes the TSan-enabled binary

EXECUTABLE="./build/tsan_demo"

if [ ! -f "$EXECUTABLE" ]; then
    echo "Executable $EXECUTABLE not found. Please run ./build.sh first."
    exit 1
fi

echo "===================================================="
echo "Running ThreadSanitizer Demo"
echo "===================================================="
echo "The binary has been compiled with -fsanitize=thread."
echo "TSan will monitor all memory accesses to find races."
echo "===================================================="
echo ""

setarch $(uname -m) -R $EXECUTABLE
