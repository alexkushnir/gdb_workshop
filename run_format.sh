#!/bin/bash
find . -type f \( -name "*.cpp" -o -name "*.c" -o -name "*.h" \) \
       -exec clang-format-18 -i -style=file {} \;
