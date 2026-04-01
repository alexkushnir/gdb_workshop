#!/usr/bin/env bash
# build.sh — Main build script
# Iterates over all day* subdirectories and invokes their build.sh script.

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_SCRIPT="build.sh"

echo "=== Mini Build System ==="
echo "Root: ${SCRIPT_DIR}"
echo ""

# Track overall status
FAILED=()
BUILT=()

# Find and sort all day* directories
mapfile -t DAY_DIRS < <(find "${SCRIPT_DIR}" -maxdepth 1 -type d -name "day*" | sort)

if [[ ${#DAY_DIRS[@]} -eq 0 ]]; then
    echo "No day* directories found. Nothing to build."
    exit 0
fi

for day_dir in "${DAY_DIRS[@]}"; do
    day_name="$(basename "${day_dir}")"
    build_script="${day_dir}/${BUILD_SCRIPT}"

    echo "--- Building ${day_name} ---"

    if [[ ! -f "${build_script}" ]]; then
        echo "  [SKIP] No ${BUILD_SCRIPT} found in ${day_name}"
        echo ""
        continue
    fi

    if [[ ! -x "${build_script}" ]]; then
        chmod +x "${build_script}"
    fi

    if (cd "${day_dir}" && bash "${build_script}" "$@"); then
        echo "  [OK] ${day_name} built successfully"
        BUILT+=("${day_name}")
    else
        echo "  [FAIL] ${day_name} build failed"
        FAILED+=("${day_name}")
    fi
    echo ""
done

echo "=== Build Summary ==="
echo "  Built:  ${#BUILT[@]} day(s): ${BUILT[*]:-none}"
echo "  Failed: ${#FAILED[@]} day(s): ${FAILED[*]:-none}"

if [[ ${#FAILED[@]} -gt 0 ]]; then
    exit 1
fi
