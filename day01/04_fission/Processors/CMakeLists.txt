add_library(proc_lib OBJECT 
    Processor1.cpp
    Processor2.cpp
    Processor3.cpp
    Processor4.cpp
)

message(STATUS "CMAKE_SOURCE_DIR: ${CMAKE_SOURCE_DIR}")
target_include_directories(proc_lib INTERFACE 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}
)
