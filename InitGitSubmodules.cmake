find_package(Git)

function(update_git_submodule directory)
    # Check if the source directory is the root of a git repo and that submodules are being used
    if(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git/modules")
        OPTION(GIT_SUBMODULE "Check submodules during build" ON)

        if(GIT_SUBMODULE)
            message(STATUS "Git submodules found - updating")

            # Update only the submodule at the directory specified
            execute_process(
                    COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive ${directory}
                    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                    RESULT_VARIABLE GIT_SUBMODULE_RESULT
            )

            # If any errors, print an error message
            if(NOT GIT_SUBMODULE_RESULT EQUAL "0")
                message(FATAL_ERROR "The command 'git submodule update --init --recursive ${directory}' failed with ${GIT_SUBMOD_RESULT}. Check that all submodules are updated.")
            endif()
        endif()
    endif()
endfunction()