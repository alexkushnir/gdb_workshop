cmake_minimum_required(VERSION 3.28)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
project(debuginfod_demo)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)
endif()

# ── Build the executable with full DWARF debug info ───────────────────────────
add_executable(debuginfod_demo main.cpp)

# Always build with -ggdb3 (rich DWARF + macro tables) regardless of build type,
# so the post-build objcopy step has something meaningful to extract.
target_compile_options(debuginfod_demo PRIVATE -ggdb3)

# ── Post-build: split debug info from the binary ─────────────────────────────
# Step 1 – copy the DWARF into <binary>.debug
# Step 2 – strip all debug sections from the binary itself
# Step 3 – add a GNU Build-ID link (.gnu_debuglink) so GDB/debuginfod can
#           match the stripped binary back to the .debug file by build-id.
# Step 4 – copy the .debug file to the source directory's symbols/ folder
add_custom_command(TARGET debuginfod_demo POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_CURRENT_SOURCE_DIR}/symbols"
    COMMAND ${CMAKE_OBJCOPY}
            --only-keep-debug
            $<TARGET_FILE:debuginfod_demo>
            $<TARGET_FILE:debuginfod_demo>.debug
    COMMAND ${CMAKE_COMMAND} -E copy
            $<TARGET_FILE:debuginfod_demo>.debug
            "${CMAKE_CURRENT_SOURCE_DIR}/symbols/"
    COMMAND ${CMAKE_OBJCOPY}
            --strip-debug
            --add-gnu-debuglink=$<TARGET_FILE:debuginfod_demo>.debug
            $<TARGET_FILE:debuginfod_demo>
    COMMENT "Splitting debug info and copying to source symbols/ dir"
    VERBATIM
)
