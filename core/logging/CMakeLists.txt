set(LOGGING_FILES
    # cmake-format: sortable
    experiment_results_writer.h
    logging_terms.h
    search_component_settings.h
    settings_logger.cpp
    settings_logger.h
    standard_search_statistics.cpp
    standard_search_statistics.h
    yaml_logging.cpp
    yaml_logging.h)

list(TRANSFORM LOGGING_FILES PREPEND logging/)
set(LOGGING_FILES
    ${LOGGING_FILES}
    PARENT_SCOPE)
