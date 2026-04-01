cmake_minimum_required(VERSION 3.28)
project(test CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

# Split DWARF for Debug
if(CMAKE_BUILD_TYPE STREQUAL "Debug" AND USE_SPLIT_DWARF)
    add_compile_options(-gsplit-dwarf)
    add_link_options(-fuse-ld=gold -Wl,--gdb-index)
endif()

add_subdirectory(Processors)
add_subdirectory(Utils)

add_executable(test 
    main.cpp
    Processors/Processor1.cpp 
    Processors/Processor2.cpp 
    Processors/Processor3.cpp 
    Processors/Processor4.cpp
    Utils/Heavy.cpp
)

target_link_libraries(test PRIVATE proc_lib utils_lib)
target_include_directories(test PRIVATE Processors Utils)
