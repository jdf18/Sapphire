cmake_minimum_required(VERSION 3.10)
project(Sapphire)

#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address")

#SET(CMAKE_BUILD_TYPE Debug)
#SET(CMAKE_C_FLAGS_DEBUG "-D_DEBUG")

include(InitGitSubmodules.cmake)

# * ===== TOPAZ LOGGING =====
# * Add Topaz logging library and configure
# see https://github.com/jdf18/Topaz/

# Check the submodule is up-to-date
update_git_submodule(${CMAKE_SOURCE_DIR}/lib/Topaz)

# Set custom logging settings
add_definitions(-DLOG_FORMAT="%s - [%s] - [%s]: %s%s")

# compile the project
add_subdirectory(lib/Topaz)


# * ===== SAPPHIRE DB CORE =====
# * see /src/core/CMakeLists.txt

add_subdirectory(src/core)


# * ===== SAPPHIRE REPL CONSOLE =====
# * A basic Read Eval Print Loop for executing commands on the VM

# Define the testing executable
add_executable(SapphireREPL src/repl/repl.c)

# Link database core and logging libraries to the REPL executable
target_link_libraries(SapphireREPL PRIVATE SapphireCore Topaz)

# Specify output directory for object files and the executable
set_target_properties(SapphireREPL PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
        OUTPUT_NAME sapphire_repl
)