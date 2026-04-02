cmake_minimum_required(VERSION 3.28)
project(remote_debugging)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Debug)
endif()

# We want debug symbols and no optimizations for a better GDB demo
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")

add_executable(remote_debugging main.cpp)
