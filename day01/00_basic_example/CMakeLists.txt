cmake_minimum_required(VERSION 3.28)
set(CMAKE_CXX_STANDARD 23)
project(example)

if (NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Debug)
endif()

add_executable(hello hello.cpp printer.cpp)
