add_subdirectory(evaluators)
add_subdirectory(goal_tests)
add_subdirectory(hashing)

set(BUILDING_TOOLS_FILES
    ${EVALUATOR_BUILDING_FILES} ${GOAL_TEST_BUILDING_FILES} ${HASHING_BUILDING_FILES}
    PARENT_SCOPE)
