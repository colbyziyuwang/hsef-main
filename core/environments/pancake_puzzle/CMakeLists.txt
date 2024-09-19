set(PANCAKE_FILES
    # cmake-format: sortable
    gap_heuristic.cpp
    gap_heuristic.h
    pancake_action.cpp
    pancake_action.h
    pancake_hash_function.h
    pancake_names.h
    pancake_state.cpp
    pancake_state.h
    pancake_transitions.cpp
    pancake_transitions.h
    pancake_utils.cpp
    pancake_utils.h)

list(TRANSFORM PANCAKE_FILES PREPEND environments/pancake_puzzle/)
set(PANCAKE_FILES
    ${PANCAKE_FILES}
    PARENT_SCOPE)
