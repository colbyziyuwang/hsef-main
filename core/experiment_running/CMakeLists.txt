set(EXPERIMENT_RUNNING_FILES
    # cmake-format: sortable
    experiment_results.h experiment_runner.h search_resource_limits.cpp search_resource_limits.cpp)

list(TRANSFORM EXPERIMENT_RUNNING_FILES PREPEND experiment_running/)
set(EXPERIMENT_RUNNING_FILES
    ${EXPERIMENT_RUNNING_FILES}
    PARENT_SCOPE)
