add_subdirectory(eval_functions)
add_subdirectory(node_containers)
add_subdirectory(open_lists)

set(ENGINE_COMPONENTS_FILES
    ${EVAL_FUNCTIONS_FILES} ${NODE_CONTAINERS_FILES} ${OPEN_LISTS_FILES}
    PARENT_SCOPE)
