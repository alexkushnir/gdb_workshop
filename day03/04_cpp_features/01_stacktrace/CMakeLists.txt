cmake_minimum_required(VERSION 3.28)
project(stacktrace_demo CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Ensure debug info is present. Required for std::stacktrace to provide source info.
set(CMAKE_BUILD_TYPE Debug)

add_executable(stacktrace_demo main.cpp)

# When using GCC, <stacktrace> usually requires linking against stdc++exp and backtrace
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_link_libraries(stacktrace_demo PRIVATE stdc++exp backtrace)
endif()
