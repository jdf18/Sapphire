set(MODULE_INCLUDE_DIRS
        ${CMAKE_CURRENT_SOURCE_DIR}/common
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# * ===== COMPILE ALL SUBMODULES AND LINK TOGETHER =====

# * btree
add_library(btree STATIC btree/btree.c)
target_include_directories(btree PUBLIC btree)
target_include_directories(btree PRIVATE ${MODULE_INCLUDE_DIRS})

# * database
add_library(database STATIC database/database.c)
target_include_directories(database PUBLIC database)
target_include_directories(database PRIVATE ${MODULE_INCLUDE_DIRS})
target_link_libraries(database PUBLIC table file)
target_link_libraries(database PRIVATE Topaz)

# * file
add_library(file STATIC file/file.c)
target_include_directories(file PUBLIC file)
target_include_directories(file PRIVATE ${MODULE_INCLUDE_DIRS})
target_link_libraries(file PRIVATE Topaz)

# * pager
add_library(pager STATIC pager/pager.c)
target_include_directories(pager PUBLIC pager)
target_include_directories(pager PRIVATE ${MODULE_INCLUDE_DIRS})
target_link_libraries(pager PUBLIC btree file)
target_link_libraries(pager PRIVATE Topaz)

# * table
add_library(table STATIC table/table.c table/cursor.c)
target_include_directories(table PUBLIC table)
target_include_directories(table PRIVATE ${MODULE_INCLUDE_DIRS})
target_link_libraries(table PRIVATE Topaz)

# * utils
add_library(utils STATIC utils/utils.c)
target_include_directories(utils PUBLIC utils)
target_include_directories(utils PRIVATE ${MODULE_INCLUDE_DIRS})

# * vm
add_library(vm STATIC vm/vm.c)
target_include_directories(vm PUBLIC vm)
target_include_directories(vm PRIVATE ${MODULE_INCLUDE_DIRS})
target_link_libraries(vm PRIVATE Topaz)

# * ===== LINK SUBMODULES TOGETHER TO CREATE CORE LIBRARY

# Create the core library
add_library(SapphireCore STATIC core.c)

# Target libraries to link to SapphireCore
target_link_libraries(SapphireCore PRIVATE btree database file pager table utils vm)

# Add the shared headers directory for all core libraries
# All core modules will have access to the headers in /src/core/common and /src/core/include
target_include_directories(SapphireCore
        PRIVATE ${MODULE_INCLUDE_DIRS}                  # Path to the common headers
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include      # Include headers and expose as interface for the core library
)