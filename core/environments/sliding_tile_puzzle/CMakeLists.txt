set(SLIDING_TILE_FILES
    # cmake-format: sortable
    sliding_tile_action.cpp
    sliding_tile_action.h
    sliding_tile_hash_function.h
    sliding_tile_manhattan_heuristic.cpp
    sliding_tile_manhattan_heuristic.h
    sliding_tile_names.h
    sliding_tile_state.cpp
    sliding_tile_state.h
    sliding_tile_transitions.cpp
    sliding_tile_transitions.h
    sliding_tile_utils.cpp
    sliding_tile_utils.h)

list(TRANSFORM SLIDING_TILE_FILES PREPEND environments/sliding_tile_puzzle/)
set(SLIDING_TILE_FILES
    ${SLIDING_TILE_FILES}
    PARENT_SCOPE)
