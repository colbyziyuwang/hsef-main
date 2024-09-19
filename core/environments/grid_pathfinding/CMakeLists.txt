set(GRID_PATHFINDING_FILES
    # cmake-format: sortable
    grid_location.cpp
    grid_location.h
    grid_location_hash_function.cpp
    grid_location_hash_function.h
    grid_map.cpp
    grid_map.h
    grid_names.h
    grid_pathfinding_action.cpp
    grid_pathfinding_action.h
    grid_pathfinding_euclidean_heuristic.cpp
    grid_pathfinding_euclidean_heuristic.h
    grid_pathfinding_lifecost_heuristic.cpp
    grid_pathfinding_lifecost_heuristic.h
    grid_pathfinding_manhattan_heuristic.cpp
    grid_pathfinding_manhattan_heuristic.h
    grid_pathfinding_octile_heuristic.cpp
    grid_pathfinding_octile_heuristic.h
    grid_pathfinding_scenario_running.cpp
    grid_pathfinding_scenario_running.h
    grid_pathfinding_transitions.cpp
    grid_pathfinding_transitions.h
    grid_pathfinding_utils.cpp
    grid_pathfinding_utils.h)

list(TRANSFORM GRID_PATHFINDING_FILES PREPEND environments/grid_pathfinding/)
set(GRID_PATHFINDING_FILES
    ${GRID_PATHFINDING_FILES}
    PARENT_SCOPE)
