set(SEARCH_BASICS_FILES # cmake-format: sortable
                        goal_test.h node_container.h node_evaluator.h search_engine.h transition_system.h)

list(TRANSFORM SEARCH_BASICS_FILES PREPEND search_basics/)
set(SEARCH_BASICS_FILES
    ${SEARCH_BASICS_FILES}
    PARENT_SCOPE)
