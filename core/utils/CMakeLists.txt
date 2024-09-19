set(UTIL_FILES
    # cmake-format: sortable
    combinatorics.cpp
    combinatorics.h
    evaluator_utils.h
    floating_point_utils.cpp
    floating_point_utils.h
    io_utils.cpp
    io_utils.h
    plan_and_path_utils.h
    random_gen_utils.cpp
    random_gen_utils.h
    string_utils.cpp
    string_utils.h
    timer.cpp
    timer.h)

list(TRANSFORM UTIL_FILES PREPEND utils/)
set(UTIL_FILES
    ${UTIL_FILES}
    PARENT_SCOPE)
