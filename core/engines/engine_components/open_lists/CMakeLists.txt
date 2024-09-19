set(OPEN_LISTS_FILES # cmake-format: sortable
                     evaluator_and_comparing_usage.h heap_based_open_list.h)

list(TRANSFORM OPEN_LISTS_FILES PREPEND engines/engine_components/open_lists/)

set(NODE_CONTAINERS_FILES
    ${NODE_CONTAINERS_FILES}
    PARENT_SCOPE)
