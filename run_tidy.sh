#!/bin/bash
find . -type f \( -name "*.cpp" -o -name "*.c" -o -name "*.h" \) \
       -exec clang-tidy-18 -fix-errors \
       -config-file=.clang-tidy {} \
       -- -std=c++23 \;
