#!/bin/bash
# 02_procmon/run_demo.sh - Demonstrates system activity tracing with ProcMon

EXECUTABLE="./build/procmon_demo"

if [ ! -f "$EXECUTABLE" ]; then
    echo "Executable $EXECUTABLE not found. Please run ./build.sh first."
    exit 1
fi

echo "=================================================="
echo "ProcMon for Linux Demo"
echo "=================================================="
echo "Starting the simulator in the background..."
$EXECUTABLE &
SIM_PID=$!

echo "Simulator started with PID: $SIM_PID"
echo ""
echo "TO TRACE THE ACTIVITY:"
echo "1. Open another terminal."
echo "2. Run this command:"
echo "   ./run_trace.sh $SIM_PID"
echo "=================================================="
echo ""

echo "Press Ctrl+C to stop the simulator and exit."
trap "kill $SIM_PID; exit" INT
wait $SIM_PID
