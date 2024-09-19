#include <gtest/gtest.h>

#include "building_tools/goal_tests/single_state_goal_test.h"
#include "environments/grid_pathfinding/grid_location.h"
#include "environments/sliding_tile_puzzle/sliding_tile_state.h"

/**
 * Tests setGoalState and isGoal on SlidingTileState to see if they return the intended values.
 * Also tests if the default constructor works as intended.
 */
TEST(SingleGoalTestTests, slidingTileConstructorTest) {
    SlidingTileState state1(123, 839), state2(69, 420), state3(69, 420);

    SingleStateGoalTest<SlidingTileState> single_goal(state1);
    ASSERT_TRUE(single_goal.isGoal(state1));

    single_goal.setGoalState(state2);
    ASSERT_FALSE(single_goal.isGoal(state1));

    ASSERT_TRUE(single_goal.isGoal(state3));
}

/**
 * Tests getCurrentGoal on SlidingTileState to see if they return the intended values.
 */
TEST(SingleGoalTestTests, slidingTileGetCurrentGoalTest) {
    SlidingTileState state1(123, 839), state2(69, 420), state3(69, 420);

    SingleStateGoalTest<SlidingTileState> single_goal(state1);
    ASSERT_EQ(single_goal.getGoalState(), state1);

    ASSERT_NE(single_goal.getGoalState(), state2);

    ASSERT_NE(single_goal.getGoalState(), state3);
}

/**
 * Tests setGoalState and isGoal on MapLocation to see
 * if they return the intended values.
 * Also tests if the default constructor works as intended.
 */
TEST(SingleGoalTestTests, mapLocationConstructorTest) {
    GridLocation state1(123, 839), state2(69, 420), state3(69, 420);

    SingleStateGoalTest<GridLocation> single_goal(state1);
    ASSERT_TRUE(single_goal.isGoal(state1));

    single_goal.setGoalState(state2);
    ASSERT_FALSE(single_goal.isGoal(state1));

    ASSERT_TRUE(single_goal.isGoal(state3));
}

/**
 * Tests getCurrentGoal on MapLocation to see if they return the intended values.
 */
TEST(SingleGoalTestTests, mapLocationGetCurrentGoalTest) {
    GridLocation state1(123, 839), state2(69, 420), state3(69, 420);

    SingleStateGoalTest<GridLocation> single_goal(state1);
    ASSERT_EQ(single_goal.getGoalState(), state1);

    ASSERT_NE(single_goal.getGoalState(), state2);

    ASSERT_NE(single_goal.getGoalState(), state3);
}

/**
 * Ensures getAllSettings works for single goal test.
 */
TEST(SingleGoalTestTests, mapLocationGetSettings) {
    GridLocation state(123, 839);
    SingleStateGoalTest<GridLocation> single_goal(state);

    auto settings = single_goal.getAllSettings();
    ASSERT_EQ(settings.m_name, "SingleStateGoalTest");
    ASSERT_EQ(settings.m_main_settings.size(), 1);
    ASSERT_EQ(settings.m_main_settings["goal_state"], "(123, 839)");
    ASSERT_EQ(settings.m_sub_component_settings.size(), 0);
}
