#include <gtest/gtest.h>

#include "environments/pancake_puzzle/pancake_state.h"

/**
 * Tests PancakeState on some given initial permutations.
 */
TEST(PancakeStateTests, constructorTest) {
    std::vector<Pancake> perm{4, 3, 2, 1, 0};
    PancakeState pancake_state1(perm);

    ASSERT_EQ(pancake_state1.m_permutation, perm);

    std::ostringstream state1_string;
    state1_string << pancake_state1;
    ASSERT_EQ(state1_string.str(), "[4 3 2 1 0]");
    // Pancake states a and b are the same state
    PancakeState pancake_state_b(perm);
    ASSERT_TRUE(pancake_state1 == pancake_state_b);
    ASSERT_FALSE(pancake_state1 != pancake_state_b);

    // Pancake states a and c are different states
    perm = {0, 1, 2, 3, 4};
    PancakeState pancake_state2(perm);
    std::ostringstream state2_string;
    state2_string << pancake_state2;
    ASSERT_EQ(state2_string.str(), "[0 1 2 3 4]");
    ASSERT_FALSE(pancake_state1 == pancake_state2);
    ASSERT_TRUE(pancake_state1 != pancake_state2);
}