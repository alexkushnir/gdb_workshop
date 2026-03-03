cmake_minimum_required(VERSION 3.28)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
project(core_dump_exe)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE RelWithDebInfo)
endif()

add_executable(core_dump_exe main.cpp)

add_custom_command(TARGET core_dump_exe POST_BUILD
    COMMAND objcopy --only-keep-debug $<TARGET_FILE:core_dump_exe> $<TARGET_FILE:core_dump_exe>.debug
    COMMAND objcopy --strip-all $<TARGET_FILE:core_dump_exe>
    COMMAND objcopy --add-gnu-debuglink=$<TARGET_FILE:core_dump_exe>.debug $<TARGET_FILE:core_dump_exe>
    COMMENT "Extracting debug symbols, stripping, and adding debug link"
)
