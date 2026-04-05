#!/usr/bin/env bash
# exercises/day01/ex01/build.sh
# Usage: bash build.sh [--full-build | --build-only | --clean]
#   (no args)      Full build: configure + build
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
