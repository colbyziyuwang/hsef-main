#include <gtest/gtest.h>

#include "building_tools/hashing/permutation_hash_function.h"
#include "environments/pancake_puzzle/pancake_state.h"
#include "environments/sliding_tile_puzzle/sliding_tile_state.h"

/**
 * Tests that the state string hash function works for the sliding tile puzzle.
 */
TEST(PermutationHashFunctionTests, slidingTileTest) {
    PermutationHashFunction<SlidingTileState> hasher;

    std::vector<int> perm1{1, 3, 0, 2};
    SlidingTileState state1(perm1, 2, 2);
    uint64_t expected_hash1 = 10;

    std::vector<int> perm2{2, 3, 6, 5, 7, 4, 0, 1};
    SlidingTileState state2(perm2, 3, 3);
    uint64_t expected_hash2 = 12094;

    ASSERT_EQ(hasher.getHashValue(state1), expected_hash1);
    ASSERT_EQ(hasher.getHashValue(state2), expected_hash2);
    ASSERT_TRUE(hasher.isPerfectHashFunction());
}

/**
 * Tests that the state string hash function works for the pancake puzzle.
 */

TEST(PermutationHashFunctionTests, pancakeTest) {
    PermutationHashFunction<PancakeState> hasher;

    std::vector<int> perm1{4, 3, 2, 1, 0};
    PancakeState state1(perm1);
    uint64_t expected_hash1 = 119;

    std::vector<int> perm2{3, 1, 0, 2, 5, 4};
    PancakeState state2(perm2);
    uint64_t expected_hash2 = 385;

    ASSERT_EQ(hasher.getHashValue(state1), expected_hash1);
    ASSERT_EQ(hasher.getHashValue(state2), expected_hash2);
    ASSERT_TRUE(hasher.isPerfectHashFunction());
}

/**
 * Checks that getAllSettings returns the correct values.
 */
TEST(PermutationHashFunctionTests, getSettingsTest) {
    PermutationHashFunction<PancakeState> hasher;

    auto settings = hasher.getAllSettings();
    ASSERT_EQ(settings.m_name, PermutationHashFunction<PancakeState>::CLASS_NAME);
    ASSERT_EQ(settings.m_main_settings.size(), 0);
    ASSERT_EQ(settings.m_sub_component_settings.size(), 0);
}