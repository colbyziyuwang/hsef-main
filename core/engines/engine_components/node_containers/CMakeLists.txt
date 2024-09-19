set(NODE_CONTAINERS_FILES # cmake-format: sortable
                          node_list.h)

list(TRANSFORM NODE_CONTAINERS_FILES PREPEND engines/engine_components/node_containers/)

set(NODE_CONTAINERS_FILES
    ${NODE_CONTAINERS_FILES}
    PARENT_SCOPE)
