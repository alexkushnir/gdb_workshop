cmake_minimum_required(VERSION 3.28)
project(inspect_heap)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Debug)

# Fetch scnlib for input handling
include(FetchContent)
FetchContent_Declare(
    scnlib
    GIT_REPOSITORY https://github.com/eliaskosunen/scnlib.git
    GIT_TAG master
)
FetchContent_MakeAvailable(scnlib)

add_executable(inspect_heap main.cpp)
target_link_libraries(inspect_heap PRIVATE scn::scn)
