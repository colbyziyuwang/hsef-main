#include <gtest/gtest.h>

#include "environments/grid_pathfinding/grid_location.h"
#include "environments/grid_pathfinding/grid_names.h"
#include "environments/grid_pathfinding/grid_pathfinding_euclidean_heuristic.h"
#include "test_helpers.h"
#include "utils/floating_point_utils.h"

/**
 * Tests that is dead end and is heuristic value stored is set correct when constructor is called.
 */
TEST(GridPathfindingEuclideanHeuristicTests, basicSetupTest) {
    GridLocation state(4, 4);

    GridPathfindingEuclideanHeuristic euclidean;
    euclidean.setGoalState(state);

    // Test that initial setup is done correctly
    ASSERT_FALSE(euclidean.isEvalComputed());
}

/**
 * Tests that the computation works correctly when given the goal.
 */
TEST(GridPathfindingEuclideanHeuristicTests, givenGoalTest) {
    GridLocation state(0, 0);

    GridPathfindingEuclideanHeuristic euclidean;
    euclidean.setGoalState(state);

    ASSERT_TRUE(checkStateEvaluation(euclidean, state, 0.0, false));
}

/**
 * Tests that when we perform a sequence of moves and heuristic computations, all functions return the correct stuff.
 */
TEST(GridPathfindingEuclideanHeuristicTests, cycleOfMovesTest) {
    GridLocation goal_state(0, 0);

    GridPathfindingEuclideanHeuristic euclidean;
    euclidean.setGoalState(goal_state);

    GridLocation current_state(1, 0);  // Move one loc
    ASSERT_TRUE(checkStateEvaluation(euclidean, current_state, 1.0, false));

    current_state = {1, 1};  // Move one location again
    ASSERT_TRUE(checkStateEvaluation(euclidean, current_state, ROOT_TWO, false));

    current_state = {0, 1};  // Move one location again
    ASSERT_TRUE(checkStateEvaluation(euclidean, current_state, 1.0, false));

    current_state = {0, 0};  // Move back to goal
    ASSERT_TRUE(checkStateEvaluation(euclidean, current_state, 0.0, false));
}

/**
 * Tests the euclidean distance on two arbitrary states. Correct values calculated manually.
 */
TEST(GridPathfindingEuclideanHeuristicTests, hValueDiffStatesTest) {
    GridLocation goal_state(788, 487);

    GridPathfindingEuclideanHeuristic euclidean;
    euclidean.setGoalState(goal_state);

    // First test state
    GridLocation state_a(2188, 9205);
    ASSERT_TRUE(checkStateEvaluation(euclidean, state_a, 8829.695578, false));

    // Second test state
    GridLocation state_b(8927, 2808);
    ASSERT_TRUE(checkStateEvaluation(euclidean, state_b, 8463.472219, false));

    // Third test state
    GridLocation state_c(65535, 65535);
    ASSERT_TRUE(checkStateEvaluation(euclidean, state_c, 91779.171455, false));
}

/**
 * Tests that setGoalState with state specified works properly by checking that we can update the goal and get different
 * results. Correct values calculated manually.
 */
TEST(GridPathfindingEuclideanHeuristicTests, setGoalStateSpecifyStateTest) {
    GridPathfindingEuclideanHeuristic euclidean;

    // First test state
    GridLocation goal_state_a(788, 487);
    GridLocation state_a(2188, 9205);
    euclidean.setGoalState(goal_state_a);

    ASSERT_EQ(euclidean.getGoalState(), goal_state_a);
    ASSERT_TRUE(checkStateEvaluation(euclidean, state_a, 8829.695578, false));

    // Second test state
    GridLocation goal_state_b(8927, 2808);
    GridLocation state_b(65535, 65535);
    euclidean.setGoalState(goal_state_b);

    ASSERT_EQ(euclidean.getGoalState(), goal_state_b);
    ASSERT_TRUE(checkStateEvaluation(euclidean, state_b, 84493.444675, false));
}

/**
 * Tests that setGoalState with coordinates specifiedworks properly  by checking that we can update the goal and get
 * different results. Correct values calculated manually.
 */
TEST(GridPathfindingEuclideanHeuristicTests, setGoalStateSpecifyCoordsTest) {
    GridPathfindingEuclideanHeuristic euclidean;

    // First test state
    GridLocation state_a(2188, 9205);
    euclidean.setGoalState(788, 487);

    ASSERT_EQ(euclidean.getGoalState(), GridLocation(788, 487));
    ASSERT_TRUE(checkStateEvaluation(euclidean, state_a, 8829.695578, false));

    // Second test state
    GridLocation state_b(65535, 65535);
    euclidean.setGoalState(8927, 2808);

    ASSERT_EQ(euclidean.getGoalState(), GridLocation(8927, 2808));
    ASSERT_TRUE(checkStateEvaluation(euclidean, state_b, 84493.444675, false));
}

/**
 * Tests that setGoalState with given coordinates less than the goal coordinates.
 */
TEST(GridPathfindingEuclideanHeuristicTests, goalGreaterThanGivenTest) {
    GridPathfindingEuclideanHeuristic euclidean;

    // First test state
    GridLocation state_a(2188, 9205);
    euclidean.setGoalState(9000, 9500);
    ASSERT_TRUE(checkStateEvaluation(euclidean, state_a, 6818.384632, false));
}

/**
 * Tests getAllSettings functionality
 */
TEST(GridPathfindingEuclideanHeuristicTests, getSettingsTest) {
    using namespace gridNames;
    GridLocation goal_state_a(120, 120);
    GridPathfindingEuclideanHeuristic heuristic(goal_state_a);

    auto settings = heuristic.getAllSettings();
    ASSERT_EQ(settings.m_name, GridPathfindingEuclideanHeuristic::CLASS_NAME);
    ASSERT_EQ(settings.m_main_settings.size(), 1);
    ASSERT_EQ(settings.m_main_settings[SETTING_GOAL_STATE], "(120, 120)");
    ASSERT_EQ(settings.m_sub_component_settings.size(), 0);
}