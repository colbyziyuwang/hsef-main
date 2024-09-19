#include <gtest/gtest.h>
#include <vector>

#include "engines/engine_components/node_containers/node_list.h"
#include "environments/grid_pathfinding/grid_location.h"
#include "environments/grid_pathfinding/grid_pathfinding_manhattan_heuristic.h"
#include "environments/sliding_tile_puzzle/sliding_tile_manhattan_heuristic.h"
#include "environments/sliding_tile_puzzle/sliding_tile_transitions.h"


/**
 * Runs through the creation of a heuristic function and doing two heuristic computations. Test using the sliding
 * tile puzzle manhattan distance heuristic.
 *
 * Tests that flags are correct at every step, that the second state is ignored when doPrepare is not called,
 * and that get last heuristic works as expected.
 */
TEST(NodeEvaluatorWithCacheTests, slidingTileManhattanStateTest) {
    NodeList<SlidingTileState, BlankSlide> nodes;

    std::vector<Tile> perm{1, 2, 3, 4, 5, 0, 7, 8, 9, 10, 11, 6, 12, 13, 14, 15};
    SlidingTileState tile_puzzle_state(perm, 4, 4);

    SlidingTileManhattanHeuristic heuristic = SlidingTileManhattanHeuristic(tile_puzzle_state, SlidingTileCostType::unit);
    heuristic.setNodeContainer(nodes);
    ASSERT_FALSE(heuristic.isEvalComputed());

    heuristic.prepareToEvaluate();
    ASSERT_FALSE(heuristic.isEvalComputed());

    std::vector<Tile> state1_perm{7, 5, 1, 2, 4, 6, 11, 14, 12, 0, 3, 8, 9, 10, 15, 13};
    SlidingTileState state1(state1_perm, 4, 4);

    NodeID node1_id = nodes.addNode(state1);
    heuristic.evaluate(node1_id);
    double h_computed = heuristic.getLastNodeEval();

    ASSERT_TRUE(heuristic.isEvalComputed());

    std::vector<Tile> state2_perm{7, 5, 1, 2, 4, 6, 11, 14, 12, 3, 0, 8, 9, 10, 15, 13};
    SlidingTileState state2(state2_perm, 4, 4);

    // New state should be ignored since was prepared was not called
    NodeID node2_id = nodes.addNode(state2);
    heuristic.prepareToEvaluate();
    heuristic.evaluate(node2_id);
    double h_computed2 = heuristic.getLastNodeEval();

    ASSERT_NE(h_computed, h_computed2);
}

/**
 * Tests evaluate when given an ID and a container.
 */
TEST(NodeEvaluatorWithCacheTests, slidingTileManhattanStateContainerTest) {
    std::vector<Tile> goal_perm{1, 2, 3, 4, 5, 0, 7, 8, 9, 10, 11, 6, 12, 13, 14, 15};
    std::vector<Tile> state1_perm{7, 5, 1, 2, 4, 6, 11, 14, 12, 0, 3, 8, 9, 10, 15, 13};
    std::vector<Tile> state2_perm{7, 5, 1, 2, 4, 6, 11, 14, 12, 3, 0, 8, 9, 10, 15, 13};
    SlidingTileState goal_state(goal_perm, 4, 4);
    SlidingTileState state1(state1_perm, 4, 4);
    SlidingTileState state2(state2_perm, 4, 4);

    NodeList<SlidingTileState, BlankSlide> nodes;

    NodeID state1_id = nodes.addNode(state1);
    NodeID state2_id = nodes.addNode(state2, state1_id, 15, BlankSlide::up, 1);
    NodeID goal_id = nodes.addNode(goal_state, state1_id, 15, BlankSlide::up, 1);

    SlidingTileManhattanHeuristic heuristic = SlidingTileManhattanHeuristic(goal_state, SlidingTileCostType::unit);
    heuristic.setNodeContainer(nodes);

    heuristic.prepareToEvaluate();
    heuristic.evaluate(state1_id);

    double h_computed = heuristic.getLastNodeEval();
    NodeID h_computed_id = heuristic.getIDofLastEvaluatedNode();
    ASSERT_EQ(h_computed, 29);
    ASSERT_EQ(h_computed_id, state1_id);

    heuristic.prepareToEvaluate();
    heuristic.evaluate(state2_id);

    h_computed = heuristic.getLastNodeEval();
    h_computed_id = heuristic.getIDofLastEvaluatedNode();
    ASSERT_EQ(h_computed, 30);
    ASSERT_EQ(h_computed_id, state2_id);

    heuristic.prepareToEvaluate();
    heuristic.evaluate(goal_id);

    h_computed = heuristic.getLastNodeEval();
    h_computed_id = heuristic.getIDofLastEvaluatedNode();
    ASSERT_EQ(h_computed, 0);
    ASSERT_EQ(h_computed_id, goal_id);
}

/**
 * Tests that doPrepare updates the flags correctly when using grid map pathfinding.
 */
TEST(NodeEvaluatorWithCacheTests, gridPathfindingManhattanTest) {
    NodeList<GridLocation, GridDirection> nodes;
    GridLocation goal_state(4, 4);
    GridPathfindingManhattanHeuristic heuristic;
    heuristic.setNodeContainer(nodes);
    ASSERT_FALSE(heuristic.isEvalComputed());

    heuristic.setGoalState(goal_state);

    heuristic.prepareToEvaluate();
    ASSERT_FALSE(heuristic.isEvalComputed());

    GridLocation state1(5, 5);
    NodeID node1_id = nodes.addNode(state1);
    heuristic.evaluate(node1_id);
    double h_computed = heuristic.getLastNodeEval();

    ASSERT_TRUE(heuristic.isEvalComputed());

    GridLocation state2(10, 10);

    // New state should be ignored since was prepared was not called
    NodeID node2_id = nodes.addNode(state2);
    heuristic.prepareToEvaluate();
    heuristic.evaluate(node2_id);
    double h_computed2 = heuristic.getLastNodeEval();

    ASSERT_NE(h_computed, h_computed2);
}