#include <algorithm>
#include <gtest/gtest.h>

#include "environments/sliding_tile_puzzle/sliding_tile_manhattan_heuristic.h"
#include "environments/sliding_tile_puzzle/sliding_tile_names.h"
#include "test_helpers.h"

/**
 * Note that constructor tests are in heuristic_function_test since that is used for base functionality.
 */

/**
 * Tests that the computation works correctly when given the goal.
 */
TEST(SlidingTileManhattanHeuristicTests, givenGoalTest) {
    std::vector<Tile> perm{1, 2, 3, 4, 5, 0, 7, 8, 9, 10, 11, 6, 12, 13, 14, 15};
    SlidingTileState tile_puzzle_state(perm, 4, 4);
    SlidingTileManhattanHeuristic manhattan = SlidingTileManhattanHeuristic(tile_puzzle_state, SlidingTileCostType::unit);

    ASSERT_TRUE(checkDistanceToGoStateEvaluation(manhattan, tile_puzzle_state, 0.0, false, 0.0));
}

/**
 * Tests that when we perform a sequence of moves and heuristic computations, all functions return the correct stuff.
 */
TEST(SlidingTileManhattanHeuristicTests, tile4x4UnitHValueAfterMovesTest) {
    std::vector<Tile> goal_perm{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
    SlidingTileState goal_state(goal_perm, 4, 4);
    SlidingTileTransitions trans_func(4, 4, SlidingTileCostType::unit);

    SlidingTileManhattanHeuristic manhattan = SlidingTileManhattanHeuristic(goal_state, SlidingTileCostType::unit);

    // Move one tile
    SlidingTileState current_state = goal_state;
    trans_func.applyAction(current_state, BlankSlide::right);
    ASSERT_TRUE(checkDistanceToGoStateEvaluation(manhattan, current_state, 1.0, false, 1.0));

    // Move one tile again
    trans_func.applyAction(current_state, BlankSlide::down);
    ASSERT_TRUE(checkDistanceToGoStateEvaluation(manhattan, current_state, 2.0, false, 2.0));

    // Move one tile again
    trans_func.applyAction(current_state, BlankSlide::left);
    ASSERT_TRUE(checkDistanceToGoStateEvaluation(manhattan, current_state, 3.0, false, 3.0));

    // Move one tile again
    trans_func.applyAction(current_state, BlankSlide::up);
    ASSERT_TRUE(checkDistanceToGoStateEvaluation(manhattan, current_state, 4.0, false, 4.0));
}

/**
 * Tests the manhattan distance on two arbitrary states. Correct values calculated manually.
 */
TEST(SlidingTileManhattanHeuristicTests, tile4x4UnitHValueDiffStatesTest) {
    std::vector<Tile> goal_perm{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
    SlidingTileState goal_state(goal_perm, 4, 4);
    SlidingTileManhattanHeuristic manhattan = SlidingTileManhattanHeuristic(goal_state, SlidingTileCostType::unit);

    // First test state
    std::vector<Tile> perm_a{7, 5, 1, 2, 4, 6, 11, 14, 12, 0, 3, 8, 9, 10, 15, 13};
    SlidingTileState state_a(perm_a, 4, 4);

    ASSERT_TRUE(checkDistanceToGoStateEvaluation(manhattan, state_a, 27.0, false, 27.0));

    // Second test state is the reverse of the goal permutation
    std::reverse(goal_perm.begin(), goal_perm.end());
    SlidingTileState state_b(goal_perm, 4, 4);
    ASSERT_TRUE(checkDistanceToGoStateEvaluation(manhattan, state_b, 58.0, false, 58.0));
}

/**
 * Tests the manhattan distance when using heavy tile costs. Correct value calculated manually.
 */
TEST(SlidingTileManhattanHeuristicTests, tile4x4HeavyTest) {
    std::vector<Tile> goal_perm{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
    SlidingTileState goal_state(goal_perm, 4, 4);
    SlidingTileManhattanHeuristic manhattan = SlidingTileManhattanHeuristic(goal_state, SlidingTileCostType::heavy);

    // Using same state as in previous test
    std::vector<Tile> perm{7, 5, 1, 2, 4, 6, 11, 14, 12, 0, 3, 8, 9, 10, 15, 13};
    SlidingTileState state(perm, 4, 4);
    ASSERT_TRUE(checkDistanceToGoStateEvaluation(manhattan, state, 230.0, false, 27.0));
}

/**
 * Tests the manhattan distance when using inverse tile costs. Correct value calculated manually.
 */
TEST(SlidingTileManhattanHeuristicTests, tile4x4InverseTest) {
    std::vector<Tile> goal_perm{0, 1, 2, 3, 4, 5};
    SlidingTileState goal_state(goal_perm, 2, 3);
    SlidingTileManhattanHeuristic manhattan = SlidingTileManhattanHeuristic(goal_state, SlidingTileCostType::inverse);

    // Using same state as in previous test
    std::vector<Tile> perm{5, 4, 3, 2, 1, 0};
    SlidingTileState state(perm, 2, 3);

    double expected = (1 * 1.0) + (3 * 0.5) + (3 * (1.0 / 3.0)) + (1 * 0.25) + (3 * 0.2);
    ASSERT_TRUE(checkDistanceToGoStateEvaluation(manhattan, state, expected, false, 11.0));
}

/**
 * Tests the manhattan distance when using set cost type to switch cost type
 */
TEST(SlidingTileManhattanHeuristicTests, tile4x4SetCostTypeTest) {
    std::vector<Tile> goal_perm{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
    SlidingTileState goal_state(goal_perm, 4, 4);
    SlidingTileManhattanHeuristic manhattan = SlidingTileManhattanHeuristic(goal_state, SlidingTileCostType::unit);

    // Using same state as in previous test
    std::vector<Tile> perm{7, 5, 1, 2, 4, 6, 11, 14, 12, 0, 3, 8, 9, 10, 15, 13};
    SlidingTileState state(perm, 4, 4);
    ASSERT_TRUE(checkDistanceToGoStateEvaluation(manhattan, state, 27.0, false, 27.0));

    manhattan.setCostType(SlidingTileCostType::heavy);
    ASSERT_TRUE(checkDistanceToGoStateEvaluation(manhattan, state, 230.0, false, 27.0));
}

/**
 * Tests the manhattan distance on the same permutation for the 3x4 and 4x3 puzzle. Correct values
 * calculated manually.
 */
TEST(SlidingTileManhattanHeuristicTests, tileAssymetricTests) {
    std::vector<Tile> goal_perm{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
    std::vector<Tile> state_perm{5, 6, 7, 8, 9, 10, 11, 0, 1, 2, 3, 4};  // permutation used for testing

    // Tests the 3x4 puzzle
    SlidingTileState goal_3x4(goal_perm, 3, 4);
    SlidingTileManhattanHeuristic manhattan_3x4 = SlidingTileManhattanHeuristic(goal_3x4, SlidingTileCostType::unit);
    SlidingTileState state_3x4(state_perm, 3, 4);
    ASSERT_TRUE(checkStateEvaluation(manhattan_3x4, state_3x4, 30.0, false));

    // Tests the 4x3 puzzle
    SlidingTileState goal_4x3(goal_perm, 4, 3);
    SlidingTileManhattanHeuristic manhattan_4x3 = SlidingTileManhattanHeuristic(goal_4x3, SlidingTileCostType::unit);
    SlidingTileState state_4x3(state_perm, 4, 3);
    ASSERT_TRUE(checkStateEvaluation(manhattan_4x3, state_4x3, 35.0, false));
}

/**
 * Tests that SetGoalState works properly by checking that we can update the goal and get different
 * results. Correct values calculated manually.
 */
TEST(SlidingTileManhattanHeuristicTests, setGoalStateTest) {
    std::vector<Tile> goal_perm_a{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
    SlidingTileState goal_state_a(goal_perm_a, 3, 4);

    std::vector<Tile> perm{5, 6, 7, 8, 9, 10, 11, 0, 1, 2, 3, 4};
    SlidingTileState state(perm, 3, 4);

    // First runs it with standard goal
    SlidingTileManhattanHeuristic manhattan = SlidingTileManhattanHeuristic(goal_state_a, SlidingTileCostType::unit);
    ASSERT_TRUE(checkStateEvaluation(manhattan, state, 30.0, false));
    ASSERT_EQ(manhattan.getGoalState(), goal_state_a);

    // Now runs it with new goal
    std::vector<Tile> goal_perm_b{9, 10, 11, 1, 2, 0, 3, 4, 5, 6, 7, 8};
    SlidingTileState goal_state_b(goal_perm_b, 3, 4);

    manhattan.setGoalState(goal_state_b);
    ASSERT_TRUE(checkStateEvaluation(manhattan, state, 20.0, false));
    ASSERT_EQ(manhattan.getGoalState(), goal_state_b);
}

/**
 * Tests that isValidState works as expected.
 */
TEST(SlidingTileManhattanHeuristicTests, isValidStateTest) {
    std::vector<Tile> goal_perm{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
    SlidingTileState goal_state(goal_perm, 3, 4);
    SlidingTileManhattanHeuristic manhattan = SlidingTileManhattanHeuristic(goal_state, SlidingTileCostType::unit);

    std::vector<Tile> perm{5, 6, 7, 8, 9, 10, 11, 0, 1, 2, 3, 4};
    SlidingTileState state_3x4(perm, 3, 4);
    SlidingTileState state_4x3(perm, 4, 3);

    ASSERT_TRUE(manhattan.isValidState(state_3x4));
    ASSERT_FALSE(manhattan.isValidState(state_4x3));
}

/**
 * Checks that getAllSettings is works.
 */
TEST(SlidingTileManhattanHeuristicTests, getSettingsTest) {
    using namespace slidingTileNames;
    std::vector<Tile> goal_perm{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
    SlidingTileState goal_state(goal_perm, 3, 4);
    SlidingTileManhattanHeuristic manhattan = SlidingTileManhattanHeuristic(goal_state, SlidingTileCostType::unit);

    auto settings = manhattan.getAllSettings();
    ASSERT_EQ(settings.m_name, SlidingTileManhattanHeuristic::CLASS_NAME);
    ASSERT_EQ(settings.m_main_settings.size(), 2);
    ASSERT_EQ(settings.m_main_settings[SETTING_COST_TYPE], COST_UNIT);
    ASSERT_EQ(settings.m_main_settings[SETTING_GOAL_STATE], "(3x4)-[0 1 2 3, 4 5 6 7, 8 9 10 11]");
    ASSERT_EQ(settings.m_sub_component_settings.size(), 0);
}