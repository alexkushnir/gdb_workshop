#!/bin/bash
# 13_ubsan/run_demo.sh - Demonstrates USBan runtime reports

EXECUTABLE="./build/ubsan_demo"

if [ ! -f "$EXECUTABLE" ]; then
    echo "Executable $EXECUTABLE not found. Please run ./build.sh first."
    exit 1
fi

echo "=================================================="
echo "UBSan Undefined Behavior Demo"
echo "=================================================="
echo "Running the program with UBSan enabled..."
echo "We set UBSAN_OPTIONS=print_stacktrace=1 for better reports."
echo "=================================================="
echo ""

# print_stacktrace=1 requires llvm-symbolizer to be in PATH for pretty traces
export UBSAN_OPTIONS=print_stacktrace=1

$EXECUTABLE
