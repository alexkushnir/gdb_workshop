#!/bin/bash
find . -type f \( -name "*.cpp" -o -name "*.c" -o -name "*.h" \) -print0 | while IFS= read -r -d '' file; do
    if [[ "$file" == *.c ]]; then
        clang-tidy-18 -fix-errors -config-file=.clang-tidy "$file" -- -std=c11
    else
        # Treat .cpp and .h as C++
        clang-tidy-18 -fix-errors -config-file=.clang-tidy "$file" -- -x c++ -std=c++23
    fi
done
