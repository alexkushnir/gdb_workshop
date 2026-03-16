cmake_minimum_required(VERSION 3.28)
project(asan_demo)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Debug)

# Fetch scnlib for modern input handling
include(FetchContent)
FetchContent_Declare(
    scnlib
    GIT_REPOSITORY https://github.com/eliaskosunen/scnlib.git
    GIT_TAG master
)
FetchContent_MakeAvailable(scnlib)

# Enable AddressSanitizer
# -fsanitize=address: The primary flag for ASan
# -fno-omit-frame-pointer: Ensures better stack traces in reports
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fno-omit-frame-pointer")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address")

add_executable(asan_demo main.cpp)
target_link_libraries(asan_demo PRIVATE scn::scn)
