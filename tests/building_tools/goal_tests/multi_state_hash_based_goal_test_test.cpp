#include <cstdint>
#include <gtest/gtest.h>
#include <string>

#include "building_tools/goal_tests/multi_state_hash_based_goal_test.h"
#include "building_tools/hashing/state_string_hash_function.h"
#include "environments/grid_pathfinding/grid_location.h"
#include "environments/grid_pathfinding/grid_location_hash_function.h"
#include "environments/sliding_tile_puzzle/sliding_tile_state.h"

/**
 * Tests isGoal on HashBasedMultiGoalTest for GridLocation to see if it returns the intended values.
 */
TEST(HashBasedMultiGoalTestTests, gridLocationIsGoalTest) {
    GridLocation state1(123, 839), state2(69, 420), state3(69, 421);

    GridLocationHashFunction hasher;
    HashBasedMultiGoalTest<GridLocation, uint32_t> multi_goal(hasher, {state1, state2});

    ASSERT_TRUE(multi_goal.isGoal(state1));
    ASSERT_TRUE(multi_goal.isGoal(state2));
    ASSERT_FALSE(multi_goal.isGoal(state3));
}

/**
 * Tests isGoal on HashBasedMultiGoalTest for SlidingTileState to see if it returns the intended values.
 */
TEST(HashBasedMultiGoalTestTests, slidingTileIsGoalTest) {
    SlidingTileState state1(123, 839), state2(69, 420), state3(69, 421);

    StateStringHashFunction<SlidingTileState> hasher;

    HashBasedMultiGoalTest<SlidingTileState, std::string> multi_goal(hasher, {state1, state2});

    ASSERT_TRUE(multi_goal.isGoal(state1));
    ASSERT_TRUE(multi_goal.isGoal(state2));
    ASSERT_FALSE(multi_goal.isGoal(state3));
}

/**
 * Ensures getAllSettings works for HashBasedMultiGoalTest.
 */
TEST(HashBasedMultiGoalTestTests, getSettingsTest) {
    GridLocation state1(1, 1), state2(2, 2), state3(3, 3);
    StateStringHashFunction<GridLocation> hasher;
    HashBasedMultiGoalTest<GridLocation, std::string> multi_goal(hasher, {state1, state3});

    auto settings = multi_goal.getAllSettings();
    ASSERT_EQ(settings.m_name, multi_goal.CLASS_NAME);
    ASSERT_EQ(settings.m_main_settings.size(), 2);
    ASSERT_EQ(settings.m_main_settings["goal_state_1"], "(1, 1)");
    ASSERT_EQ(settings.m_main_settings["goal_state_2"], "(3, 3)");

    ASSERT_EQ(settings.m_sub_component_settings.size(), 1);
    ASSERT_EQ(settings.m_sub_component_settings.at("hash_function").m_name, hasher.getName());
}