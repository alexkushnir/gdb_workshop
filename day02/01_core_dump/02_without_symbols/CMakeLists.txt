cmake_minimum_required(VERSION 3.28)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
project(core_dump_no_symbols)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

add_executable(core_dump_no_symbols main.cpp)

# Build with split-DWARF so debug info is emitted into separate .dwo files.
target_compile_options(core_dump_no_symbols PRIVATE -ggdb3 -gsplit-dwarf)

# Post-build: consolidate split DWARF using `dwp`, move the consolidated
# output into the source `symbols/` directory with a `.debug` extension, then
# strip the binary and add a gnu debuglink referencing the basename.
find_program(DWP dwp REQUIRED)
set(SYMBOLS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/symbols")

add_custom_command(
    TARGET core_dump_no_symbols POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "${SYMBOLS_DIR}"
    # 1. Copy the unstripped binary to symbols/ for debuginfod to serve.
    # This provides both symbols AND the binary structure (sections) GDB needs.
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:core_dump_no_symbols>
                                     "${SYMBOLS_DIR}/$<TARGET_FILE_NAME:core_dump_no_symbols>"
    # 2. Strip the original binary for the "without symbols" demo.
    COMMAND ${CMAKE_STRIP} --strip-debug $<TARGET_FILE:core_dump_no_symbols>
    COMMENT "Saving unstripped binary to symbols/ and stripping executable"
    VERBATIM
)
