cmake_minimum_required(VERSION 3.28)
project(gdbinit_demo)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Debug)

add_executable(gdbinit_demo main.cpp)
