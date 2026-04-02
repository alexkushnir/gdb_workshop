#!/usr/bin/env bash
# 09_debuginfod/build.sh
# Usage: bash build.sh [--full-build | --build-only | --clean]
#   (no args)      Full build: configure + build (also splits debug info)
#   --full-build   Delete build dir, then configure + build
#   --build-only   Skip configure, only run cmake --build
#   --clean        Delete build dir only, no build

set -euo pipefail

PROJECT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="${PROJECT_DIR}/build"

MODE="full"
for arg in "$@"; do
    case "${arg}" in
        --full-build) MODE="full-build" ;;
        --build-only) MODE="build-only" ;;
        --full)       MODE="full" ;;
        --clean)      MODE="clean" ;;
        *) echo "Unknown argument: ${arg}"; exit 1 ;;
    esac
done

if [[ "${MODE}" == "full-build" || "${MODE}" == "clean" ]]; then
    echo "  Cleaning ${BUILD_DIR}..."
    rm -rf "${BUILD_DIR}"
fi

[[ "${MODE}" == "clean" ]] && exit 0

mkdir -p "${BUILD_DIR}"

if [[ "${MODE}" != "build-only" ]]; then
    if ! cmake -S "${PROJECT_DIR}" -B "${BUILD_DIR}" \
            -DCMAKE_BUILD_TYPE=Debug \
            -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
            --log-level=ERROR -Wno-dev \
            > "${BUILD_DIR}/cmake_configure.log" 2>&1; then
        echo "--- cmake configure FAILED ---"
        cat "${BUILD_DIR}/cmake_configure.log"
        exit 1
    fi
fi

if ! cmake --build "${BUILD_DIR}" \
        > "${BUILD_DIR}/cmake_build.log" 2>&1; then
    echo "--- cmake build FAILED ---"
    cat "${BUILD_DIR}/cmake_build.log"
    exit 1
fi

# ── Print a short summary of what was produced ────────────────────────────────
BINARY="${BUILD_DIR}/debuginfod_demo"
DEBUG_FILE="${BINARY}.debug"

echo ""
echo "  Build artifacts:"
if [[ -f "${BINARY}" ]]; then
    echo "    stripped binary : ${BINARY}"
    echo "      $(file "${BINARY}")"
fi
if [[ -f "${DEBUG_FILE}" ]]; then
    echo "    debug info file : ${DEBUG_FILE}"
    echo "      $(file "${DEBUG_FILE}")"
fi
echo ""
echo "  To demo debuginfod:"
echo "    1. Start a local server:  bash ${PROJECT_DIR}/run_debuginfod.sh"
echo "    2. In another terminal:   export DEBUGINFOD_URLS=http://localhost:8002"
echo "    3. Debug stripped binary: gdb ${BINARY}"
echo "       GDB will fetch the DWARF automatically and show full source."
