add_subdirectory(burnt_pancake_puzzle)
add_subdirectory(graph)
add_subdirectory(grid_pathfinding)
add_subdirectory(pancake_puzzle)
add_subdirectory(sliding_tile_puzzle)
add_subdirectory(k_ary_tree)

set(ENVIRONMENT_FILES
    # cmake-format: sortable
    ${BURNT_PANCAKE_FILES} ${GRAPH_FILES} ${GRID_PATHFINDING_FILES} ${K_ARY_TREE_FILES} ${PANCAKE_FILES}
    ${SLIDING_TILE_FILES}
    PARENT_SCOPE)
