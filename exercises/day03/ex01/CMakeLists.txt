cmake_minimum_required(VERSION 3.28)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
project(ex01)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)
endif()
add_executable(ex01 main.cpp)

if (USE_OPTIMIZATION)
    target_compile_options(ex01 PRIVATE -O2)
endif()

if (USE_TSAN)
    set(TSAN_FLAGS "-fsanitize=thread -fno-omit-frame-pointer -O1")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${TSAN_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${TSAN_FLAGS}")
    find_package(Threads REQUIRED)
    target_link_libraries(ex01 PRIVATE Threads::Threads)
endif()


