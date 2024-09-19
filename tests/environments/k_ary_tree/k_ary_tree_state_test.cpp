#include <gtest/gtest.h>

#include "environments/k_ary_tree/k_ary_tree_state.h"

// Test case for creating an empty k-ary tree state
TEST(KaryStateTests, createEmptyKaryTreeState) {
    KAryTreeState state;

    ASSERT_EQ(state.m_actions.size(), 0);
}

// Test case for creating a k-ary tree state from a permutation
TEST(KaryStateTests, createKaryTreeStateFromPermutation) {
    std::vector<KAryTreeAction> permutation = {1, 2, 3, 4, 5, 6, 7, 8};
    KAryTreeState state(permutation);

    ASSERT_EQ(state.m_actions.size(), 8);
    ASSERT_EQ(state.m_actions[0], 1);
    ASSERT_EQ(state.m_actions[1], 2);
}

// Test case for the equality operator
TEST(KaryStateTests, testKaryTreeStateEquality) {
    std::vector<KAryTreeAction> permutation = {1, 2, 3, 4, 5, 6, 7, 8};

    KAryTreeState state1(permutation);
    KAryTreeState state2(permutation);

    ASSERT_TRUE(state1 == state2);

    // Modify one action in state2
    state2.m_actions[2] = 9;

    ASSERT_FALSE(state1 == state2);
}

// Test case for the inequality operator
TEST(KaryStateTests, testKaryTreeStateInequality) {
    std::vector<KAryTreeAction> permutation1 = {1, 2, 3, 4, 5, 6, 7, 8};
    std::vector<KAryTreeAction> permutation2 = {1, 2, 3, 4, 9, 6, 7, 8};

    KAryTreeState state1(permutation1);
    KAryTreeState state2(permutation2);

    ASSERT_TRUE(state1 != state2);

    state2.m_actions[4] = 5;

    ASSERT_FALSE(state1 != state2);
}