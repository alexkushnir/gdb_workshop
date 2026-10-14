project(valgrind_demo)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Ensure debug symbols are included for Valgrind
set(CMAKE_BUILD_TYPE Debug)

#add_executable(valgrind_demo main.c)

add_executable(valgrind_demo_2 main.cpp)