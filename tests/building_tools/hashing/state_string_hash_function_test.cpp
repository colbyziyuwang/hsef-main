#include <gtest/gtest.h>

#include "building_tools/hashing/state_string_hash_function.h"
#include "environments/grid_pathfinding/grid_location.h"
#include "environments/sliding_tile_puzzle/sliding_tile_state.h"

/**
 * Tests that the state string hash function works for the sliding tile puzzle.
 */
TEST(StateStringHashFunctionTests, slidingTileTest) {
    StateStringHashFunction<SlidingTileState> hasher;

    std::vector<int> perm1{11, 0, 10, 1, 9, 2, 8, 3, 7, 4, 6, 5};
    SlidingTileState state1(perm1, 3, 4);
    std::string expected_hash1 = "(3x4)-[11 0 10 1, 9 2 8 3, 7 4 6 5]";

    std::vector<int> perm2{9, 4, 5, 2, 11, 1, 8, 3, 7, 0, 6, 10};
    SlidingTileState state2(perm2, 4, 3);
    std::string expected_hash2 = "(4x3)-[9 4 5, 2 11 1, 8 3 7, 0 6 10]";

    ASSERT_EQ(hasher.getHashValue(state1), expected_hash1);
    ASSERT_EQ(hasher.getHashValue(state2), expected_hash2);
    ASSERT_TRUE(hasher.isPerfectHashFunction());
}

/**
 * Tests that the state string hash function works for the sliding tile puzzle.
 */
TEST(StateStringHashFunctionTests, mapPathfindingTest) {
    StateStringHashFunction<GridLocation> hasher;

    GridLocation state1(100, 43);
    std::string expected_hash1 = "(100, 43)";

    GridLocation state2(76, 9084);
    std::string expected_hash2 = "(76, 9084)";

    ASSERT_EQ(hasher.getHashValue(state1), expected_hash1);
    ASSERT_EQ(hasher.getHashValue(state2), expected_hash2);
    ASSERT_TRUE(hasher.isPerfectHashFunction());
}

/**
 * Checks that getAllSettings returns the correct values.
 */
TEST(StateStringHashFunctionTests, getSettingsTest) {
    StateStringHashFunction<GridLocation> hasher;

    auto settings = hasher.getAllSettings();
    ASSERT_EQ(settings.m_name, StateStringHashFunction<GridLocation>::CLASS_NAME);
    ASSERT_EQ(settings.m_main_settings.size(), 0);
    ASSERT_EQ(settings.m_sub_component_settings.size(), 0);
}
