#include <gtest/gtest.h>

#include "environments/pancake_puzzle/pancake_names.h"
#include "environments/pancake_puzzle/pancake_state.h"
#include "environments/pancake_puzzle/pancake_transitions.h"

/**
 * Tests PancakeTransitions and isValidState on some given initial permutations.
 */
TEST(PancakeTransitionsTests, isValidStateTest) {
    PancakeTransitions transitions(5);

    std::vector<Pancake> puzzle = {0, 1, 2, 3, 4};
    PancakeState state(puzzle);
    ASSERT_TRUE(transitions.isValidState(state));

    state.m_permutation = {0, 1, 2, 3, 4, 5};
    ASSERT_FALSE(transitions.isValidState(state));

    state.m_permutation = {0, 1, 2, 3};
    ASSERT_FALSE(transitions.isValidState(state));

    state.m_permutation = {0, 1, 2, 3, 0};
    ASSERT_FALSE(transitions.isValidState(state));
}


/**
 * Tests PancakeTransitions and isApplicable on some given initial permutations.
 */
TEST(PancakeTransitionsTests, isApplicableTest) {
    PancakeTransitions transitions(5);

    std::vector<Pancake> puzzle{0, 1, 2, 3, 4};
    PancakeState state(puzzle);

    for (NumToFlip action = 2; action < 10; action++) {
        if (action <= 5) {
            ASSERT_TRUE(transitions.isApplicable(state, action));
        } else {
            ASSERT_FALSE(transitions.isApplicable(state, action));
        }
    }

    ASSERT_FALSE(transitions.isApplicable(state, 0));  // checking dummy
}

/**
 * Tests PancakeTransitions and getActions on some given initial permutations.
 */
TEST(PancakeTransitionsTests, getActionsTest) {
    PancakeTransitions transitions(5);

    std::vector<Pancake> puzzle{0, 1, 2, 3, 4};
    PancakeState state(puzzle);

    std::vector<NumToFlip> actions = transitions.getActions(state);
    std::vector<NumToFlip> expected_actions{2, 3, 4, 5};
    ASSERT_EQ(actions, expected_actions);
}

/**
 * Tests PancakeTransitions and applyAction/getInverse on some given initial permutations.
 */
TEST(PancakeTransitionsTests, applyActionAndInverseTest) {
    PancakeTransitions transitions(5);

    std::vector<Pancake> expected_state;
    std::vector<Pancake> puzzle{0, 1, 2, 3, 4};
    PancakeState state(puzzle);

    transitions.applyAction(state, 0);
    expected_state = {0, 1, 2, 3, 4};
    ASSERT_EQ(state.m_permutation, expected_state);

    transitions.applyAction(state, 2);
    expected_state = {1, 0, 2, 3, 4};
    ASSERT_EQ(state.m_permutation, expected_state);

    transitions.applyAction(state, 3);
    expected_state = {2, 0, 1, 3, 4};
    ASSERT_EQ(state.m_permutation, expected_state);

    transitions.applyAction(state, 4);
    expected_state = {3, 1, 0, 2, 4};
    ASSERT_EQ(state.m_permutation, expected_state);

    transitions.applyAction(state, 5);
    expected_state = {4, 2, 0, 1, 3};
    ASSERT_EQ(state.m_permutation, expected_state);

    // Should get the previous state as we are applying the inverse
    transitions.applyAction(state, *transitions.getInverse(state, 5));
    expected_state = {3, 1, 0, 2, 4};
    ASSERT_EQ(state.m_permutation, expected_state);
}

/**
 * Tests PancakeTransitions and getActionCost on some different cost types.
 */
TEST(PancakeTransitionsTests, getActionCostsTest) {
    PancakeTransitions transitions(5);

    std::vector<Pancake> puzzle{2, 1, 3, 4, 0};
    PancakeState state(puzzle);

    ASSERT_EQ(transitions.getActionCost(state, 2), 1.0);
    ASSERT_EQ(transitions.getActionCost(state, 3), 1.0);
    ASSERT_EQ(transitions.getActionCost(state, 4), 1.0);
    ASSERT_EQ(transitions.getActionCost(state, 5), 1.0);

    transitions.setTileCostType(PancakePuzzleCostType::heavy);
    ASSERT_EQ(transitions.getActionCost(state, 2), 3.0);
    ASSERT_EQ(transitions.getActionCost(state, 3), 4.0);
    ASSERT_EQ(transitions.getActionCost(state, 4), 5.0);
    ASSERT_EQ(transitions.getActionCost(state, 5), 3.0);

    // Try the same thing, but this time initialize cost type with constructor
    PancakeTransitions transitions_2(5, PancakePuzzleCostType::heavy);
    ASSERT_EQ(transitions_2.getActionCost(state, 2), 3.0);
    ASSERT_EQ(transitions_2.getActionCost(state, 3), 4.0);
    ASSERT_EQ(transitions_2.getActionCost(state, 4), 5.0);
    ASSERT_EQ(transitions_2.getActionCost(state, 5), 3.0);
}

/**
 * Tests getAllSettings functionality
 */
TEST(PancakeTransitionsTests, getSettingsTest) {
    using namespace pancakeNames;
    PancakeTransitions transitions(5, PancakePuzzleCostType::heavy);

    auto settings = transitions.getAllSettings();
    ASSERT_EQ(settings.m_name, PancakeTransitions::CLASS_NAME);
    ASSERT_EQ(settings.m_main_settings.size(), 2);
    ASSERT_EQ(settings.m_main_settings[SETTING_COST_TYPE], COST_HEAVY);
    ASSERT_EQ(settings.m_main_settings[SETTING_STACK_SIZE], "5");
    ASSERT_EQ(settings.m_sub_component_settings.size(), 0);
}
