project(ubsan_demo)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Debug)

# Enable UBSan
add_compile_options(-fsanitize=undefined -fno-omit-frame-pointer)
add_link_options(-fsanitize=undefined)

add_executable(ubsan_demo main.cpp)
