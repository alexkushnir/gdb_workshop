#!/bin/bash
if [ $# -ne 1 ]; then
    echo "Usage: $0 <PID>"
    exit 1
fi

PID="$1"

# Check if the process exists (we'll still try by name if it doesn't, but PID is preferred)
if ! kill -0 "$PID" 2>/dev/null; then
    echo "Warning: PID $PID not found. Will attempt to capture by process name: procmon_demo"
fi

# Create a temporary script with the real PID
TEMP_SCRIPT=$(mktemp /tmp/bpftrace_script.XXXXXX.bt)
sed "s/TARGET_PID/$PID/g" trace_commands.bt > "$TEMP_SCRIPT"

echo "Attaching to process $PID (and/or 'procmon_demo')..."
echo "Using processed script: $TEMP_SCRIPT"

# Run bpftrace and capture stderr to see any syntax or permission errors
if ! sudo bpftrace "$TEMP_SCRIPT"; then
    echo ""
    echo "ERROR: bpftrace failed to start or encountered a runtime issue."
    echo "Check if you have sudo privileges or if BPF is restricted on this system."
fi

# Clean up
rm -f "$TEMP_SCRIPT"
