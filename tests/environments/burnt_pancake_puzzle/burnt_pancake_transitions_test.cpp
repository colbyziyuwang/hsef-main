#include <gtest/gtest.h>

#include "environments/burnt_pancake_puzzle/burnt_pancake_state.h"
#include "environments/burnt_pancake_puzzle/burnt_pancake_transitions.h"
#include "environments/pancake_puzzle/pancake_names.h"

/**
 * Tests BurntPancakeTransitions and isValidState on some given initial permutations.
 */
TEST(BurntPancakeTransitionsTests, isValidStateTest) {
    BurntPancakeTransitions transitions(5);

    std::vector<BurntPancake> puzzle = {1, -2, 3, -4, 5};
    BurntPancakeState state(puzzle);
    ASSERT_TRUE(transitions.isValidState(state));

    state.m_permutation = {1, 2, 3, 4, 5, 6};
    ASSERT_FALSE(transitions.isValidState(state));

    state.m_permutation = {1, 2, 3, 4};
    ASSERT_FALSE(transitions.isValidState(state));

    state.m_permutation = {1, 2, 4, -5, 6};
    ASSERT_FALSE(transitions.isValidState(state));
}

/**
 * Tests BurntPancakeTransitions and isApplicable on some given initial permutations.
 *
 * Also checks that dummy is not applicable.
 */
TEST(BurntPancakeTransitionsTests, isApplicableTest) {
    BurntPancakeTransitions transitions(5);

    std::vector<BurntPancake> puzzle{-1, 2, -3, 4, -5};
    BurntPancakeState state(puzzle);

    for (NumToFlip action = 1; action < 10; action++) {
        if (action <= 5) {
            ASSERT_TRUE(transitions.isApplicable(state, action));
        } else {
            ASSERT_FALSE(transitions.isApplicable(state, action));
        }
    }

    ASSERT_FALSE(transitions.isApplicable(state, 0));
}

/**
 * Tests BurntPancakeTransitions and getActions on some given initial permutations.
 */
TEST(BurntPancakeTransitionsTests, getActionsTest) {
    BurntPancakeTransitions transitions(5);

    std::vector<BurntPancake> puzzle{-1, -2, -3, -4, -5};
    BurntPancakeState state(puzzle);

    std::vector<NumToFlip> actions = transitions.getActions(state);
    std::vector<NumToFlip> expected_actions{1, 2, 3, 4, 5};
    ASSERT_EQ(actions, expected_actions);
}

/**
 * Tests BurntPancakeTransitions and applyAction/getInverse on some given initial permutations.
 */
TEST(BurntPancakeTransitionsTests, applyActionAndInverseTest) {
    BurntPancakeTransitions transitions(5);

    std::vector<BurntPancake> expected_state;
    std::vector<BurntPancake> puzzle{1, 2, 3, 4, 5};
    BurntPancakeState state(puzzle);

    transitions.applyAction(state, 0);
    expected_state = {1, 2, 3, 4, 5};
    ASSERT_EQ(state.m_permutation, expected_state);

    transitions.applyAction(state, 1);
    expected_state = {-1, 2, 3, 4, 5};
    ASSERT_EQ(state.m_permutation, expected_state);

    transitions.applyAction(state, 2);
    expected_state = {-2, 1, 3, 4, 5};
    ASSERT_EQ(state.m_permutation, expected_state);

    transitions.applyAction(state, 3);
    expected_state = {-3, -1, 2, 4, 5};
    ASSERT_EQ(state.m_permutation, expected_state);

    transitions.applyAction(state, 4);
    expected_state = {-4, -2, 1, 3, 5};
    ASSERT_EQ(state.m_permutation, expected_state);

    transitions.applyAction(state, 5);
    expected_state = {-5, -3, -1, 2, 4};
    ASSERT_EQ(state.m_permutation, expected_state);

    // Should get the previous state as we are applying the inverse
    transitions.applyAction(state, *transitions.getInverse(state, 5));
    expected_state = {-4, -2, 1, 3, 5};
    ASSERT_EQ(state.m_permutation, expected_state);
}

/**
 * Tests PancakeTransitions and getActionCost on some different cost types.
 */
TEST(BurntPancakeTransitionsTests, getActionCostTest) {
    BurntPancakeTransitions transitions(5);

    std::vector<BurntPancake> puzzle{2, 1, 3, 4, 0};
    BurntPancakeState state(puzzle);

    ASSERT_EQ(transitions.getActionCost(state, 2), 1.0);
    ASSERT_EQ(transitions.getActionCost(state, 3), 1.0);
    ASSERT_EQ(transitions.getActionCost(state, 4), 1.0);
    ASSERT_EQ(transitions.getActionCost(state, 5), 1.0);

    transitions.setTileCostType(PancakePuzzleCostType::heavy);
    ASSERT_EQ(transitions.getActionCost(state, 2), 2.0);
    ASSERT_EQ(transitions.getActionCost(state, 3), 3.0);
    ASSERT_EQ(transitions.getActionCost(state, 4), 4.0);
    ASSERT_EQ(transitions.getActionCost(state, 5), 2.0);

    // Try the same thing, but this time initialize cost type with constructor
    BurntPancakeTransitions transitions_2(5, PancakePuzzleCostType::heavy);
    ASSERT_EQ(transitions_2.getActionCost(state, 2), 2.0);
    ASSERT_EQ(transitions_2.getActionCost(state, 3), 3.0);
    ASSERT_EQ(transitions_2.getActionCost(state, 4), 4.0);
    ASSERT_EQ(transitions_2.getActionCost(state, 5), 2.0);

    // Test with initial negative pancakes
    std::vector<BurntPancake> puzzle_2{-3, -2, 4, -5, 1};
    BurntPancakeState state_2(puzzle);

    ASSERT_EQ(transitions.getActionCost(state_2, 2), 2.0);
    ASSERT_EQ(transitions.getActionCost(state_2, 3), 3.0);
    ASSERT_EQ(transitions.getActionCost(state_2, 4), 4.0);
    ASSERT_EQ(transitions.getActionCost(state_2, 5), 2.0);
}

/**
 * Tests getAllSettings functionality
 */
TEST(BurntPancakeTransitionsTests, getSettingsTest) {
    using namespace pancakeNames;
    PancakeTransitions transitions(5, PancakePuzzleCostType::heavy);

    auto settings = transitions.getAllSettings();
    ASSERT_EQ(settings.m_name, PancakeTransitions::CLASS_NAME);
    ASSERT_EQ(settings.m_main_settings.size(), 2);
    ASSERT_EQ(settings.m_main_settings[SETTING_COST_TYPE], COST_HEAVY);
    ASSERT_EQ(settings.m_main_settings[SETTING_STACK_SIZE], "5");
    ASSERT_EQ(settings.m_sub_component_settings.size(), 0);
}