#!/bin/bash
find . -type f \( -name "*.cpp" -o -name "*.h" \) \
       -exec clang-tidy-18 -fix-errors \
       -config-file=.clang-tidy {} \
       -- -x c++ -std=c++23 \;
