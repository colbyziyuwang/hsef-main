add_subdirectory(building_tools)
add_subdirectory(engines)
add_subdirectory(environments)
add_subdirectory(experiment_running)
add_subdirectory(logging)
add_subdirectory(search_basics)
add_subdirectory(utils)

set(CORE_FILES
    # cmake-format: sortable
    ${BUILDING_TOOLS_FILES} ${ENGINES_FILES} ${ENVIRONMENT_FILES} ${EXPERIMENT_RUNNING_FILES} ${LOGGING_FILES}
    ${UTIL_FILES})
message("In core: ${CORE_FILES}")
add_library(HSEFLib ${CORE_FILES})
