cmake_minimum_required(VERSION 3.28)
project(deadlock_demo)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Debug)

find_package(Threads REQUIRED)

add_executable(deadlock_demo main.cpp)
target_link_libraries(deadlock_demo PRIVATE Threads::Threads)
