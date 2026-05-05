cmake_minimum_required(VERSION 3.28)
project(ex02a LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Threads REQUIRED)
find_package(Boost REQUIRED COMPONENTS system)

add_executable(ex02a main.cpp)

target_link_libraries(ex02a PRIVATE 
    Boost::system
    Threads::Threads
)
