# ######################################################################################################################
# ############### BASIC PROPERTIES
cmake_minimum_required(VERSION 3.14)
project(HSEF)
set(CMAKE_CXX_STANDARD 17)

message("\nBuilding project: ${PROJECT_NAME}")
message("Build type: ${CMAKE_BUILD_TYPE}")

message("\nProject source directory: ${PROJECT_SOURCE_DIR}")
message("Project binary directory: ${PROJECT_BINARY_DIR}")

# Prevents in-source builds
if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
    message(
        FATAL_ERROR
            "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there.\n"
    )
endif()

# Includes the settings and utils for cmake
include(cmake/StandardSettings.cmake)
include(cmake/Utils.cmake)

# ######################################################################################################################
# ############### PROJECT WARNINGS
message("\nUsing compiler warnings: ${USE_COMPILER_WARNINGS}")
# If needed, sets the project warnings by adding them to the compiler options
if(USE_COMPILER_WARNINGS)
    include(cmake/CompilerWarnings.cmake)
    get_project_warnings(PROJECT_WARNINGS)
    message("Using warnings as errors: ${WARNINGS_AS_ERRORS}")
    message("Output flags: ${PROJECT_WARNINGS}\n")
    add_compile_options(${PROJECT_WARNINGS})
endif()

# ######################################################################################################################
# ############### SETS UP INCLUDES AND ADDS MAIN DIRECTORIES
set(CMAKE_INCLUDE_CURRENT_DIR ON)
include_directories(core)

add_subdirectory(core)
add_subdirectory(apps)
add_subdirectory(tutorials)

# ######################################################################################################################
# ############### SETS UP TESTING IF NEED BE
message("Enable tests: ${ENABLE_TESTING}")
if(ENABLE_TESTING)
    enable_testing()
    add_subdirectory(tests)
endif()

# ######################################################################################################################
# ############### SETS UP CLANG-TIDY If needed, prepares clang-tidy to run
message("Using clang tidy: ${ENABLE_CLANG_TIDY}")
if(ENABLE_CLANG_TIDY)
    include(cmake/StaticAnalyzers.cmake)
endif()

if(ENABLE_CLANG_TIDY AND MSVC)
    get_all_targets(all_targets)
    message("All targets: ${all_targets}")

    # Sets up clang-tidy for Windows
    set_target_properties(${all_targets} PROPERTIES VS_GLOBAL_RunCodeAnalysis "true"
                                                    VS_GLOBAL_EnableClangTidyCodeAnalysis "true")
endif()

# ######################################################################################################################
# ############### DOXYGEN FOR DOCUMENTATION
find_package(Doxygen)
message("Using Doxygen: ${ENABLE_DOXYGEN}")
if(ENABLE_DOXYGEN)
    add_subdirectory(docs)
endif()

# ######################################################################################################################
# ############### Builds the Visualizer
message("\nBuild visualizer: ${BUILD_VISUALIZER}")
if(BUILD_VISUALIZER)
    add_subdirectory(visualizer)
endif()
