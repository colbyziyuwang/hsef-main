add_standard_test(grid_map_test.cpp)
add_standard_test(grid_location_test.cpp)
add_standard_test(grid_pathfinding_transitions_test.cpp)
add_standard_test(grid_pathfinding_utils_test.cpp)

add_test_with_libs(grid_pathfinding_octile_heuristic_test.cpp TestHelpersLib)
add_test_with_libs(grid_pathfinding_manhattan_heuristic_test.cpp TestHelpersLib)
add_test_with_libs(grid_pathfinding_euclidean_heuristic_test.cpp TestHelpersLib)
add_test_with_libs(grid_pathfinding_lifecost_heuristic_test.cpp TestHelpersLib)
add_test_with_libs(grid_location_hash_function_test.cpp TestHelpersLib)

add_standard_test(grid_pathfinding_scenario_running_test.cpp)
target_compile_definitions(grid_pathfinding_scenario_running_test PRIVATE STRING HSEF_DIR="${PROJECT_SOURCE_DIR}")
