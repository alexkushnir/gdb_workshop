#!/bin/bash
# 09_vgdb/run_vgdb.sh

EXECUTABLE="./build/vgdb_demo"

if [ ! -f "$EXECUTABLE" ]; then
    echo "Executable $EXECUTABLE not found. Please run ./build.sh first."
    exit 1
fi

echo "=================================================="
echo "Starting Valgrind with vgdb support"
echo "=================================================="
echo "Valgrind will wait for GDB to connect at the first error."
echo "You can also connect during the 10s wait in main.c."
echo ""
echo "To connect, run in another terminal:"
echo "  gdb -x gdb_commands.txt $EXECUTABLE"
echo "=================================================="
echo ""

# --vgdb=yes: Enables the built-in gdbserver
# --vgdb-error=0: Stop and wait for gdb at the very first error (or immediately if you connect)
valgrind --vgdb=yes --vgdb-error=0 $EXECUTABLE
