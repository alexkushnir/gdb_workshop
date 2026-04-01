#!/usr/bin/env bash
# day03/01_multithreading/build.sh — Builds all projects in this subdirectories
#                                   by delegating to each project's own build.sh.

set -euo pipefail

BASE_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_SCRIPT="build.sh"

echo "  Building multithreading projects in $(basename "${BASE_DIR}")..."

FAILED=()
BUILT=()

# Find all immediate subdirectories, sorted
mapfile -t PROJECTS < <(find "${BASE_DIR}" -maxdepth 1 -mindepth 1 -type d | sort)

for project_dir in "${PROJECTS[@]}"; do
    project_name="$(basename "${project_dir}")"
    build_script="${project_dir}/${BUILD_SCRIPT}"

    if [[ ! -f "${build_script}" ]]; then
        echo "    [SKIP] ${project_name} — no ${BUILD_SCRIPT} found"
        continue
    fi

    [[ ! -x "${build_script}" ]] && chmod +x "${build_script}"

    echo "    [build] ${project_name}"

    if (cd "${project_dir}" && bash "${build_script}" "$@"); then
        echo "      -> OK"
        BUILT+=("${project_name}")
    else
        echo "      -> FAILED"
        FAILED+=("${project_name}")
    fi
done

if [[ ${#BUILT[@]} -eq 0 && ${#FAILED[@]} -eq 0 ]]; then
    echo "  No projects found."
fi

if [[ ${#FAILED[@]} -gt 0 ]]; then
    echo "  Failed projects: ${FAILED[*]}"
    exit 1
fi
