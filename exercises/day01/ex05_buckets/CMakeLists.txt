cmake_minimum_required(VERSION 3.28)
project(summary_exercise LANGUAGES CXX)
 
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
 
include(FetchContent)
 
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.17.0
)
 
FetchContent_MakeAvailable(googletest)
 
enable_testing()
 
add_executable(summary_exercise tests.cpp)
add_executable(summary_exercise_prev tests_prev.cpp)

target_link_libraries(summary_exercise PRIVATE GTest::gtest_main)
target_link_libraries(summary_exercise_prev PRIVATE GTest::gtest_main)
 
include(GoogleTest)
gtest_discover_tests(summary_exercise)