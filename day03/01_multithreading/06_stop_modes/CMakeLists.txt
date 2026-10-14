project(stop_modes CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Ensure debug symbols are included
set(CMAKE_BUILD_TYPE Debug)

find_package(Threads REQUIRED)

add_executable(stop_modes main.cpp)
target_link_libraries(stop_modes PRIVATE Threads::Threads)
