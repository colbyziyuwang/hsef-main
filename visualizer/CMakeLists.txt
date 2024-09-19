include(FetchContent)
FetchContent_Declare(
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)

# MacOS can't use this option if it is a debug build
if(APPLE AND CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(TGUI_BUILD_FRAMEWORK OFF)
endif()

# Specify the TGUI backend
set(TGUI_BACKEND
    "SFML_GRAPHICS"
    CACHE STRING "TGUI backend selection")
message("Selected TGUI Backend: ${TGUI_BACKEND}")

FetchContent_Declare(
    TGUI
    GIT_REPOSITORY https://github.com/texus/TGUI.git
    GIT_TAG 1.x)
FetchContent_MakeAvailable(TGUI)

set(GRID_PATHFINDING_VIZ_FILES # cmake-format: sortable
                               grid_pathfinding.cpp grid_pathfinding.h grid_pathfinding_colours.h)

add_library(HSEFVisLib INTERFACE)
target_include_directories(HSEFVisLib INTERFACE .)
target_link_libraries(HSEFVisLib INTERFACE HSEFLib sfml-graphics TGUI::TGUI HSEFLib)

add_executable(visualizer ${GRID_PATHFINDING_VIZ_FILES} main.cpp)
target_link_libraries(visualizer PRIVATE HSEFVisLib)

if(WIN32)
    add_custom_command(
        TARGET visualizer
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/_deps/sfml-build/lib/sfml-graphics-d-2.dll
                $<TARGET_FILE_DIR:visualizer>)

    add_custom_command(
        TARGET visualizer
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/_deps/tgui-build/lib/tgui-d.dll
                $<TARGET_FILE_DIR:visualizer>)
endif()
