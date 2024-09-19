#include <gtest/gtest.h>

#include "environments/graph/graph_transitions.h"
#include "environments/grid_pathfinding/grid_pathfinding_transitions.h"
#include "environments/sliding_tile_puzzle/sliding_tile_transitions.h"


/**
 * Tests that getChildState works properly for the sliding tile puzzle.
 */
TEST(TransitionFunctionTests, slidingTileGetChildStateTest) {
    SlidingTileTransitions transitions(2, 3, SlidingTileCostType::unit);

    std::vector<Tile> perm{1, 2, 0, 3, 4, 5};
    SlidingTileState parent(perm, 2, 3);

    ASSERT_EQ(transitions.getChildState(parent, BlankSlide::left),
              SlidingTileState({1, 0, 2, 3, 4, 5}, 2, 3));
    ASSERT_EQ(transitions.getChildState(parent, BlankSlide::down),
              SlidingTileState({1, 2, 5, 3, 4, 0}, 2, 3));
}

/**
 * Tests that gettChildState works properly for grid map pathfinding.
 */
TEST(TransitionFunctionTests, gridPathfindingtGetChildStateTest) {
    GridMap grid(3, 4);
    GridPathfindingTransitions transitions(&grid, GridConnectionType::four);

    GridLocation parent(2, 3);

    ASSERT_EQ(transitions.getChildState(parent, GridDirection::north), GridLocation(2, 2));
    ASSERT_EQ(transitions.getChildState(parent, GridDirection::west), GridLocation(1, 3));
}

/**
 * Tests that isInverseAction works in the sliding tile puzzle.
 */
TEST(TransitionFunctionTests, slidingTileIsInverseActionTest) {
    SlidingTileTransitions transitions(2, 3, SlidingTileCostType::unit);

    std::vector<Tile> perm{1, 2, 0, 3, 4, 5};
    SlidingTileState state(perm, 2, 3);

    ASSERT_TRUE(transitions.isInverseAction(state, BlankSlide::right, BlankSlide::left));
    ASSERT_TRUE(transitions.isInverseAction(state, BlankSlide::up, BlankSlide::down));
    ASSERT_FALSE(transitions.isInverseAction(state, BlankSlide::left, BlankSlide::down));
}

/**
 * Tests that isInverseAction works in grid map pathfinding.
 */
TEST(TransitionFunctionTests, gridPathfindingtIsInverseActionTest) {
    GridMap grid(4, 3);
    GridPathfindingTransitions transitions(&grid, GridConnectionType::four);

    GridLocation state(2, 3);

    ASSERT_TRUE(transitions.isInverseAction(state, GridDirection::south, GridDirection::north));
    ASSERT_TRUE(transitions.isInverseAction(state, GridDirection::east, GridDirection::west));
    ASSERT_FALSE(transitions.isInverseAction(state, GridDirection::north, GridDirection::west));
}

/**
 * Tests that isInverseAction works when we have non invertible actions.
 */
TEST(TransitionFunctionTests, graphIsInverseActionTest) {
    Graph graph;
    graph.addVertex("a");
    graph.addVertex("b");
    graph.addVertex("c");
    graph.addEdge("a", "b", 1);
    graph.addEdge("b", "a", 1);
    graph.addEdge("b", "c", 1);

    GraphTransitions transitions(graph);

    GraphState vertex_b = transitions.getVertexState("b");
    GraphAction edge_a_to_b = transitions.getEdgeAction("a", "b");
    GraphAction edge_b_to_a = transitions.getEdgeAction("b", "a");
    GraphAction edge_b_to_c = transitions.getEdgeAction("b", "c");

    ASSERT_TRUE(transitions.isInverseAction(vertex_b, edge_b_to_a, edge_a_to_b));
    ASSERT_FALSE(transitions.isInverseAction(vertex_b, edge_b_to_a, edge_b_to_c));
    ASSERT_FALSE(transitions.isInverseAction(vertex_b, edge_b_to_c, edge_a_to_b));
}