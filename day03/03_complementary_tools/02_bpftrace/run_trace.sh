if [ $# -ne 1 ]; then
	echo "Usage: $0 <PID>"
	exit 1
fi

PID="$1"

sed "s/TARGET_PID/$PID/g" trace_commands.bt | sudo bpftrace -
