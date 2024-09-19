#include <gtest/gtest.h>

#include "environments/grid_pathfinding/grid_location.h"
#include "environments/grid_pathfinding/grid_names.h"
#include "environments/grid_pathfinding/grid_pathfinding_octile_heuristic.h"
#include "test_helpers.h"
#include "utils/floating_point_utils.h"

/**
 * Tests that is dead end and is heuristic value stored is set correct when constructor is called.
 */
TEST(GridPathfindingOctileHeuristicTests, basicSetupTest) {
    GridLocation state(4, 4);

    GridPathfindingOctileHeuristic octile;
    octile.setGoalState(state);

    ASSERT_FALSE(octile.isEvalComputed());
}

/**
 * Tests that the computation works correctly when given the goal.
 */
TEST(GridPathfindingOctileHeuristicTests, givenGoalTest) {
    GridLocation state(0, 0);

    GridPathfindingOctileHeuristic octile;
    octile.setGoalState(state);

    ASSERT_TRUE(checkDistanceToGoStateEvaluation(octile, state, 0.0, false, 0.0));
}

/**
 * Tests that when we perform a sequence of moves and heuristic computations, all functions return the correct stuff.
 */
TEST(GridPathfindingOctileHeuristicTests, cycleOfMovesTest) {
    GridLocation goal_state(0, 0);

    GridPathfindingOctileHeuristic octile;
    octile.setGoalState(goal_state);

    GridLocation current_state(1, 0);  // One loc over
    ASSERT_TRUE(checkDistanceToGoStateEvaluation(octile, current_state, 1.0, false, 1.0));

    current_state = {1, 1};  // 2 locations over
    ASSERT_TRUE(checkDistanceToGoStateEvaluation(octile, current_state, ROOT_TWO, false, 1.0));

    current_state = {0, 1};  // One loc over by y
    ASSERT_TRUE(checkDistanceToGoStateEvaluation(octile, current_state, 1.0, false, 1.0));

    current_state = {0, 0};  // Back to goal
    ASSERT_TRUE(checkDistanceToGoStateEvaluation(octile, current_state, 0.0, false, 0.0));
}

/**
 * Tests the octile distance on two arbitrary states. Correct values calculated manually.
 */
TEST(GridPathfindingOctileHeuristicTests, hValueDiffStatesTest) {
    GridLocation goal_state(788, 487);

    GridPathfindingOctileHeuristic octile;
    octile.setGoalState(goal_state);

    // First test state
    GridLocation state_a(2188, 9205);
    ASSERT_TRUE(checkDistanceToGoStateEvaluation(octile, state_a, 9297.8989868, false, 8718.0));

    // Second test state
    GridLocation state_b(8927, 2808);
    ASSERT_TRUE(checkDistanceToGoStateEvaluation(octile, state_b, 9100.389677402, false, 8139.0));

    // Third test state
    GridLocation state_c(65535, 65535);
    ASSERT_TRUE(checkDistanceToGoStateEvaluation(octile, state_c, 91867.085498814, false, 65048.0));
}

/**
 * Tests that setGoalState with state specified works properly  by checking that we can update the goal and get different
 * results. Correct values calculated manually.
 */
TEST(GridPathfindingOctileHeuristicTests, setGoalStateSpecifyStateTest) {
    GridPathfindingOctileHeuristic octile;

    // First test state
    GridLocation goal_state_a(788, 487);
    GridLocation state_a(2188, 9205);
    octile.setGoalState(goal_state_a);

    ASSERT_EQ(octile.getGoalState(), goal_state_a);
    ASSERT_TRUE(checkStateEvaluation(octile, state_a, 9297.8989868, false));

    // Second test state
    GridLocation goal_state_b(8927, 2808);
    GridLocation state_b(65535, 65535);
    octile.setGoalState(goal_state_b);

    ASSERT_EQ(octile.getGoalState(), goal_state_b);
    ASSERT_TRUE(checkStateEvaluation(octile, state_b, 86174.801317696, false));
}

/**
 * Tests that setGoalState with coordinates specified works properly by checking that we can update the goal and get
 * different results. Correct values calculated manually.
 */
TEST(GridPathfindingOctileHeuristicTests, setGoalStateSpecifyCoordsTest) {
    GridPathfindingOctileHeuristic octile;

    // First test state
    GridLocation state_a(2188, 9205);
    octile.setGoalState(788, 487);

    ASSERT_EQ(octile.getGoalState(), GridLocation(788, 487));
    ASSERT_TRUE(checkStateEvaluation(octile, state_a, 9297.8989868, false));

    // Second test state
    GridLocation state_b(65535, 65535);
    octile.setGoalState(8927, 2808);

    ASSERT_EQ(octile.getGoalState(), GridLocation(8927, 2808));
    ASSERT_TRUE(checkStateEvaluation(octile, state_b, 86174.801317696, false));
}

/**
 * Tests that SetDiagonalCost works as expected with valid/invalid.
 */
TEST(GridPathfindingOctileHeuristicTests, setDiagonalCostValidTest) {
    GridPathfindingOctileHeuristic octile;

    ASSERT_FALSE(octile.setDiagonalCost(-0.1));
    ASSERT_FALSE(octile.setDiagonalCost(-69.0));
    ASSERT_FALSE(octile.setDiagonalCost(0.0));

    ASSERT_TRUE(octile.setDiagonalCost(2.0));
    ASSERT_TRUE(octile.setDiagonalCost(123.0));
}

/**
 * Tests that SetDiagonalCost works when setting cost to 2.0.
 */
TEST(GridPathfindingOctileHeuristicTests, setDiagonalCostTest) {
    GridLocation goal_state(788, 487);
    GridPathfindingOctileHeuristic octile;
    octile.setGoalState(goal_state);

    ASSERT_TRUE(octile.setDiagonalCost(2.0));

    // First test state
    GridLocation state_a(2188, 9205);
    ASSERT_TRUE(checkStateEvaluation(octile, state_a, 10118.0, false));

    // Second test state
    GridLocation state_b(8927, 2808);
    ASSERT_TRUE(checkStateEvaluation(octile, state_b, 10460.0, false));

    // Third test state
    GridLocation state_c(65535, 65535);
    ASSERT_TRUE(checkStateEvaluation(octile, state_c, 129795.0, false));
}

/**
 * Tests that setGoalState with given coordinates less than the goal coordinates.
 */
TEST(GridPathfindingOctileHeuristicTests, goalGreaterThanGivenTest) {
    GridPathfindingOctileHeuristic octile;

    GridLocation state_a(2188, 9205);
    octile.setGoalState(9000, 9500);
    ASSERT_TRUE(checkDistanceToGoStateEvaluation(octile, state_a, 6934.19300079, false, 6812));
}

/**
 * Tests getAllSettings functionality
 */
TEST(GridPathfindingOctileHeuristicTests, getSettingsTest) {
    using namespace gridNames;
    GridLocation goal_state_a(120, 120);
    double diag_cost = 30;
    GridPathfindingOctileHeuristic heuristic(goal_state_a, diag_cost);

    auto settings = heuristic.getAllSettings();
    ASSERT_EQ(settings.m_name, GridPathfindingOctileHeuristic::CLASS_NAME);
    ASSERT_EQ(settings.m_main_settings.size(), 2);
    ASSERT_EQ(settings.m_main_settings[SETTING_GOAL_STATE], "(120, 120)");
    ASSERT_EQ(settings.m_main_settings[SETTING_DIAG_COST], "30.0");
    ASSERT_EQ(settings.m_sub_component_settings.size(), 0);
}