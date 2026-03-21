#!/bin/bash
# 05_mt_coredump/run_demo.sh - Enables core dumps and executes the binary

EXECUTABLE="./build/mt_coredump"

if [ ! -f "$EXECUTABLE" ]; then
    echo "Executable $EXECUTABLE not found. Please run ./build.sh first."
    exit 1
fi

echo "=================================================="
echo "Running Multithreaded Core Dump Demo"
echo "=================================================="
echo "Enabling core dumps (ulimit -c unlimited)..."
ulimit -c unlimited

# Check if the kernel is configured to write core files in the current directory
echo "Kernel core pattern: $(cat /proc/sys/kernel/core_pattern)"

echo "Starting program. It will crash in 3 seconds..."
echo "=================================================="
echo ""

$EXECUTABLE

if [ $? -ne 0 ]; then
    echo ""
    echo "=================================================="
    echo "Program crashed as expected."
    echo "If a core file was generated, you can debug it with:"
    echo "gdb $EXECUTABLE core"
    echo "=================================================="
fi
