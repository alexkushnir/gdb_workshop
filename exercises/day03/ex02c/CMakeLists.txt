cmake_minimum_required(VERSION 3.28)
project(ex02c LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(ENABLE_TSAN "Enable ThreadSanitizer" OFF)

if(ENABLE_TSAN)
    add_compile_options(-fsanitize=thread)
    add_link_options(-fsanitize=thread)
endif()

find_package(Threads REQUIRED)
find_package(Boost REQUIRED COMPONENTS system)

add_executable(ex02c main.cpp)

target_link_libraries(ex02c PRIVATE 
    Boost::system
    Threads::Threads
)
