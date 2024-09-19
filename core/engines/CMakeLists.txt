set(ENGINES_FILES # cmake-format: sortable
                  single_step_search_engine.h)

list(TRANSFORM ENGINES_FILES PREPEND engines/)

add_subdirectory(engine_components)
add_subdirectory(iterative_deepening)
add_subdirectory(best_first_search)

set(ENGINES_FILES
    ${ENGINES_FILES} ${ENGINE_COMPONENTS_FILES} ${ID_FILES} ${BFS_FILES}
    PARENT_SCOPE)
