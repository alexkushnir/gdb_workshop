#!/usr/bin/env bash
# run_debuginfod.sh — Start a local debuginfod server that serves the .debug
# file produced by the build, then print GDB usage instructions.
#
# Requirements:
#   - debuginfod  (part of elfutils, e.g. "sudo apt install elfutils")
#
# Usage:
#   bash run_debuginfod.sh [start|stop|status] [dir] [port]
#
# Commands:
#   start   Start the server (default)
#   stop    Stop a running server
#   status  Check if a server is running

set -euo pipefail

PROJECT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
COMMAND="${1:-start}"
BUILD_DIR="${2:-${PROJECT_DIR}/build}"
SYMBOLS_DIR="${PROJECT_DIR}/symbols"
PORT="${3:-8002}"
PID_FILE="${BUILD_DIR}/debuginfod.pid"

check_requirements() {
    if ! command -v debuginfod &>/dev/null; then
        echo "ERROR: 'debuginfod' not found."
        echo "  Install it with:  sudo apt install elfutils"
        exit 1
    fi
}

start_server() {
    if [[ -f "${PID_FILE}" ]]; then
        PID=$(cat "${PID_FILE}")
        if kill -0 "${PID}" 2>/dev/null; then
            echo "ERROR: debuginfod is already running (PID: ${PID})."
            exit 1
        fi
        rm -f "${PID_FILE}"
    fi

    if [[ ! -d "${BUILD_DIR}" ]]; then
        echo "ERROR: Build directory not found: ${BUILD_DIR}"
        echo "  Run build.sh first."
        exit 1
    fi

    echo "=========================================="
    echo "  debuginfod server: STARTING"
    echo "  Serving: ${BUILD_DIR}"
    echo "  Port:    ${PORT}"
    echo "  Symbols dir: ${SYMBOLS_DIR}"
    echo "=========================================="
    echo ""
    echo "  In another terminal, set:"
    echo "    export DEBUGINFOD_URLS=http://127.0.0.1:${PORT}"
    echo ""
    echo "  Then debug the stripped binary:"
    echo "    gdb ${BUILD_DIR}/debuginfod_demo"
    echo ""
    echo "  Press Ctrl-C to stop the server (cleanly removes PID file)."
    echo "=========================================="
    echo ""

    debuginfod -F "${SYMBOLS_DIR}" -p "${PORT}" -vvv &
    PID=$!
    echo "${PID}" > "${PID_FILE}"

    # Ensure PID file removal on script exit (Ctrl-C or normal termination)
    trap 'echo "Stopping (PID: ${PID})..."; kill "${PID}"; rm -f "${PID_FILE}"; exit 0' SIGINT SIGTERM EXIT

    wait "${PID}"
}

stop_server() {
    if [[ ! -f "${PID_FILE}" ]]; then
        echo "debuginfod is not running (no PID file found)."
        return
    fi

    PID=$(cat "${PID_FILE}")
    if kill -0 "${PID}" 2>/dev/null; then
        echo "Stopping debuginfod (PID: ${PID})..."
        kill "${PID}"
        echo "Done."
    else
        echo "Process ${PID} not found. Cleaning up stale PID file."
    fi
    rm -f "${PID_FILE}"
}

status_server() {
    if [[ -f "${PID_FILE}" ]]; then
        PID=$(cat "${PID_FILE}")
        if kill -0 "${PID}" 2>/dev/null; then
            echo "debuginfod is running (PID: ${PID})."
            return 0
        fi
        echo "Stale PID file exists (Process ${PID} not found)."
        return 1
    fi
    echo "debuginfod is not running."
    return 1
}

case "${COMMAND}" in
    start)
        check_requirements
        start_server
        ;;
    stop)
        stop_server
        ;;
    status)
        status_server
        ;;
    *)
        echo "Usage: $0 [start|stop|status] [dir] [port]"
        exit 1
        ;;
esac
