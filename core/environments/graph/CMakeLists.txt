set(GRAPH_FILES
    # cmake-format: sortable
    graph.cpp
    graph.h
    graph_action.cpp
    graph_action.h
    graph_state.cpp
    graph_transitions.cpp
    graph_transitions.h
    graph_utils.cpp
    graph_utils.h
    vertex_hash_function.cpp
    vertex_hash_function.h)

list(TRANSFORM GRAPH_FILES PREPEND environments/graph/)
set(GRAPH_FILES
    ${GRAPH_FILES}
    PARENT_SCOPE)
