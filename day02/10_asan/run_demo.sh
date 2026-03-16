#!/bin/bash
# 10_asan/run_demo.sh - Demonstrates AddressSanitizer report for various memory errors

EXECUTABLE="./build/asan_demo"

if [ ! -f "$EXECUTABLE" ]; then
    echo "Executable $EXECUTABLE not found. Please run ./build.sh first."
    exit 1
fi

echo "=================================================="
echo "AddressSanitizer Demo"
echo "=================================================="
echo ""

run_test() {
    local test_name=$1
    echo "--------------------------------------------------"
    echo "Running test: $test_name"
    echo "--------------------------------------------------"
    # ASAN_OPTIONS can be used to tune behavior, but defaults are usually good
    # For leaks, ASan reports them at the end of execution
    $EXECUTABLE "$test_name"
    echo ""
}

# Run tests one by one
# Note: ASan typically terminates the process on the first detected error (except for leaks)
run_test "stack"
run_test "heap"
run_test "after_free"
run_test "leak"
