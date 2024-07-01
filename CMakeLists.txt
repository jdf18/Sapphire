cmake_minimum_required(VERSION 3.10)
project(Sapphire)

# Set the compiler
set(CMAKE_C_COMPILER "C:/Program Files/mingw64/bin/gcc.exe")

#SET(CMAKE_BUILD_TYPE Debug)
#SET(CMAKE_C_FLAGS_DEBUG "-D_DEBUG")

#include_directories(libs/Topaz/include)
# * Set custom logging settings
add_definitions(-DLOG_FORMAT="%s - [%s] - [%s]: %s%s")

# Define include directories
include_directories(include)

# Add source files
set(SOURCES
        src/main.c
        src/btree.c
        src/pager.c
        src/file.c
        src/table.c
        src/cursor.c
        src/database.c
        src/vm_executor.c
        src/vm_executor.h
        src/sql_compiler.c
        include/sql_compiler.h
        src/dynamic_arrays.h
        src/utils.c
        src/utils.h
)

set(TEST_SOURCES
        tests/test_database.c
)

# Create the library
add_library(SapphireDB STATIC ${SOURCES})

# Define the testing executable
add_executable(TestDatabase ${TEST_SOURCES})

# Link the libraries to the project
target_link_libraries(TestDatabase PRIVATE SapphireDB "C:/Users/Jacob/CLionProjects/Sapphire/libs/Topaz/lib/libTopaz.a")

## Specify output directory for object files and the executable
set_target_properties(TestDatabase PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
        OUTPUT_NAME main
)