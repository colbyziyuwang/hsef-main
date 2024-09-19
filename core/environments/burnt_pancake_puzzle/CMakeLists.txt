set(BURNT_PANCAKE_FILES
    # cmake-format: sortable
    burnt_gap_heuristic.cpp
    burnt_gap_heuristic.h
    burnt_pancake_hash_function.h
    burnt_pancake_state.cpp
    burnt_pancake_state.h
    burnt_pancake_transitions.cpp
    burnt_pancake_transitions.h)

list(TRANSFORM BURNT_PANCAKE_FILES PREPEND environments/burnt_pancake_puzzle/)
set(BURNT_PANCAKE_FILES
    ${BURNT_PANCAKE_FILES}
    PARENT_SCOPE)
