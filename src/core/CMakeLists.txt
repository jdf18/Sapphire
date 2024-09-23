# * Define which header files all files have access to and are able to import (all private)
set(MODULE_INCLUDE_DIRS
        ${CMAKE_CURRENT_SOURCE_DIR}/common
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# * ===== COMPILE ALL SUBMODULES AND LINK TOGETHER =====

# * btree
# Create the library for btree and include its corresponding header files
add_library(btree STATIC btree/btree.c)
target_include_directories(btree PUBLIC btree)
target_include_directories(btree PRIVATE ${MODULE_INCLUDE_DIRS})

# * database
# Create the library for database and include its corresponding header files
add_library(database STATIC database/database.c)
target_include_directories(database PUBLIC database)
target_include_directories(database PRIVATE ${MODULE_INCLUDE_DIRS})
# Database requires access to table and file libraries.
# Make public so if database is library is linked, these are also accessible
target_link_libraries(database PUBLIC table file)
# Requires access to Topaz logging library (setup in root CMakeLists)
target_link_libraries(database PRIVATE Topaz)

# * file
# Create the library for file and include its corresponding header files
add_library(file STATIC file/file.c)
target_include_directories(file PUBLIC file)
target_include_directories(file PRIVATE ${MODULE_INCLUDE_DIRS})
# Requires access to Topaz logging library (setup in root CMakeLists)
target_link_libraries(file PRIVATE Topaz)

# * pager
# Create the library for pager and include its corresponding header files
add_library(pager STATIC pager/pager.c)
target_include_directories(pager PUBLIC pager)
target_include_directories(pager PRIVATE ${MODULE_INCLUDE_DIRS})
# Database requires access to btree and file libraries.
# Make public so if database is library is linked, these are also accessible
target_link_libraries(pager PUBLIC btree file)
# Requires access to Topaz logging library (setup in root CMakeLists)
target_link_libraries(pager PRIVATE Topaz)

# * table
# Create the library for table and include its corresponding header files
add_library(table STATIC table/table.c table/cursor.c)
target_include_directories(table PUBLIC table)
target_include_directories(table PRIVATE ${MODULE_INCLUDE_DIRS})
# Requires access to Topaz logging library (setup in root CMakeLists)
target_link_libraries(table PRIVATE Topaz)

# * utils
# Create the library for utils and include its corresponding header files
add_library(utils STATIC utils/utils.c)
target_include_directories(utils PUBLIC utils)
target_include_directories(utils PRIVATE ${MODULE_INCLUDE_DIRS})

# * vm
# Create the library for vm and include its corresponding header files
add_library(vm STATIC vm/vm.c)
target_include_directories(vm PUBLIC vm)
target_include_directories(vm PRIVATE ${MODULE_INCLUDE_DIRS})
# Requires access to Topaz logging library (setup in root CMakeLists)
target_link_libraries(vm PRIVATE Topaz)

# * ===== LINK SUBMODULES TOGETHER TO CREATE CORE LIBRARY

# Create the core library
# ? core.c is empty as CMake requires at least one source per library
add_library(SapphireCore STATIC core.c)

# Link all of the submodules above to the core library
target_link_libraries(SapphireCore PRIVATE btree database file pager table utils vm)

# Connect include headers to expose as interface for the core library
target_include_directories(SapphireCore
        INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include
)