#include <gtest/gtest.h>

#include "building_tools/evaluators/evaluator_tools_terms.h"
#include "building_tools/evaluators/non_goal_heuristic.h"
#include "building_tools/goal_tests/goal_tests_tools_terms.h"
#include "building_tools/goal_tests/single_state_goal_test.h"
#include "engines/engine_components/node_containers/node_list.h"
#include "environments/grid_pathfinding/grid_location.h"
#include "environments/grid_pathfinding/grid_pathfinding_action.h"
#include "environments/sliding_tile_puzzle/sliding_tile_action.h"
#include "environments/sliding_tile_puzzle/sliding_tile_state.h"
#include "search_basics/node_container.h"
#include "test_helpers.h"

/**
 * Tests default constructor of NonGoalHeuristic to see if it sets the correct
 * values. Tests using MapLocation state.
 */
TEST(NonGoalHeuristicTests, defaultConstructorTest) {
    GridLocation state1(123, 839);
    GridLocation state2(69, 420);
    GridLocation state3(901, 2048);

    SingleStateGoalTest<GridLocation> goal_test(state1);
    NonGoalHeuristic<GridLocation, GridDirection> heuristic(goal_test);

    ASSERT_TRUE(checkStateEvaluation(heuristic, state1, 0.0, false));
    ASSERT_TRUE(checkStateEvaluation(heuristic, state2, 1.0, false));
    ASSERT_TRUE(checkStateEvaluation(heuristic, state3, 1.0, false));
}

/**
 * Tests constructor with specified parameters of NonGoalHeuristic to see if it sets the correct values. Tests using
 * MapLocation state.
 */
TEST(NonGoalHeuristicTests, specifyConstructorTest) {
    GridLocation state1(123, 839), state1_copy(123, 839);
    GridLocation state2(69, 420);
    GridLocation state3(901, 2048);

    SingleStateGoalTest<GridLocation> goal_test(state1);
    NonGoalHeuristic<GridLocation, GridDirection> heuristic(goal_test, 5.0);

    ASSERT_TRUE(checkStateEvaluation(heuristic, state1_copy, 0.0, false));
    ASSERT_TRUE(checkStateEvaluation(heuristic, state2, 5.0, false));
    ASSERT_TRUE(checkStateEvaluation(heuristic, state3, 5.0, false));
}

/**
 * Tests that re-evaluate works as expected.
 */
TEST(NonGoalHeuristicTests, reEvaluateTest) {
    GridLocation state1(123, 839);
    GridLocation state2(69, 420);

    SingleStateGoalTest<GridLocation> goal_test(state1);
    NonGoalHeuristic<GridLocation, GridDirection> heuristic(goal_test, 5.0);

    NodeList<GridLocation, GridDirection> node_list;
    heuristic.setNodeContainer(node_list);

    NodeID node_id1 = node_list.addNode(state1);
    NodeID node_id2 = node_list.addNode(state2);

    heuristic.prepareToEvaluate();
    heuristic.evaluate(node_id1);

    heuristic.prepareToEvaluate();
    heuristic.evaluate(node_id2);

    // Tests re-evaluate hasn't changed anything
    ASSERT_TRUE(checkNodeEvaluation(heuristic, node_id1, 0.0, false, true));
    ASSERT_TRUE(checkNodeEvaluation(heuristic, node_id2, 5.0, false, true));

    // Manually change cached value and check it hasn't changed anything
    heuristic.setCachedEval(node_id1, 10.0);
    ASSERT_TRUE(checkNodeEvaluation(heuristic, node_id1, 10.0, false, true));
}

/**
 * Tests getAllSettings functionality for Non-Goal Heuristic
 */
TEST(NonGoalHeuristicTests, getSettingsTestForNonGoalHeuristic) {
    GridLocation goal_state(120, 120);
    SingleStateGoalTest<GridLocation> goal_test(goal_state);  // Set goal_test to nullptr

    NonGoalHeuristic<GridLocation, GridDirection> heuristic(goal_test, 2.0);

    auto settings = heuristic.getAllSettings();
    ASSERT_EQ(settings.m_name, heuristic.CLASS_NAME);
    ASSERT_EQ(settings.m_main_settings.size(), 1);
    ASSERT_EQ(settings.m_main_settings["default_h_value"], "2.0");

    ASSERT_EQ(settings.m_sub_component_settings.size(), 1);
    ASSERT_TRUE(settings.m_sub_component_settings.find(evaluatorToolsTerms::SETTING_GOAL_TEST) != settings.m_sub_component_settings.end());

    auto goal_settings = settings.m_sub_component_settings.at(evaluatorToolsTerms::SETTING_GOAL_TEST);
    ASSERT_EQ(goal_settings.m_name, SingleStateGoalTest<GridLocation>::CLASS_NAME);
    ASSERT_EQ(goal_settings.m_main_settings.at(goalTestToolsTerms::SETTING_GOAL_STATE), "(120, 120)");
    ASSERT_EQ(goal_settings.m_sub_component_settings.size(), 0);
}
