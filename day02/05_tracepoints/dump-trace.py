import csv
import re
import sys
import gdb
import pdb

def _gdb_eval(name: str) -> str:
    try:
        val = gdb.parse_and_eval(name)
        return str(val)
    except gdb.error:
        return "error"


def _gdb_execute(cmd: str) -> str:
    return gdb.execute(cmd, to_string=True)


class DumpTraceVarsCommand(gdb.Command):
    """
    dump-trace-vars OUTPUT.csv

    Dumps collected variables (and trace state variables) from
    all trace frames to CSV, excluding registers and memory.
    """

    def __init__(self):
        super().__init__("dump-trace-vars", gdb.COMMAND_DATA)

    def invoke(self, arg, from_tty):
        # pdb.set_trace()
        argv = gdb.string_to_argv(arg)
        if len(argv) != 1:
            raise gdb.GdbError("usage: dump-trace-vars OUTPUT.csv")

        out_csv = argv[0]

        # Example list of variables / expressions you collected
        # Adjust this to match your `collect ...` list
        collected_vars = [
            "i",
            "processed",
        ]

        tvars = []  # adjust as per your tvars

        rows = []
        frame_count = 0

        # Start at first trace frame
        try:
            _gdb_execute("tfind start")
        except gdb.error as e:
            raise gdb.GdbError(f"cannot start at first trace frame: {e}")

        while True:
            try:
                frame = int(gdb.parse_and_eval("$trace_frame"))
            except gdb.error as Err:
                print(f"Error occurred while evaluating $trace_frame: {Err}")
                frame = frame_count

            try:
                tp = int(gdb.parse_and_eval("$tracepoint"))
            except gdb.error as Err:
                print(f"Error occurred while evaluating $tracepoint: {Err}")
                tp = -1

            try:
                pc = int(gdb.parse_and_eval("$pc"))
            except gdb.error as Err:
                print(f"Error occurred while evaluating $pc: {Err}")
                pc = 0

            # Collect explicit variables
            for name in collected_vars:
                val = _gdb_eval(name)
                rows.append({
                    "frame": frame,
                    "tracepoint": tp,
                    "kind": "var",
                    "name": name,
                    "value": val,
                    "pc": hex(pc),
                })

            # Collect trace‑state variables
            for tvar in tvars:
                val = _gdb_eval(tvar)
                rows.append({
                    "frame": frame,
                    "tracepoint": tp,
                    "kind": "tvar",
                    "name": tvar,
                    "value": val,
                    "pc": hex(pc),
                })

            frame_count += 1

            # Move to next frame
            try:
                _gdb_execute("tfind")
            except gdb.error as Err:
                print(f"Error occurred while moving to next trace frame: {Err}")
                break

            # Detect end: if $trace_frame stays the same
            try:
                new_frame = int(gdb.parse_and_eval("$trace_frame"))
                if new_frame == -1:
                    print("No more trace frames (tfind did not advance), ending collection.")
                    break
            except gdb.error as Err:
                print(f"Error occurred while checking trace frame: {Err}")
                pass

        with open(out_csv, "w", newline="", encoding="utf-8") as f:
            fieldnames = ["frame", "tracepoint", "kind", "name", "value", "pc"]
            writer = csv.DictWriter(f, fieldnames=fieldnames)
            writer.writeheader()
            writer.writerows(rows)

        print(f"Wrote {len(rows)} collected‑variable rows from {frame_count} trace frames to {out_csv}")


# Register the command with GDB
DumpTraceVarsCommand()