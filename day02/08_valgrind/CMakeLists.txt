cmake_minimum_required(VERSION 3.10)
project(valgrind_demo)

set(CMAKE_C_STANDARD 11)

# Ensure debug symbols are included for Valgrind
set(CMAKE_BUILD_TYPE Debug)

add_executable(valgrind_demo main.c)
