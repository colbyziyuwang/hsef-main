set(GOAL_TEST_BUILDING_FILES # cmake-format: sortable
                             goal_tests_tools_terms.h multi_state_hash_based_goal_test.h single_state_goal_test.h)

list(TRANSFORM GOAL_TEST_BUILDING_FILES PREPEND building_tools/goal_tests/)
set(GOAL_TEST_BUILDING_FILES
    ${GOAL_TEST_BUILDING_FILES}
    PARENT_SCOPE)
