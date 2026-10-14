set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
project(advanced_breakpoints)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)
endif()

add_library(advanced_breakpoints_lib SHARED lib.cpp)

add_executable(advanced_breakpoints main.cpp)
target_link_libraries(advanced_breakpoints PRIVATE advanced_breakpoints_lib)

