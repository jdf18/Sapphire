cmake_minimum_required(VERSION 3.10)
project(Sapphire)

# Set the compiler
set(CMAKE_C_COMPILER "C:/Program Files/mingw64/bin/gcc.exe")

# Define include directories
include_directories(include)

# Add source files
set(SOURCES
        src/statements.c
        src/main.c
        src/btree.c
        src/pager.c
)

# Define the executable
add_executable(main ${SOURCES})

# Specify output directory for object files and the executable
set_target_properties(main PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
        OUTPUT_NAME main
)