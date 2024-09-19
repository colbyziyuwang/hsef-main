set(BFS_FILES
    # cmake-format: sortable
    a_star_epsilon.h a_star_epsilon_params.cpp a_star_epsilon_params.h best_first_search.h best_first_search_params.cpp
    best_first_search_params.h)

list(TRANSFORM BFS_FILES PREPEND engines/best_first_search/)

set(BFS_FILES
    ${BFS_FILES}
    PARENT_SCOPE)

message("In best_first_search ${BFS_FILES}")
