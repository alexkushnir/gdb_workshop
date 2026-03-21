cmake_minimum_required(VERSION 3.28)
project(mt_coredump)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Debug)

find_package(Threads REQUIRED)

add_executable(mt_coredump main.cpp)
target_link_libraries(mt_coredump PRIVATE Threads::Threads)
