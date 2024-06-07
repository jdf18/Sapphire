cmake_minimum_required(VERSION 3.10)
project(Sapphire)

# Set the compiler
set(CMAKE_C_COMPILER "C:/Program Files/mingw64/bin/gcc.exe")

#SET(CMAKE_BUILD_TYPE Debug)
#SET(CMAKE_C_FLAGS_DEBUG "-D_DEBUG")

# Define include directories
include_directories(include)
#include_directories(src)

# Add source files
set(SOURCES
        src/statements.c
        src/main.c
        src/btree.c
        src/pager.c
        src/file.c
        src/table.c
        src/cursor.c
)

set(TEST_SOURCES
        tests/test_database.c
)

# Create the library
add_library(SapphireDB STATIC ${SOURCES})

# Define the testing executable
add_executable(TestDatabase ${TEST_SOURCES})
target_link_libraries(TestDatabase SapphireDB)

## Specify output directory for object files and the executable
set_target_properties(TestDatabase PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
        OUTPUT_NAME main
)