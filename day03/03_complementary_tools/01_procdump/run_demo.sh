#!/bin/bash
# 01_procdump/run_demo.sh - Simulates a production CPU spike scenario

EXECUTABLE="./build/procdump_demo"

if [ ! -f "$EXECUTABLE" ]; then
    echo "Executable $EXECUTABLE not found. Please run ./build.sh first."
    exit 1
fi

echo "=================================================="
echo "ProcDump Memory Spike Demo (OOM Prevention)"
echo "=================================================="
echo "Starting the simulator in the background..."
$EXECUTABLE &
SIM_PID=$!

echo "Simulator started with PID: $SIM_PID"
echo ""
echo "TO CAPTURE THE MEMORY SPIKE:"
echo "1. Open another terminal."
echo "2. Run this ProcDump command:"
echo "   sudo procdump -M 512 -n 2 -s 3 -p $SIM_PID"
echo ""
echo "What this does:"
echo "-M 512  : Trigger if Memory Commit >= 512 MB"
echo "-n 2    : Capture 2 dumps (to compare growth)"
echo "-s 3    : Spike must last 3 seconds"
echo "-p      : Specify the PID"
echo "=================================================="
echo ""

# Keep script running so child doesn't die immediately on some setups, 
# and provide a way to kill it.
echo "Press Ctrl+C to stop the simulator and exit."
trap "kill $SIM_PID; exit" INT
wait $SIM_PID
