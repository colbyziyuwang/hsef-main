#include <gtest/gtest.h>

#include "environments/grid_pathfinding/grid_location.h"
#include "environments/grid_pathfinding/grid_names.h"
#include "environments/grid_pathfinding/grid_pathfinding_manhattan_heuristic.h"
#include "test_helpers.h"

/**
 * Tests that the computation works correctly when given the goal.
 */
TEST(GridPathfindingManhattanHeuristicTests, givenGoalTest) {
    GridLocation state(0, 0);

    GridPathfindingManhattanHeuristic manhattan;
    manhattan.setGoalState(state);

    ASSERT_TRUE(checkStateEvaluation(manhattan, state, 0.0, false));
}

/**
 * Tests that when we perform a sequence of moves and heuristic computations, all functions return the correct stuff.
 */
TEST(GridPathfindingManhattanHeuristicTests, cycleOfMovesTest) {
    GridLocation goal_state(0, 0);

    GridPathfindingManhattanHeuristic manhattan;
    manhattan.setGoalState(goal_state);

    GridLocation current_state(1, 0);  // One loc over
    ASSERT_TRUE(checkStateEvaluation(manhattan, current_state, 1.0, false));

    current_state = {1, 1};  // 2 locations over
    ASSERT_TRUE(checkStateEvaluation(manhattan, current_state, 2.0, false));

    current_state = {0, 1};  // One loc over by y
    ASSERT_TRUE(checkStateEvaluation(manhattan, current_state, 1.0, false));

    current_state = {0, 0};  // Back to goal
    ASSERT_TRUE(checkStateEvaluation(manhattan, current_state, 0.0, false));
}

/**
 * Tests the manhattan distance on two arbitrary states. Correct values calculated manually.
 */
TEST(GridPathfindingManhattanHeuristicTests, hValueDiffStatesTest) {
    GridLocation goal_state(788, 487);

    GridPathfindingManhattanHeuristic manhattan;
    manhattan.setGoalState(goal_state);

    // First test state
    GridLocation state_a(2188, 9205);
    ASSERT_TRUE(checkStateEvaluation(manhattan, state_a, 10118, false));

    // Second test state
    GridLocation state_b(8927, 2808);
    ASSERT_TRUE(checkStateEvaluation(manhattan, state_b, 10460, false));

    // Third test state
    GridLocation state_c(65535, 65535);
    ASSERT_TRUE(checkStateEvaluation(manhattan, state_c, 129795, false));
}

/**
 * Tests that setGoalState with state specified works properly by checking that we can update the goal and get different
 * results. Correct values calculated manually.
 */
TEST(GridPathfindingManhattanHeuristicTests, setGoalStateSpecifyStateTest) {
    GridPathfindingManhattanHeuristic manhattan;

    // First test state
    GridLocation goal_state_a(788, 487);
    GridLocation state_a(2188, 9205);
    manhattan.setGoalState(goal_state_a);

    ASSERT_EQ(manhattan.getGoalState(), goal_state_a);
    ASSERT_TRUE(checkStateEvaluation(manhattan, state_a, 10118, false));

    // Second test state
    GridLocation goal_state_b(8927, 2808);
    GridLocation state_b(65535, 65535);
    manhattan.setGoalState(goal_state_b);

    ASSERT_EQ(manhattan.getGoalState(), goal_state_b);
    ASSERT_TRUE(checkStateEvaluation(manhattan, state_b, 119335, false));
}

/**
 * Tests that setGoalState with coordinates specifiedworks properly by checking that we can update the goal and get different
 * results. Correct values calculated manually.
 */
TEST(GridPathfindingManhattanHeuristicTests, setGoalStateSpecifyCoordsTest) {
    GridPathfindingManhattanHeuristic manhattan;

    // First test state
    GridLocation state_a(2188, 9205);
    manhattan.setGoalState(788, 487);

    ASSERT_EQ(manhattan.getGoalState(), GridLocation(788, 487));
    ASSERT_TRUE(checkStateEvaluation(manhattan, state_a, 10118, false));

    // Second test state
    GridLocation state_b(65535, 65535);
    manhattan.setGoalState(8927, 2808);

    ASSERT_EQ(manhattan.getGoalState(), GridLocation(8927, 2808));
    ASSERT_TRUE(checkStateEvaluation(manhattan, state_b, 119335, false));
}

/**
 * Tests that setGoalState with given coordinates less than the goal coordinates.
 */
TEST(GridPathfindingManhattanHeuristicTests, goalGreaterThanGivenTest) {
    GridPathfindingManhattanHeuristic manhattan;

    // First test state
    GridLocation state_a(2188, 9205);
    manhattan.setGoalState(9000, 9500);
    ASSERT_TRUE(checkStateEvaluation(manhattan, state_a, 7107, false));
}

/**
 * Tests getAllSettings functionality
 */
TEST(GridPathfindingManhattanHeuristicTests, getSettingsTest) {
    using namespace gridNames;
    GridLocation goal_state_a(120, 120);
    GridPathfindingManhattanHeuristic heuristic(goal_state_a);

    auto settings = heuristic.getAllSettings();
    ASSERT_EQ(settings.m_name, GridPathfindingManhattanHeuristic::CLASS_NAME);
    ASSERT_EQ(settings.m_main_settings.size(), 1);
    ASSERT_EQ(settings.m_main_settings[SETTING_GOAL_STATE], "(120, 120)");
    ASSERT_EQ(settings.m_sub_component_settings.size(), 0);
}