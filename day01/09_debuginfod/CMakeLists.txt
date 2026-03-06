cmake_minimum_required(VERSION 3.28)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
project(debuginfod_demo)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)
endif()

add_executable(debuginfod_demo main.cpp)

# Build with split-DWARF so debug info is emitted into separate .dwo files.
target_compile_options(debuginfod_demo PRIVATE -ggdb3 -gsplit-dwarf -g -O2)
target_link_options(debuginfod_demo PRIVATE -Wl,--build-id=sha1)

find_program(DWP dwp HINTS /usr/bin REQUIRED)
find_program(STRIP strip HINTS /usr/bin REQUIRED)

set(SYMBOLS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/symbols")

add_custom_command(
    TARGET debuginfod_demo POST_BUILD
    # 1. Create the DWP file using dwp
    # The output is placed directly in the symbols directory
    COMMAND ${CMAKE_COMMAND} -E make_directory "${SYMBOLS_DIR}"
    COMMAND sh -c "${DWP} ${CMAKE_CURRENT_BINARY_DIR}/CMakeFiles/debuginfod_demo.dir/*.dwo -o ${SYMBOLS_DIR}/$<TARGET_FILE_NAME:debuginfod_demo>.dwp"
    
    # 2. Extract the Build-ID note from the executable
    COMMAND ${CMAKE_OBJCOPY} --dump-section .note.gnu.build-id=${CMAKE_CURRENT_BINARY_DIR}/build_id.bin $<TARGET_FILE:debuginfod_demo>
    
    # 3. Embed the Build-ID into the DWP file
    # This allows debuginfod to find the DWP by its Build-ID
    COMMAND ${CMAKE_OBJCOPY} --add-section .note.gnu.build-id=${CMAKE_CURRENT_BINARY_DIR}/build_id.bin 
                             --set-section-flags .note.gnu.build-id=alloc,contents,readonly 
                             ${SYMBOLS_DIR}/$<TARGET_FILE_NAME:debuginfod_demo>.dwp
    
    # 4. Strip the original executable to reduce size
    COMMAND ${CMAKE_STRIP} --strip-debug $<TARGET_FILE:debuginfod_demo>
    
    # Cleanup temporary Build-ID file
    COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_CURRENT_BINARY_DIR}/build_id.bin
    
    COMMENT "Packaging DWP, embedding Build-ID, and stripping executable for target debuginfod_demo"
    VERBATIM
)
