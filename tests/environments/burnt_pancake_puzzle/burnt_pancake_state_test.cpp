#include <gtest/gtest.h>

#include "environments/burnt_pancake_puzzle/burnt_pancake_state.h"

/**
 * Tests BurntPancakeState on some given initial permutations.
 */
TEST(BurntPancakeStateTests, constructorTest) {
    std::vector<BurntPancake> perm{5, -4, 3, -2, -1};
    BurntPancakeState state1(perm);

    ASSERT_EQ(state1.m_permutation, perm);

    std::ostringstream state1_string;
    state1_string << state1;
    ASSERT_EQ(state1_string.str(), "[5 -4 3 -2 -1]");
    // Pancake states a and b are the same state
    BurntPancakeState pancake_state_b(perm);
    ASSERT_TRUE(state1 == pancake_state_b);
    ASSERT_FALSE(state1 != pancake_state_b);

    // Pancake states a and c are different states
    perm = {-5, -4, -3, -2, -1};
    BurntPancakeState state2(perm);
    std::ostringstream state2_string;
    state2_string << state2;
    ASSERT_EQ(state2_string.str(), "[-5 -4 -3 -2 -1]");
    ASSERT_FALSE(state1 == state2);
    ASSERT_TRUE(state1 != state2);
}