#include <gtest/gtest.h>

#include "building_tools/goal_tests/single_state_goal_test.h"
#include "environments/grid_pathfinding/grid_location.h"
#include "environments/grid_pathfinding/grid_pathfinding_transitions.h"
#include "environments/sliding_tile_puzzle/sliding_tile_state.h"
#include "environments/sliding_tile_puzzle/sliding_tile_transitions.h"
#include "utils/plan_and_path_utils.h"


/**
 * Tests that getTransitionAction works properly for the sliding tile puzzle.
 */
TEST(PlanAndPathUtilsTests, slidingTileGetTransitionActionTest) {
    SlidingTileTransitions transitions(2, 3, SlidingTileCostType::unit);

    SlidingTileState parent({1, 2, 0, 3, 4, 5}, 2, 3);

    SlidingTileState child1({1, 0, 2, 3, 4, 5}, 2, 3);
    ASSERT_EQ(getTransitionAction(transitions, parent, child1), BlankSlide::left);

    SlidingTileState child2({1, 2, 5, 3, 4, 0}, 2, 3);
    ASSERT_EQ(getTransitionAction(transitions, parent, child2), BlankSlide::down);

    // not child of itself
    ASSERT_EQ(getTransitionAction(transitions, parent, parent), std::nullopt);

    SlidingTileState not_child({0, 1, 2, 3, 4, 5}, 2, 3);
    ASSERT_EQ(getTransitionAction(transitions, parent, not_child), std::nullopt);
}

/**
 * Tests that getTransitionAction works properly for map pathfinding.
 */
TEST(PlanAndPathUtilsTests, mapPathfindingGetTransitionActionTest) {
    GridMap grid(3, 4);
    GridPathfindingTransitions transitions(&grid, GridConnectionType::four);

    GridLocation parent(2, 3);
    GridLocation child1(2, 2);

    ASSERT_EQ(getTransitionAction(transitions, parent, child1), GridDirection::north);

    GridLocation child2(1, 3);
    ASSERT_EQ(getTransitionAction(transitions, parent, child2), GridDirection::west);

    // not child of itself
    ASSERT_EQ(getTransitionAction(transitions, parent, parent), std::nullopt);

    GridLocation not_child(1, 1);
    ASSERT_EQ(getTransitionAction(transitions, parent, not_child), std::nullopt);
}

/**
 * Tests that getSuccessors works properly for the sliding tile puzzle
 */
TEST(PlanAndPathUtilsTests, slidingTileGetSuccessorsTest) {
    SlidingTileTransitions transitions(2, 3, SlidingTileCostType::unit);

    std::vector<Tile> perm{1, 2, 0, 3, 4, 5};
    SlidingTileState parent(perm, 2, 3);
    std::vector<SlidingTileState> successors = getSuccessors(transitions, parent);

    ASSERT_EQ(successors.size(), 2);

    SlidingTileState child1({1, 0, 2, 3, 4, 5}, 2, 3);
    ASSERT_NE(std::find(successors.begin(), successors.end(), child1), successors.end());

    SlidingTileState child2({1, 2, 5, 3, 4, 0}, 2, 3);
    ASSERT_NE(std::find(successors.begin(), successors.end(), child2), successors.end());

    // not child of itself
    ASSERT_EQ(std::find(successors.begin(), successors.end(), parent), successors.end());

    SlidingTileState not_child({0, 1, 2, 3, 4, 5}, 2, 3);
    ASSERT_EQ(std::find(successors.begin(), successors.end(), not_child), successors.end());
}

/**
 * Tests that getSuccessors works properly for map pathfinding.
 */
TEST(PlanAndPathUtilsTests, mapPathfindingGetSuccessorsTest) {
    GridMap grid(3, 4);
    GridPathfindingTransitions transitions(&grid, GridConnectionType::four);

    GridLocation parent(2, 3);
    std::vector<GridLocation> successors = getSuccessors(transitions, parent);

    ASSERT_EQ(successors.size(), 2);

    GridLocation child1(2, 2);
    ASSERT_NE(std::find(successors.begin(), successors.end(), child1), successors.end());

    GridLocation child2(1, 3);
    ASSERT_NE(std::find(successors.begin(), successors.end(), child2), successors.end());

    // not child of itself
    ASSERT_EQ(std::find(successors.begin(), successors.end(), parent), successors.end());

    GridLocation not_child(1, 1);
    ASSERT_EQ(std::find(successors.begin(), successors.end(), not_child), successors.end());
}

/**
 * Creates elements for the path utils tests that use the sliding tile environment.
 */
class PlanAndPathUtilsSlidingTileTests : public ::testing::Test {
protected:
    void SetUp() override {
        // Sets up valid 5-state sequence
        state_sequence_5_valid.push_back(init_state);
        state_sequence_5_valid.push_back(SlidingTileState({0, 1, 3, 4, 5, 2, 7, 8, 9, 10, 11, 6}, 3, 4));
        state_sequence_5_valid.push_back(SlidingTileState({5, 1, 3, 4, 0, 2, 7, 8, 9, 10, 11, 6}, 3, 4));
        state_sequence_5_valid.push_back(SlidingTileState({5, 1, 3, 4, 9, 2, 7, 8, 0, 10, 11, 6}, 3, 4));
        state_sequence_5_valid.push_back(SlidingTileState({5, 1, 3, 4, 9, 2, 7, 8, 10, 0, 11, 6}, 3, 4));

        // Sets up invalid 3-state sequence
        state_sequence_third_invalid.push_back(init_state);
        state_sequence_third_invalid.push_back(SlidingTileState({0, 1, 3, 4, 5, 2, 7, 8, 9, 10, 11, 6}, 3, 4));
        state_sequence_third_invalid.push_back(SlidingTileState({5, 1, 3, 4, 0, 2, 7, 8, 9, 10, 11, 6}, 4, 3));
        state_sequence_third_invalid.push_back(SlidingTileState({5, 1, 3, 4, 9, 2, 7, 8, 0, 10, 11, 6}, 3, 4));

        // Sets up 3-state sequence where third state is not a child
        state_sequence_third_not_child.push_back(init_state);
        state_sequence_third_not_child.push_back(SlidingTileState({0, 1, 3, 4, 5, 2, 7, 8, 9, 10, 11, 6}, 3, 4));
        state_sequence_third_not_child.push_back(SlidingTileState({5, 1, 3, 4, 2, 0, 7, 8, 9, 10, 11, 6}, 3, 4));
        state_sequence_third_not_child.push_back(SlidingTileState({5, 1, 3, 4, 9, 2, 7, 8, 0, 10, 11, 6}, 3, 4));
    }

public:
    SlidingTileTransitions transitions = {3, 4, SlidingTileCostType::heavy};  // Basic heavy transitions

    SlidingTileState init_state = {{1, 0, 3, 4, 5, 2, 7, 8, 9, 10, 11, 6}, 3, 4};  // default init state

    std::vector<BlankSlide> empty_plan;  // empty plan

    // Sets up a valid 4-action plan
    std::vector<BlankSlide> plan_valid_4action = {BlankSlide::left, BlankSlide::down, BlankSlide::down, BlankSlide::right};
    double plan_valid_4action_cost = 25.0;
    SlidingTileState plan_valid_4action_end = {{5, 1, 3, 4, 9, 2, 7, 8, 10, 0, 11, 6}, 3, 4};

    // Plan that fails on the first action
    std::vector<BlankSlide> plan_first_action_fails = {BlankSlide::up, BlankSlide::left, BlankSlide::left};

    // Sets up plan that fails on the third action
    std::vector<BlankSlide> plan_third_action_fails = {BlankSlide::left, BlankSlide::down, BlankSlide::left};
    double plan_third_action_fails_cost = 6.0;
    SlidingTileState plan_third_action_fails_end = {{5, 1, 3, 4, 0, 2, 7, 8, 9, 10, 11, 6}, 3, 4};

    // Creates sequences for those tests
    std::vector<SlidingTileState> state_sequence_empty;
    std::vector<SlidingTileState> state_sequence_single_valid = {init_state};
    std::vector<SlidingTileState> state_sequence_5_valid;
    double state_sequence_5_valid_cost = 25.0;
    std::vector<SlidingTileState> state_sequence_first_invalid = {SlidingTileState({0, 1, 3, 4, 5, 2, 7, 8, 9, 10, 11, 6}, 4, 3)};
    std::vector<SlidingTileState> state_sequence_third_invalid;
    double state_sequence_third_invalid_cost = 1.0;
    std::vector<SlidingTileState> state_sequence_third_not_child;
    double state_sequence_third_not_child_cost = 1.0;

    // Goal tests to be used for solution checks
    SingleStateGoalTest<SlidingTileState> init_goal_test = SingleStateGoalTest<SlidingTileState>(init_state);
    SingleStateGoalTest<SlidingTileState> valid_plan_test = SingleStateGoalTest<SlidingTileState>(plan_valid_4action_end);
};

/**
 * Tests isActionSequenceValid on the sliding tile environment. Empty, valid, and invalid sequences are tested
 */
TEST_F(PlanAndPathUtilsSlidingTileTests, applyPlanTest) {
    // Check empty plan
    SlidingTileState state_copy(init_state);
    SequenceCheckResult result = applyPlan(state_copy, empty_plan, transitions);
    ASSERT_TRUE(result.m_is_valid);
    ASSERT_EQ(result.m_sequence_cost, 0.0);
    ASSERT_EQ(state_copy, init_state);

    // Check valid 4 action plan
    state_copy = SlidingTileState(init_state);
    result = applyPlan(state_copy, plan_valid_4action, transitions);
    ASSERT_TRUE(result.m_is_valid);
    ASSERT_EQ(result.m_sequence_cost, plan_valid_4action_cost);
    ASSERT_EQ(state_copy, plan_valid_4action_end);

    // First action fails
    state_copy = SlidingTileState(init_state);
    result = applyPlan(state_copy, plan_first_action_fails, transitions);
    ASSERT_FALSE(result.m_is_valid);
    ASSERT_EQ(result.m_sequence_cost, 0.0);
    ASSERT_EQ(state_copy, init_state);

    // Third action fails
    state_copy = SlidingTileState(init_state);
    result = applyPlan(state_copy, plan_third_action_fails, transitions);
    ASSERT_FALSE(result.m_is_valid);
    ASSERT_EQ(result.m_sequence_cost, plan_third_action_fails_cost);
    ASSERT_EQ(state_copy, plan_third_action_fails_end);
}

/**
 * Tests that check plan function works for the sliding tile environment. Tests onon empty, valid, and invalid plans.
 */
TEST_F(PlanAndPathUtilsSlidingTileTests, checkPlanTest) {
    // Check empty plan
    SequenceCheckResult result = checkPlan(init_state, empty_plan, transitions);
    ASSERT_TRUE(result.m_is_valid);
    ASSERT_EQ(result.m_sequence_cost, 0.0);

    // Check valid 4 action plan
    result = checkPlan(init_state, plan_valid_4action, transitions);
    ASSERT_TRUE(result.m_is_valid);
    ASSERT_EQ(result.m_sequence_cost, plan_valid_4action_cost);

    // First action fails
    result = checkPlan(init_state, plan_first_action_fails, transitions);
    ASSERT_FALSE(result.m_is_valid);
    ASSERT_EQ(result.m_sequence_cost, 0.0);

    // Third action fails
    result = checkPlan(init_state, plan_third_action_fails, transitions);
    ASSERT_FALSE(result.m_is_valid);
    ASSERT_EQ(result.m_sequence_cost, plan_third_action_fails_cost);
}

/**
 * Tests if checkStateSequence works for the sliding tile environment. Tests on empty sequence, valid, and invalid
 * sequences.
 */
TEST_F(PlanAndPathUtilsSlidingTileTests, checkStateSequenceTest) {
    // Check empty sequence
    SequenceCheckResult result = checkStateSequence(state_sequence_empty, transitions);
    ASSERT_TRUE(result.m_is_valid);
    ASSERT_EQ(result.m_sequence_cost, 0.0);

    // Check single valid state
    result = checkStateSequence(state_sequence_single_valid, transitions);
    ASSERT_TRUE(result.m_is_valid);
    ASSERT_EQ(result.m_sequence_cost, 0.0);

    // Check valid 5 state sequence
    result = checkStateSequence(state_sequence_5_valid, transitions);
    ASSERT_TRUE(result.m_is_valid);
    ASSERT_EQ(result.m_sequence_cost, state_sequence_5_valid_cost);

    // Check first state is invalid
    result = checkStateSequence(state_sequence_first_invalid, transitions);
    ASSERT_FALSE(result.m_is_valid);
    ASSERT_EQ(result.m_sequence_cost, 0.0);

    // Check third state is invalid
    result = checkStateSequence(state_sequence_third_invalid, transitions);
    ASSERT_FALSE(result.m_is_valid);
    ASSERT_EQ(result.m_sequence_cost, state_sequence_third_invalid_cost);

    // Check third state is not a child
    result = checkStateSequence(state_sequence_third_not_child, transitions);
    ASSERT_FALSE(result.m_is_valid);
    ASSERT_EQ(result.m_sequence_cost, state_sequence_third_not_child_cost);
}

/**
 * Tests checkSolutionPlan for the sliding tile environment. Tests empty, valid, and invalid plans.
 */
TEST_F(PlanAndPathUtilsSlidingTileTests, checkSolutionPlanTest) {
    // Check empty plan
    SequenceCheckResult result = checkSolutionPlan(init_state, empty_plan, transitions, init_goal_test);
    ASSERT_TRUE(result.m_is_valid);
    ASSERT_EQ(result.m_sequence_cost, 0.0);

    // Check valid 4 action plan with correct goal test
    result = checkSolutionPlan(init_state, plan_valid_4action, transitions, valid_plan_test);
    ASSERT_TRUE(result.m_is_valid);
    ASSERT_EQ(result.m_sequence_cost, plan_valid_4action_cost);

    // Check valid 4 action plan with incorrect goal test
    result = checkSolutionPlan(init_state, plan_valid_4action, transitions, init_goal_test);
    ASSERT_FALSE(result.m_is_valid);
    ASSERT_EQ(result.m_sequence_cost, plan_valid_4action_cost);

    // First action fails
    result = checkSolutionPlan(init_state, plan_first_action_fails, transitions, valid_plan_test);
    ASSERT_FALSE(result.m_is_valid);
    ASSERT_EQ(result.m_sequence_cost, 0.0);

    // Third action fails
    result = checkSolutionPlan(init_state, plan_third_action_fails, transitions, valid_plan_test);
    ASSERT_FALSE(result.m_is_valid);
    ASSERT_EQ(result.m_sequence_cost, plan_third_action_fails_cost);
}

/**
 * Tests checkSolutionStateSequence on the sliding tile environment. Tests empty, valid, and invalid sequences.
 */
TEST_F(PlanAndPathUtilsSlidingTileTests, checkSolutionStateSequenceTest) {
    // Check empty sequence
    SequenceCheckResult result = checkSolutionStateSequence(state_sequence_empty, transitions, init_goal_test);
    ASSERT_FALSE(result.m_is_valid);
    ASSERT_EQ(result.m_sequence_cost, 0.0);

    // Check single valid state
    result = checkSolutionStateSequence(state_sequence_single_valid, transitions, init_goal_test);
    ASSERT_TRUE(result.m_is_valid);
    ASSERT_EQ(result.m_sequence_cost, 0.0);

    // Check valid 5 state sequence with correct test
    result = checkSolutionStateSequence(state_sequence_5_valid, transitions, valid_plan_test);
    ASSERT_TRUE(result.m_is_valid);
    ASSERT_EQ(result.m_sequence_cost, state_sequence_5_valid_cost);

    // Check valid 5 state sequence with incorrect test
    result = checkSolutionStateSequence(state_sequence_5_valid, transitions, init_goal_test);
    ASSERT_FALSE(result.m_is_valid);
    ASSERT_EQ(result.m_sequence_cost, state_sequence_5_valid_cost);

    // Check first state is invalid
    result = checkSolutionStateSequence(state_sequence_first_invalid, transitions, valid_plan_test);
    ASSERT_FALSE(result.m_is_valid);
    ASSERT_EQ(result.m_sequence_cost, 0.0);

    // Check third state is invalid
    result = checkSolutionStateSequence(state_sequence_third_invalid, transitions, valid_plan_test);
    ASSERT_FALSE(result.m_is_valid);
    ASSERT_EQ(result.m_sequence_cost, state_sequence_third_invalid_cost);

    // Check third state is not a child
    result = checkSolutionStateSequence(state_sequence_third_not_child, transitions, valid_plan_test);
    ASSERT_FALSE(result.m_is_valid);
    ASSERT_EQ(result.m_sequence_cost, state_sequence_third_not_child_cost);
}

/**
 * Creates elements for the path utils tests that use the map pathfinding environment.
 */
class PlanAndPathUtilsMapPathfindingTests : public ::testing::Test {
protected:

public:
    // Sets up transitions
    std::stringstream map_3x4_4 = std::stringstream("height 4\nwidth 3\nmap\nGST\n@.W\n.O.\n...");
    GridMap grid = GridMap(map_3x4_4);
    GridPathfindingTransitions transitions = GridPathfindingTransitions(&grid);
    GridLocation init_state = {1, 1};

    std::vector<GridDirection> plan_empty;  // empty plan

    // Sets up valid two action plan
    std::vector<GridDirection> plan_valid_2action = {GridDirection::north, GridDirection::west};
    GridLocation plan_valid_2action_end = {0, 0};

    // Sets up plan whose first action is invalid
    std::vector<GridDirection> plan_invalid_first_action = {GridDirection::east, GridDirection::west};

    // Sets up plam whose second action is invalid
    std::vector<GridDirection> plan_invalid_second_action = {GridDirection::north, GridDirection::east};
    GridLocation plan_invalid_second_action_end = {1, 0};

    // Sequences for state sequence tests
    std::vector<GridLocation> empty_sequence;
    std::vector<GridLocation> sequence_single_valid = {init_state};
    std::vector<GridLocation> sequence_3states_valid = {GridLocation(1, 1), GridLocation(1, 0), GridLocation(0, 0)};
    std::vector<GridLocation> sequence_first_invalid = {GridLocation(100, 100), GridLocation(1, 0)};
    std::vector<GridLocation> sequence_second_invalid = {GridLocation(1, 1), GridLocation(100, 100), GridLocation(0, 0)};
    std::vector<GridLocation> sequence_third_not_child = {GridLocation(1, 1), GridLocation(1, 0), GridLocation(2, 0)};

    // Goal tests for solution tests
    SingleStateGoalTest<GridLocation> init_goal_test = SingleStateGoalTest<GridLocation>(init_state);
    SingleStateGoalTest<GridLocation> valid_plan_test = SingleStateGoalTest<GridLocation>(plan_valid_2action_end);
};

/**
 * Tests apply plan on map pathfinding environments. Tests empty, valid, and invalid plans.
 */
TEST_F(PlanAndPathUtilsMapPathfindingTests, applyPlanTest) {
    GridLocation state_copy(init_state);

    // Check empty plan
    SequenceCheckResult result = applyPlan(state_copy, plan_empty, transitions);
    ASSERT_TRUE(result.m_is_valid);
    ASSERT_EQ(result.m_sequence_cost, 0.0);
    ASSERT_EQ(state_copy, init_state);

    // Check valid 2 action path
    state_copy = GridLocation(init_state);
    result = applyPlan(state_copy, plan_valid_2action, transitions);
    ASSERT_TRUE(result.m_is_valid);
    ASSERT_EQ(result.m_sequence_cost, 2.0);
    ASSERT_EQ(state_copy, plan_valid_2action_end);

    // First action invalid
    state_copy = GridLocation(init_state);
    result = applyPlan(state_copy, plan_invalid_first_action, transitions);
    ASSERT_FALSE(result.m_is_valid);
    ASSERT_EQ(result.m_sequence_cost, 0.0);
    ASSERT_EQ(state_copy, init_state);

    // Second action invalid
    state_copy = GridLocation(init_state);
    result = applyPlan(state_copy, plan_invalid_second_action, transitions);
    ASSERT_FALSE(result.m_is_valid);
    ASSERT_EQ(result.m_sequence_cost, 1.0);
    ASSERT_EQ(state_copy, plan_invalid_second_action_end);
}

/**
 * Tests apply plan on map pathfinding environments. Tests empty, valid, and invalid plans.
 */
TEST_F(PlanAndPathUtilsMapPathfindingTests, checkPlanTest) {
    // Check empty plan
    SequenceCheckResult result = checkPlan(init_state, plan_empty, transitions);
    ASSERT_TRUE(result.m_is_valid);
    ASSERT_EQ(result.m_sequence_cost, 0.0);

    // Check valid 2 action path
    result = checkPlan(init_state, plan_valid_2action, transitions);

    ASSERT_TRUE(result.m_is_valid);
    ASSERT_EQ(result.m_sequence_cost, 2.0);

    // First action invalid
    result = checkPlan(init_state, plan_invalid_first_action, transitions);
    ASSERT_FALSE(result.m_is_valid);
    ASSERT_EQ(result.m_sequence_cost, 0.0);

    // Second action invalid
    result = checkPlan(init_state, plan_invalid_second_action, transitions);
    ASSERT_FALSE(result.m_is_valid);
    ASSERT_EQ(result.m_sequence_cost, 1.0);
}

/**
 * Tests that checkStateSequence works on the map pahtfinding environment. Tests it on empty, valid, and invalid
 * sequences.
 */
TEST_F(PlanAndPathUtilsMapPathfindingTests, checkStateSequenceTest) {
    // Check empty sequence
    SequenceCheckResult result = checkStateSequence(empty_sequence, transitions);
    ASSERT_TRUE(result.m_is_valid);
    ASSERT_EQ(result.m_sequence_cost, 0.0);

    // Check single valid state
    result = checkStateSequence(sequence_single_valid, transitions);
    ASSERT_TRUE(result.m_is_valid);
    ASSERT_EQ(result.m_sequence_cost, 0.0);

    // Valid three state path
    result = checkStateSequence(sequence_3states_valid, transitions);
    ASSERT_TRUE(result.m_is_valid);
    ASSERT_EQ(result.m_sequence_cost, 2.0);

    // Check first state is invalid
    result = checkStateSequence(sequence_first_invalid, transitions);
    ASSERT_FALSE(result.m_is_valid);
    ASSERT_EQ(result.m_sequence_cost, 0.0);

    // Check second state is invalid
    result = checkStateSequence(sequence_second_invalid, transitions);
    ASSERT_FALSE(result.m_is_valid);
    ASSERT_EQ(result.m_sequence_cost, 0.0);

    // Check third state is not a child (there is a wall in this case)
    result = checkStateSequence(sequence_third_not_child, transitions);
    ASSERT_FALSE(result.m_is_valid);
    ASSERT_EQ(result.m_sequence_cost, 1.0);
}

/**
 * Tests checkSolutionPlan in map pathfinding. Tests on empty, valid, and invalid plans.
 */
TEST_F(PlanAndPathUtilsMapPathfindingTests, checkSolutionPlanTest) {
    // Check empty plan
    SequenceCheckResult result = checkSolutionPlan(init_state, plan_empty, transitions, init_goal_test);
    ASSERT_TRUE(result.m_is_valid);
    ASSERT_EQ(result.m_sequence_cost, 0.0);

    // Check valid 2 action path
    result = checkSolutionPlan(init_state, plan_valid_2action, transitions, valid_plan_test);
    ASSERT_TRUE(result.m_is_valid);
    ASSERT_EQ(result.m_sequence_cost, 2.0);

    // Check valid 2 action path wrong test
    result = checkSolutionPlan(init_state, plan_valid_2action, transitions, init_goal_test);
    ASSERT_FALSE(result.m_is_valid);
    ASSERT_EQ(result.m_sequence_cost, 2.0);

    // First action invalid
    result = checkSolutionPlan(init_state, plan_invalid_first_action, transitions, valid_plan_test);
    ASSERT_FALSE(result.m_is_valid);
    ASSERT_EQ(result.m_sequence_cost, 0.0);

    // Second action invalid
    result = checkSolutionPlan(init_state, plan_invalid_second_action, transitions, valid_plan_test);
    ASSERT_FALSE(result.m_is_valid);
    ASSERT_EQ(result.m_sequence_cost, 1.0);
}

/**
 * Tests checkSolutionStateSequence in map pathfinding. Tests on empty, valid, and invalid plans.
 */
TEST_F(PlanAndPathUtilsMapPathfindingTests, checkSolutionStateSequenceTest) {
    // Check empty sequence
    SequenceCheckResult result = checkSolutionStateSequence(empty_sequence, transitions, init_goal_test);
    ASSERT_FALSE(result.m_is_valid);
    ASSERT_EQ(result.m_sequence_cost, 0.0);

    // Check single valid state
    result = checkSolutionStateSequence(sequence_single_valid, transitions, init_goal_test);
    ASSERT_TRUE(result.m_is_valid);
    ASSERT_EQ(result.m_sequence_cost, 0.0);

    // Valid three state path
    result = checkSolutionStateSequence(sequence_3states_valid, transitions, valid_plan_test);
    ASSERT_TRUE(result.m_is_valid);
    ASSERT_EQ(result.m_sequence_cost, 2.0);

    // Valid three state path with incorrect test
    result = checkSolutionStateSequence(sequence_3states_valid, transitions, init_goal_test);
    ASSERT_FALSE(result.m_is_valid);
    ASSERT_EQ(result.m_sequence_cost, 2.0);

    // Check first state is invalid
    result = checkSolutionStateSequence(sequence_first_invalid, transitions, valid_plan_test);
    ASSERT_FALSE(result.m_is_valid);
    ASSERT_EQ(result.m_sequence_cost, 0.0);

    // Check second state is invalid
    result = checkSolutionStateSequence(sequence_second_invalid, transitions, valid_plan_test);
    ASSERT_FALSE(result.m_is_valid);
    ASSERT_EQ(result.m_sequence_cost, 0.0);

    // Check third state is not a child (there is a wall in this case)
    result = checkSolutionStateSequence(sequence_third_not_child, transitions, valid_plan_test);
    ASSERT_FALSE(result.m_is_valid);
    ASSERT_EQ(result.m_sequence_cost, 1.0);
}
