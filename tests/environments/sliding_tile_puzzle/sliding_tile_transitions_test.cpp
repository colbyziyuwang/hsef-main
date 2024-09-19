#include <gtest/gtest.h>

#include "environments/sliding_tile_puzzle/sliding_tile_names.h"
#include "environments/sliding_tile_puzzle/sliding_tile_state.h"
#include "environments/sliding_tile_puzzle/sliding_tile_transitions.h"

/**
 * Tests SlidingTileTransitions (with SlidingTileCostType specified) functions with 
 * a 4x4 puzzle with the blank in row 1, column 1.
 */
TEST(SlidingTileTransitionsTests, slidingTileTransitionsGeneralTest) {
    std::vector<BlankSlide> actions;
    std::vector<Tile> perm{1, 2, 3, 4, 5, 0, 7, 8, 9, 10, 11, 6, 12, 13, 14, 15};
    SlidingTileState tile_puzzle_state(perm, 4, 4);

    SlidingTileTransitions tile_transitions(4, 4, SlidingTileCostType::unit);

    ASSERT_EQ(tile_puzzle_state.m_blank_loc, 5);  // Check to make sure we have correct initial state

    ASSERT_TRUE(tile_transitions.isApplicable(tile_puzzle_state, BlankSlide::up));
    actions = tile_transitions.getActions(tile_puzzle_state);
    ASSERT_EQ(actions.size(), 4);  // Should add all possible actions (4)
    ASSERT_EQ(actions[0], BlankSlide::up);
    ASSERT_EQ(actions[1], BlankSlide::right);
    ASSERT_EQ(actions[2], BlankSlide::down);
    ASSERT_EQ(actions[3], BlankSlide::left);

    ASSERT_TRUE(tile_transitions.getInverse(tile_puzzle_state, BlankSlide::up).has_value());
    ASSERT_DOUBLE_EQ(tile_transitions.getActionCost(tile_puzzle_state, BlankSlide::up), 1.0);
    tile_transitions.applyAction(tile_puzzle_state, BlankSlide::up);
    ASSERT_EQ(tile_puzzle_state.m_blank_loc, 1);  // Check to make sure we got right next state
    std::vector<Tile> expected_state{1, 0, 3, 4, 5, 2, 7, 8, 9, 10, 11, 6, 12, 13, 14, 15};
    ASSERT_EQ(tile_puzzle_state.m_permutation, expected_state);

    ASSERT_FALSE(tile_transitions.isApplicable(tile_puzzle_state, BlankSlide::up));
    actions = tile_transitions.getActions(tile_puzzle_state);
    ASSERT_EQ(actions.size(), 3);  // Should add 3 more actions
    ASSERT_EQ(actions[0], BlankSlide::right);
    ASSERT_EQ(actions[1], BlankSlide::down);
    ASSERT_EQ(actions[2], BlankSlide::left);

    ASSERT_TRUE(tile_transitions.isApplicable(tile_puzzle_state, BlankSlide::left));
    ASSERT_TRUE(tile_transitions.getInverse(tile_puzzle_state, BlankSlide::left).has_value());
    ASSERT_DOUBLE_EQ(tile_transitions.getActionCost(tile_puzzle_state, BlankSlide::left), 1.0);

    tile_transitions.applyAction(tile_puzzle_state, BlankSlide::left);
    ASSERT_EQ(tile_puzzle_state.m_blank_loc, 0);
    expected_state = {0, 1, 3, 4, 5, 2, 7, 8, 9, 10, 11, 6, 12, 13, 14, 15};
    ASSERT_EQ(tile_puzzle_state.m_permutation, expected_state);

    ASSERT_FALSE(tile_transitions.isApplicable(tile_puzzle_state, BlankSlide::left));
    actions = tile_transitions.getActions(tile_puzzle_state);
    ASSERT_EQ(actions.size(), 2);
    ASSERT_EQ(actions[0], BlankSlide::right);
    ASSERT_EQ(actions[1], BlankSlide::down);
}

/**
 * Tests getInverse to see if it returns the correct inverse action.
 */
TEST(SlidingTileTransitionsTests, slidingTileTransitionsInverseTest) {
    SlidingTileState tile_puzzle_state;
    SlidingTileTransitions tile_transitions(4, 4, SlidingTileCostType::unit);

    ASSERT_EQ(tile_transitions.getInverse(tile_puzzle_state, BlankSlide::left), BlankSlide::right);
    ASSERT_EQ(tile_transitions.getInverse(tile_puzzle_state, BlankSlide::right), BlankSlide::left);
    ASSERT_EQ(tile_transitions.getInverse(tile_puzzle_state, BlankSlide::up), BlankSlide::down);
    ASSERT_EQ(tile_transitions.getInverse(tile_puzzle_state, BlankSlide::down), BlankSlide::up);
}

/**
 * Tests isValidState to see if the given state is the state represented in the 
 * SlidingTileTransitions.
 */
TEST(SlidingTileTransitionsTests, slidingTileTransitionsIsValidTest) {
    SlidingTileTransitions transitions(4, 4, SlidingTileCostType::unit);
    SlidingTileState empty_state;

    ASSERT_FALSE(transitions.isValidState(empty_state));

    SlidingTileState actual_state_4x4(4, 4);
    ASSERT_TRUE(transitions.isValidState(actual_state_4x4));

    transitions.setPuzzleDimensions(10, 10);
    ASSERT_FALSE(transitions.isValidState(actual_state_4x4));

    transitions.setPuzzleDimensions(4, 4);
    actual_state_4x4.m_permutation = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 9, 11, 12, 13, 14, 15};
    ASSERT_FALSE(transitions.isValidState(actual_state_4x4));
}

/**
 * Tests setCostType with the different tile cost types to see if they are
 * giving the correct values. For SlidingTileCostType::unit, it tests all of the tiles.
 * For the other tests, we only test a couple of tiles.
 */
TEST(SlidingTileTransitionsTests, slidingTileTransitionsTileCostsTest) {
    SlidingTileState tile_puzzle_state(4, 4);
    SlidingTileTransitions tile_transitions(4, 4, SlidingTileCostType::unit);

    bool going_right = true;
    BlankSlide direction = BlankSlide::right;
    tile_transitions.setCostType(SlidingTileCostType::unit);
    for (Tile tile_num = 1; tile_num <= 16; tile_num++) {
        if (tile_transitions.isApplicable(tile_puzzle_state, BlankSlide::up)) {
            ASSERT_DOUBLE_EQ(tile_transitions.getActionCost(tile_puzzle_state, BlankSlide::up), 1.0);
        }
        if (tile_transitions.isApplicable(tile_puzzle_state, BlankSlide::down)) {
            ASSERT_DOUBLE_EQ(tile_transitions.getActionCost(tile_puzzle_state, BlankSlide::down), 1.0);
        }
        if (tile_transitions.isApplicable(tile_puzzle_state, BlankSlide::left)) {
            ASSERT_DOUBLE_EQ(tile_transitions.getActionCost(tile_puzzle_state, BlankSlide::left), 1.0);
        }
        if (tile_transitions.isApplicable(tile_puzzle_state, BlankSlide::right)) {
            ASSERT_DOUBLE_EQ(tile_transitions.getActionCost(tile_puzzle_state, BlankSlide::right), 1.0);
        }

        if (tile_num % 4 == 0) {
            direction = BlankSlide::down;
            going_right = !going_right;
        } else if (going_right) {
            direction = BlankSlide::right;
        } else {
            direction = BlankSlide::left;
        }

        if (tile_transitions.isApplicable(tile_puzzle_state, direction)) {
            tile_transitions.applyAction(tile_puzzle_state, direction);
        }
    }

    tile_transitions.setCostType(SlidingTileCostType::heavy);
    SlidingTileState tile_puzzle_state_b(4, 4);
    ASSERT_EQ(tile_transitions.getActionCost(tile_puzzle_state_b, BlankSlide::down), 4);
    ASSERT_EQ(tile_transitions.getActionCost(tile_puzzle_state_b, BlankSlide::right), 1);

    tile_transitions.setCostType(SlidingTileCostType::inverse);
    ASSERT_DOUBLE_EQ(tile_transitions.getActionCost(tile_puzzle_state_b, BlankSlide::down), 1.0 / 4);
    ASSERT_DOUBLE_EQ(tile_transitions.getActionCost(tile_puzzle_state_b, BlankSlide::right), 1.0 / 1);
}

/**
 * Tests SlidingTileTransitions in the assymetric cases of a 3x4 and 4x3 puzzle.
 * Tests the functions by initializing both states with the same permutation and
 * moving the blank to the corner of one but not the
 * other and checking if the actions are valid.
 */
TEST(SlidingTileTransitionsTests, slidingTileTransitionsAssymetricTest) {
    std::vector<BlankSlide> actions;

    SlidingTileTransitions tile_transitions_3x4(3, 4, SlidingTileCostType::unit);
    SlidingTileTransitions tile_transitions_4x3(4, 3, SlidingTileCostType::unit);

    std::vector<Tile> perm{1, 2, 3, 4, 5, 0, 7, 8, 9, 10, 11, 6};
    SlidingTileState tile_puzzle_state_3x4(perm, 3, 4);
    SlidingTileState tile_puzzle_state_4x3(perm, 4, 3);

    ASSERT_TRUE(tile_transitions_3x4.isApplicable(tile_puzzle_state_3x4, BlankSlide::up));
    ASSERT_TRUE(tile_transitions_4x3.isApplicable(tile_puzzle_state_4x3, BlankSlide::up));

    std::vector<BlankSlide> expected_actions{BlankSlide::up, BlankSlide::right,
              BlankSlide::down, BlankSlide::left};
    actions = tile_transitions_3x4.getActions(tile_puzzle_state_3x4);
    ASSERT_EQ(actions, expected_actions);

    expected_actions = {BlankSlide::up, BlankSlide::down, BlankSlide::left};
    actions = tile_transitions_4x3.getActions(tile_puzzle_state_4x3);
    ASSERT_EQ(actions, expected_actions);

    tile_transitions_3x4.applyAction(tile_puzzle_state_3x4, BlankSlide::up);
    tile_transitions_4x3.applyAction(tile_puzzle_state_4x3, BlankSlide::up);

    // Blank tile should now be in the top right corner for tile_transitions_4x3,
    // but in row 1, column 1 for tile_transitions_3x4.

    std::vector<Tile> expected_state{1, 0, 3, 4, 5, 2, 7, 8, 9, 10, 11, 6};
    ASSERT_EQ(tile_puzzle_state_3x4.m_permutation, expected_state);

    expected_state = {1, 2, 0, 4, 5, 3, 7, 8, 9, 10, 11, 6};
    ASSERT_EQ(tile_puzzle_state_4x3.m_permutation, expected_state);

    expected_actions = {BlankSlide::right, BlankSlide::down, BlankSlide::left};
    actions = tile_transitions_3x4.getActions(tile_puzzle_state_3x4);
    ASSERT_EQ(actions, expected_actions);

    expected_actions = {BlankSlide::down, BlankSlide::left};
    actions = tile_transitions_4x3.getActions(tile_puzzle_state_4x3);
    ASSERT_EQ(actions, expected_actions);
}

/**
 * Checks that getAllSettings is works.
 */
TEST(SlidingTileTransitionsTests, getSettingsTest) {
    using namespace slidingTileNames;

    SlidingTileTransitions transitions(3, 4, SlidingTileCostType::inverse);

    auto settings = transitions.getAllSettings();
    ASSERT_EQ(settings.m_name, SlidingTileTransitions::CLASS_NAME);
    ASSERT_EQ(settings.m_main_settings.size(), 3);
    ASSERT_EQ(settings.m_main_settings[SETTING_COST_TYPE], COST_INVERSE);
    ASSERT_EQ(settings.m_main_settings[SETTING_NUM_ROWS], "3");
    ASSERT_EQ(settings.m_main_settings[SETTING_NUM_COLS], "4");
    ASSERT_EQ(settings.m_sub_component_settings.size(), 0);
}