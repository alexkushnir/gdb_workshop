cmake_minimum_required(VERSION 3.28)
project(tsan_demo)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Debug)

# Enable ThreadSanitizer (TSan)
set(TSAN_FLAGS "-fsanitize=thread -fno-omit-frame-pointer -O1")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${TSAN_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${TSAN_FLAGS}")

find_package(Threads REQUIRED)

add_executable(tsan_demo main.cpp)
target_link_libraries(tsan_demo PRIVATE Threads::Threads)
