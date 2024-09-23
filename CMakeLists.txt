cmake_minimum_required(VERSION 3.10)
project(Sapphire)

#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address")

#SET(CMAKE_BUILD_TYPE Debug)
#SET(CMAKE_C_FLAGS_DEBUG "-D_DEBUG")

# Define include directories for Sapphire
include_directories(include)

# * Add Topaz logging library and configure
# Set custom logging settings
add_definitions(-DLOG_FORMAT="%s - [%s] - [%s]: %s%s")
add_subdirectory(lib/Topaz)


# * ===== SAPPHIRE DB CORE =====

# Add source files and create library
set(SOURCES
        src/main.c
        src/btree.c
        src/pager.c
        src/file.c
        src/table.c
        src/cursor.c
        src/database.c
        src/vm.c
        src/ir_compiler.c
)
add_library(SapphireDB STATIC ${SOURCES})

# Link the libraries to the project
target_link_libraries(SapphireDB PRIVATE Topaz)


# * ===== SAPPHIRE REPL CONSOLE =====

# Define the testing executable
add_executable(SapphireREPL tests/test_database.c)

# Link the libraries to the project
target_link_libraries(SapphireREPL PRIVATE SapphireDB Topaz)


# Specify output directory for object files and the executable
set_target_properties(SapphireREPL PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
        OUTPUT_NAME sapphire_repl
)