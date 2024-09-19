include(FetchContent)
FetchContent_Declare(
    googletest
    DOWNLOAD_EXTRACT_TIMESTAMP ON
    URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt
    ON
    CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

include(GoogleTest)

add_library(TestHelpersLib INTERFACE)
target_include_directories(TestHelpersLib INTERFACE .)
target_link_libraries(TestHelpersLib INTERFACE gtest_main HSEFLib)

include_directories(.)
add_subdirectory(utils)
add_subdirectory(environments)
add_subdirectory(engines)
add_subdirectory(experiment_running)
add_subdirectory(building_tools)
add_subdirectory(logging)
add_subdirectory(search_basics)
