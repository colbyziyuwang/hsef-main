#include <gtest/gtest.h>

#include "environments/grid_pathfinding/grid_location.h"
#include "environments/grid_pathfinding/grid_names.h"
#include "environments/grid_pathfinding/grid_pathfinding_lifecost_heuristic.h"
#include "test_helpers.h"

/**
 * Tests that the computation works correctly when given the goal.
 */
TEST(GridPathfindingLifecostHeuristicTests, givenGoalTest) {
    GridLocation state(0, 0);

    GridPathfindingLifecostHeuristic lifecost;
    lifecost.setGoalState(state);

    ASSERT_TRUE(checkDistanceToGoStateEvaluation(lifecost, state, 0.0, false, 0.0));
}

/**
 * Tests the lifecost distance on two arbitrary states. Correct values calculated manually.
 */
TEST(GridPathfindingLifecostHeuristicTests, hValueDiffStatesTest) {
    GridLocation goal_state(100, 2);

    GridPathfindingLifecostHeuristic lifecost;
    lifecost.setGoalState(goal_state);

    // First test state
    GridLocation state_a(0, 2);
    ASSERT_TRUE(checkDistanceToGoStateEvaluation(lifecost, state_a, 4, false, 104));

    // Second test state
    GridLocation state_b(99, 2);
    ASSERT_TRUE(checkDistanceToGoStateEvaluation(lifecost, state_b, 2, false, 1));

    // Third test state
    GridLocation state_c(200, 0);
    ASSERT_TRUE(checkDistanceToGoStateEvaluation(lifecost, state_c, 1, false, 102));

    // Third test state
    GridLocation state_d(100, 3);
    ASSERT_TRUE(checkDistanceToGoStateEvaluation(lifecost, state_d, 3, false, 1));

    // Third test state
    GridLocation state_e(101, 3);
    ASSERT_TRUE(checkDistanceToGoStateEvaluation(lifecost, state_e, 5, false, 2));

    // Start-goal and vice versa
    GridLocation state_f(10, 5);
    GridLocation state_g(12, 6);
    lifecost.setGoalState(state_g);
    ASSERT_TRUE(checkDistanceToGoStateEvaluation(lifecost, state_f, 15, false, 3));
    lifecost.setGoalState(state_f);
    ASSERT_TRUE(checkDistanceToGoStateEvaluation(lifecost, state_g, 16, false, 3));
}

/**
 * Tests that setGoalState with state specified works properly by checking that we can update the goal and get different
 * results. Correct values calculated manually.
 */
TEST(GridPathfindingLifecostHeuristicTests, setGoalStateSpecifyStateTest) {
    GridPathfindingLifecostHeuristic lifecost;

    // First test state
    GridLocation goal_state_a(100, 2);
    GridLocation state_a(0, 2);
    lifecost.setGoalState(goal_state_a);

    ASSERT_EQ(lifecost.getGoalState(), goal_state_a);
    ASSERT_TRUE(checkStateEvaluation(lifecost, state_a, 4, false));

    // Second test state
    GridLocation goal_state_b(0, 2);
    GridLocation state_b(100, 3);
    lifecost.setGoalState(goal_state_b);

    ASSERT_EQ(lifecost.getGoalState(), goal_state_b);
    ASSERT_TRUE(checkStateEvaluation(lifecost, state_b, 7, false));
}

/**
 * Tests that setGoalState with coordinates specifiedworks properly by checking that we can update the goal and get different
 * results. Correct values calculated manually.
 */
TEST(GridPathfindingLifecostHeuristicTests, setGoalStateSpecifyCoordsTest) {
    GridPathfindingLifecostHeuristic lifecost;

    // First test state
    GridLocation state_a(0, 2);
    lifecost.setGoalState(100, 2);

    ASSERT_EQ(lifecost.getGoalState(), GridLocation(100, 2));
    ASSERT_TRUE(checkStateEvaluation(lifecost, state_a, 4, false));

    // Second test state
    GridLocation state_b(100, 3);
    lifecost.setGoalState(0, 2);

    ASSERT_EQ(lifecost.getGoalState(), GridLocation(0, 2));
    ASSERT_TRUE(checkStateEvaluation(lifecost, state_b, 7, false));
}

/**
 * Tests that setGoalState with given coordinates less than the goal coordinates.
 */
TEST(GridPathfindingLifecostHeuristicTests, goalGreaterThanGivenTest) {
    GridPathfindingLifecostHeuristic lifecost;

    // First test state
    GridLocation state_a(0, 0);
    lifecost.setGoalState(9000, 9500);
    long long ans = 0;
    for (int i = 1; i < 9500; i++) {
        ans += i;
    }
    ASSERT_TRUE(checkStateEvaluation(lifecost, state_a, static_cast<double>(ans), false));
}

/**
 * Tests that the heuristic value is calculated correctly
 * with given points (x1, 5) and (x2, 6), where abs(x1 - x2) increases
 */
TEST(GridPathfindingLifecostHeuristicTests, increaseDistanceBetweenPointsTest) {
    GridPathfindingLifecostHeuristic lifecost;

    int y_coord = 6;
    GridLocation start(0, 5);

    // Distances where the right angle path is better
    lifecost.setGoalState(3, y_coord);
    ASSERT_TRUE(checkStateEvaluation(lifecost, start, 20, false));

    lifecost.setGoalState(4, y_coord);
    ASSERT_TRUE(checkStateEvaluation(lifecost, start, 25, false));

    lifecost.setGoalState(5, y_coord);
    ASSERT_TRUE(checkStateEvaluation(lifecost, start, 30, false));

    // Distances where up horizontal down path is better
    lifecost.setGoalState(6, y_coord);
    ASSERT_TRUE(checkStateEvaluation(lifecost, start, 30, false));

    lifecost.setGoalState(7, y_coord);
    ASSERT_TRUE(checkStateEvaluation(lifecost, start, 30, false));

    lifecost.setGoalState(8, y_coord);
    ASSERT_TRUE(checkStateEvaluation(lifecost, start, 30, false));

    // Swap start and goal y-coordinates
    y_coord = 5;
    start.m_x_coord = 0, start.m_y_coord = 6;

    // Distances where the right angle path is better
    lifecost.setGoalState(3, y_coord);
    ASSERT_TRUE(checkStateEvaluation(lifecost, start, 21, false));

    lifecost.setGoalState(4, y_coord);
    ASSERT_TRUE(checkStateEvaluation(lifecost, start, 26, false));

    lifecost.setGoalState(5, y_coord);
    ASSERT_TRUE(checkStateEvaluation(lifecost, start, 31, false));

    // Distances where up horizontal down path is better
    lifecost.setGoalState(6, y_coord);
    ASSERT_TRUE(checkStateEvaluation(lifecost, start, 31, false));

    lifecost.setGoalState(7, y_coord);
    ASSERT_TRUE(checkStateEvaluation(lifecost, start, 31, false));

    lifecost.setGoalState(8, y_coord);
    ASSERT_TRUE(checkStateEvaluation(lifecost, start, 31, false));
}

/**
 * Tests getAllSettings functionality
 */
TEST(GridPathfindingLifecostHeuristicTests, getSettingsTest) {
    using namespace gridNames;
    GridLocation goal_state_a(120, 120);
    GridPathfindingLifecostHeuristic heuristic(goal_state_a);

    auto settings = heuristic.getAllSettings();
    ASSERT_EQ(settings.m_name, GridPathfindingLifecostHeuristic::CLASS_NAME);
    ASSERT_EQ(settings.m_main_settings.size(), 1);
    ASSERT_EQ(settings.m_main_settings[SETTING_GOAL_STATE], "(120, 120)");
    ASSERT_EQ(settings.m_sub_component_settings.size(), 0);
}