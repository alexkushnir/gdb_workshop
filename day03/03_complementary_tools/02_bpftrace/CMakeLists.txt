cmake_minimum_required(VERSION 3.28)
project(bpftrace_demo)

# Set C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add executable
add_executable(bpftrace_demo main.cpp)
