cmake_minimum_required(VERSION 3.28)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
project(signals)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)
endif()

include(FetchContent)
FetchContent_Declare(
    scnlib
    GIT_REPOSITORY https://github.com/eliaskosunen/scnlib.git
    GIT_TAG master
)

FetchContent_MakeAvailable(scnlib)

add_executable(signals main.cpp)
target_link_libraries(signals PRIVATE scn::scn)
