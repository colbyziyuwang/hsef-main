#include <gtest/gtest.h>

#include "environments/k_ary_tree/k_ary_tree_transitions.h"

// Test case for isValidState method
TEST(KaryTransitionsTests, isValidStateTest) {
    KAryTreeTransitions transitions(3);

    std::vector<KAryTreeAction> permutation = {0, 1, 2, 3};
    KAryTreeState state(permutation);

    ASSERT_TRUE(transitions.isValidState(state));

    state.m_actions.clear();
    state.m_actions = {0, 1, 2, 3, 0};
    ASSERT_TRUE(transitions.isValidState(state));

    state.m_actions = {0, 1, 2, 3, 6};
    ASSERT_FALSE(transitions.isValidState(state));
}

// Test case for isApplicable method
TEST(KaryTransitionsTests, isApplicableTest) {
    KAryTreeTransitions transitions(3);

    std::vector<KAryTreeAction> permutation = {0, 1, 2, 3, 4, 5};
    KAryTreeState state(permutation);

    for (int action = 0; action < 3; ++action) {
        ASSERT_TRUE(transitions.isApplicable(state, action));
    }

    ASSERT_FALSE(transitions.isApplicable(state, 3));  // checking dummy
}

// Test case for getActions method
TEST(KaryTransitionsTests, getActionsTest) {
    KAryTreeTransitions transitions(3);

    std::vector<KAryTreeAction> permutation = {0, 1, 2, 3, 4, 5};
    KAryTreeState state(permutation);

    std::vector<KAryTreeAction> actions = transitions.getActions(state);
    std::vector<KAryTreeAction> expected_actions{0, 1, 2};
    ASSERT_EQ(actions, expected_actions);
}

// Test case for applyAction methods
TEST(KaryTransitionsTests, applyActionTest) {
    KAryTreeTransitions transitions(3);

    std::vector<std::vector<KAryTreeAction>> expected_tree;
    std::vector<KAryTreeAction> permutation = {0, 1, 2, 3, 4, 5, 6};
    KAryTreeState state(permutation);

    transitions.applyAction(state, 2);
    expected_tree = {{0, 1, 2, 3, 4, 5, 6, 2}};
    ASSERT_EQ(state.m_actions, expected_tree[0]);
}

// Test case for getActionCost method
TEST(KaryTransitionsTests, getActionCostsTest) {
    KAryTreeTransitions transitions(3);

    std::vector<KAryTreeAction> permutation = {0, 1, 2, 3, 4, 5};
    KAryTreeState state(permutation);

    ASSERT_EQ(transitions.getActionCost(state, 2), 1.0);
}
