#!/usr/bin/env bash
# run_debuginfod.sh — Start a local debuginfod server that serves the .debug
# file produced by the build, then print GDB usage instructions.
#
# Requirements:
#   - debuginfod  (part of elfutils, e.g. "sudo apt install elfutils")
#
# Usage:
#   bash run_debuginfod.sh          # defaults: build/  and port 8002
#   bash run_debuginfod.sh [dir] [port]

set -euo pipefail

PROJECT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="${1:-${PROJECT_DIR}/build}"
PORT="${2:-8002}"

if ! command -v debuginfod &>/dev/null; then
    echo "ERROR: 'debuginfod' not found."
    echo "  Install it with:  sudo apt install elfutils"
    exit 1
fi

if [[ ! -d "${BUILD_DIR}" ]]; then
    echo "ERROR: Build directory not found: ${BUILD_DIR}"
    echo "  Run build.sh first."
    exit 1
fi

echo "=========================================="
echo "  debuginfod server"
echo "  Serving: ${BUILD_DIR}"
echo "  Port:    ${PORT}"
echo "=========================================="
echo ""
echo "  In another terminal, set:"
echo "    export DEBUGINFOD_URLS=http://localhost:${PORT}"
echo ""
echo "  Then debug the stripped binary:"
echo "    gdb ${BUILD_DIR}/debuginfod_demo"
echo ""
echo "  Useful GDB commands once inside:"
echo "    (gdb) break OuterFunction"
echo "    (gdb) run"
echo "    (gdb) bt"
echo "    (gdb) info locals"
echo ""
echo "  Press Ctrl-C to stop the server."
echo "=========================================="
echo ""

# -F : scan build dir for ELF files (both binaries and .debug files)
# -v : verbose (print each request to stdout so you can watch queries live)
exec debuginfod -F "${BUILD_DIR}" -p "${PORT}" -v
