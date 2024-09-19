#include <gtest/gtest.h>

#include "building_tools/hashing/signed_permutation_hash_function.h"
#include "environments/burnt_pancake_puzzle/burnt_pancake_state.h"

/**
 * Tests that the burnt hash function works for the burnt pancake puzzle.
 */
TEST(SignedPermutationHashFunctionTests, burntPancakeTest) {
    SignedPermutationHashFunction<BurntPancakeState> hasher;

    std::vector<int> perm1{1, 2, 3, 4};
    BurntPancakeState state1(perm1);
    uint64_t expected_hash1 = 0;

    std::vector<int> perm2{1, 3, 4, 2};
    BurntPancakeState state2(perm2);
    uint64_t expected_hash2 = 3;

    std::vector<int> perm3{1, -3, 4, 2};
    BurntPancakeState state3(perm3);
    uint64_t expected_hash3 = 99;

    std::vector<int> perm4{1, -3, -4, 2};
    BurntPancakeState state4(perm4);
    uint64_t expected_hash4 = 147;

    std::vector<int> perm5{4, 3, 2, 1};
    BurntPancakeState state5(perm5);
    uint64_t expected_hash5 = 23;

    std::vector<int> perm6{-4, -3, -2, -1};
    BurntPancakeState state6(perm6);
    uint64_t expected_hash6 = 383;


    ASSERT_EQ(hasher.getHashValue(state1), expected_hash1);
    ASSERT_EQ(hasher.getHashValue(state2), expected_hash2);
    ASSERT_EQ(hasher.getHashValue(state3), expected_hash3);
    ASSERT_EQ(hasher.getHashValue(state4), expected_hash4);
    ASSERT_EQ(hasher.getHashValue(state5), expected_hash5);
    ASSERT_EQ(hasher.getHashValue(state6), expected_hash6);
    ASSERT_TRUE(hasher.isPerfectHashFunction());
}

/**
 * Checks that getAllSettings returns the correct values.
 */
TEST(SignedPermutationHashFunctionTests, getSettingsTest) {
    SignedPermutationHashFunction<BurntPancakeState> hasher;

    auto settings = hasher.getAllSettings();
    ASSERT_EQ(settings.m_name, SignedPermutationHashFunction<BurntPancakeState>::CLASS_NAME);
    ASSERT_EQ(settings.m_main_settings.size(), 0);
    ASSERT_EQ(settings.m_sub_component_settings.size(), 0);
}