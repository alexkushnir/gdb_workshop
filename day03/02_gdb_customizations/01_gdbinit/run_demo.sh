#!/bin/bash
# 01_gdbinit/run_demo.sh - Demonstrates .gdbinit usage

EXECUTABLE="./build/gdbinit_demo"

if [ ! -f "$EXECUTABLE" ]; then
    echo "Executable $EXECUTABLE not found. Please run ./build.sh first."
    exit 1
fi

echo "=================================================="
echo "GDB .gdbinit Customization Demo"
echo "=================================================="
echo "Note: By default, GDB might not load .gdbinit from the current directory"
echo "for security reasons. We will use the -x flag to explicitly load it."
echo "=================================================="
echo ""

# We use -x to explicitly point to the config file
gdb -q -x .gdbinit $EXECUTABLE
