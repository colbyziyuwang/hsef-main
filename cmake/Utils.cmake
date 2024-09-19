# Creates executable for gtest and links provided libraries. CPP_TEST_FILE_NAME is the .cpp file with the tests in them.
# LIBS is the list of libraries to add to this test
function(add_test_with_libs CPP_TEST_FILE_NAME LIBS)
    get_filename_component(TEST_NAME ${CPP_TEST_FILE_NAME} NAME_WLE)
    add_executable(${TEST_NAME} ${CPP_TEST_FILE_NAME})

    target_link_libraries(${TEST_NAME} ${LIBS} HSEFLib gtest_main)
    gtest_discover_tests(${TEST_NAME})
endfunction()

# Creates executable for gtest. CPP_TEST_FILE_NAME is the .cpp file with the tests in them
function(add_standard_test CPP_TEST_FILE_NAME)
    add_test_with_libs(${CPP_TEST_FILE_NAME} "")
endfunction()

# Creates an HSEF executable file. CPP_MAIN_FILE is The main file to create the executable from The executable has the
# same name as the main file, just without .cpp at the end All executables have access to a macro HSEF_DIR which gives
# the root directory
function(add_hsef_exec CPP_MAIN_FILE)
    get_filename_component(EXE_NAME ${CPP_MAIN_FILE} NAME_WLE)
    add_executable(${EXE_NAME} ${CPP_MAIN_FILE})
    target_compile_definitions(${EXE_NAME} PRIVATE STRING HSEF_DIR="${PROJECT_SOURCE_DIR}")
    target_link_libraries(${EXE_NAME} HSEFLib)
endfunction()

# ############## These are helpers for Clang-tidy on Windows ############## TODO: Need to figure if this actually works

# Gets all targets
function(get_all_targets ALL_TARGETS)
    set(TARGETS)
    get_all_targets_recursive(TARGETS ${CMAKE_CURRENT_SOURCE_DIR})
    set(${ALL_TARGETS}
        ${TARGETS}
        PARENT_SCOPE)
endfunction()

# Helper function for get all targets
macro(get_all_targets_recursive TARGETS DIR)
    get_property(
        SUBDIRS
        DIRECTORY ${DIR}
        PROPERTY SUBDIRECTORIES)
    foreach(SUBDIR ${SUBDIRS})
        get_all_targets_recursive(${TARGETS} ${SUBDIR})
    endforeach()

    get_property(
        CURRENT_TARGETS
        DIRECTORY ${DIR}
        PROPERTY BUILDSYSTEM_TARGETS)
    list(APPEND ${TARGETS} ${CURRENT_TARGETS})
endmacro()
