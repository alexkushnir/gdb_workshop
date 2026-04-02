cmake_minimum_required(VERSION 3.28)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
project(plugins)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)
endif()

add_library(plugin SHARED plugin.cpp)
add_executable(plugins_demo main.cpp)

# Export all symbols from the executable into the dynamic symbol table.
# This is the CMake equivalent of passing -rdynamic (GCC/Clang) so that the
# shared library can resolve HostCallback() from the host process at runtime.
set_target_properties(plugins_demo PROPERTIES ENABLE_EXPORTS ON)

target_link_libraries(plugins_demo PRIVATE dl)

set_target_properties(plugin PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}"
)
