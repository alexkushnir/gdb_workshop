cmake_minimum_required(VERSION 3.28)
project(reverse_execution)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Debug)
endif()

# We want debug symbols and no optimizations for a better GDB demo.
# GDB's 'record' feature is often more reliable when AVX is disabled on WSL.
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -mno-avx")

add_executable(reverse_execution main.cpp)
