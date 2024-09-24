cmake_minimum_required(VERSION 3.30)
project(MyC)

include(CTest)
enable_testing()

# Set the C/C++ standards
set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 11)

# Specify the directory with the source files
set(SRC_DIR "${CMAKE_SOURCE_DIR}/tests")

# Use file(GLOB ...) to get all source files in the folder
file(GLOB SRC_FILES "${SRC_DIR}/*.c")

# Loop through each source file
foreach(SRC_FILE ${SRC_FILES})
    # Get the name of the file without the extension
    get_filename_component(PROGRAM_NAME ${SRC_FILE} NAME_WE)

    # Add an executable for each source file
    add_executable(${PROGRAM_NAME} ${SRC_FILE})

    # Add executable to the tests tha ctest will run
    add_test(NAME ${PROGRAM_NAME} COMMAND ${PROGRAM_NAME})

endforeach()