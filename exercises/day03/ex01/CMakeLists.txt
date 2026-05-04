cmake_minimum_required(VERSION 3.28)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
project(ex01)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)
endif()
add_executable(ex01 main.cpp)

if (USE_OPTIMIZATION)
    target_compile_options(ex01 PRIVATE -O2)
endif()

if (USE_TSAN)
    target_compile_options(ex01 PRIVATE -fsanitize=thread)
    target_link_options(ex01 PRIVATE -fsanitize=thread)
endif()


