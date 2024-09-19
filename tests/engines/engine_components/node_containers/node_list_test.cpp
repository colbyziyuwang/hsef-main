#include <cstdint>
#include <gtest/gtest.h>
#include <optional>
#include <vector>

#include "engines/engine_components/node_containers/node_list.h"
#include "environments/sliding_tile_puzzle/sliding_tile_action.h"
#include "environments/sliding_tile_puzzle/sliding_tile_state.h"

/**
* Tests that adding nodes works properly. Uses sliding tile environment for the test.
*/
TEST(NodeListTests, addNodeTest) {
    NodeList<SlidingTileState, BlankSlide> nodes;

    std::vector<Tile> parent_perm{1, 0, 2, 3, 4, 5};
    std::vector<Tile> child_perm{1, 4, 2, 3, 0, 5};

    SlidingTileState parent_state(parent_perm, 2, 3);
    SlidingTileState child_state(child_perm, 2, 3);

    NodeID parent_id = nodes.addNode(parent_state);
    ASSERT_EQ(parent_id, 0);

    NodeID child_id = nodes.addNode(child_state, 0, 1, BlankSlide::up, 1);
    ASSERT_EQ(child_id, 1);

    ASSERT_EQ(nodes.getState(parent_id), parent_state);
    ASSERT_EQ(nodes.getState(child_id), child_state);

    ASSERT_EQ(nodes.getGValue(parent_id), 0.0);
    ASSERT_EQ(nodes.getGValue(child_id), 1.0);

    ASSERT_FALSE(nodes.getState(child_id) == parent_state);
    ASSERT_TRUE(nodes.getState(child_id) == child_state);

    ASSERT_EQ(nodes.getLastAction(parent_id), std::nullopt);
    ASSERT_EQ(nodes.getLastAction(child_id), BlankSlide::up);

    ASSERT_EQ(nodes.getParentID(parent_id), 0);
    ASSERT_EQ(nodes.getParentID(child_id), 0);

    ASSERT_EQ(nodes.size(), 2);

    nodes.clear();
    ASSERT_EQ(nodes.size(), 0);
}

/**
 * Tests the setters methods works correctly.
 */
TEST(NodeListTests, settersTest) {
    NodeList<SlidingTileState, BlankSlide> nodes;

    std::vector<Tile> parent_perm{1, 0, 2, 3, 4, 5};
    std::vector<Tile> child_perm{1, 4, 2, 3, 0, 5};

    SlidingTileState parent_state(parent_perm, 2, 3);
    SlidingTileState child_state(child_perm, 2, 3);

    double child_g = 1.0;

    NodeID parent_id = nodes.addNode(parent_state);
    NodeID child_id = nodes.addNode(child_state, 0, child_g, BlankSlide::up, 1);

    nodes.setGValue(child_id, 100);
    nodes.setParentID(child_id, 200);
    nodes.setLastAction(child_id, BlankSlide::down);
    nodes.setLastActionCost(child_id, 300);

    ASSERT_EQ(nodes.getGValue(parent_id), 0.0);
    ASSERT_EQ(nodes.getParentID(parent_id), 0);
    ASSERT_FALSE(nodes.getLastAction(parent_id).has_value());
    ASSERT_EQ(nodes.getLastActionCost(parent_id), 0.0);

    ASSERT_EQ(nodes.getGValue(child_id), 100.0);
    ASSERT_EQ(nodes.getParentID(child_id), 200);
    ASSERT_EQ(nodes.getLastAction(child_id).value(), BlankSlide::down);
    ASSERT_EQ(nodes.getLastActionCost(child_id), 300);
}

/**
 * Tests that pop back works correctly.
 */
TEST(NodeListTests, popBackTest) {
    NodeList<SlidingTileState, BlankSlide> nodes;

    std::vector<Tile> perm1{1, 0, 2, 3, 4, 5};
    std::vector<Tile> perm2{1, 4, 2, 3, 0, 5};
    std::vector<Tile> perm3{1, 4, 2, 0, 3, 5};
    std::vector<Tile> perm4{1, 4, 2, 3, 5, 0};

    SlidingTileState state1(perm1, 2, 3);
    SlidingTileState state2(perm2, 2, 3);
    SlidingTileState state3(perm3, 2, 3);
    SlidingTileState state4(perm4, 2, 3);

    NodeID id1 = nodes.addNode(state1);
    NodeID id2 = nodes.addNode(state2, 0, 1, BlankSlide::down, 1);
    NodeID id3 = nodes.addNode(state3, 0, 2, BlankSlide::left, 1);

    ASSERT_EQ(id1, 0);
    ASSERT_EQ(id2, 1);
    ASSERT_EQ(id3, 2);

    nodes.popBack();
    ASSERT_EQ(nodes.size(), 2);

    NodeID id4 = nodes.addNode(state4, 0, 2, BlankSlide::right, 1);

    ASSERT_EQ(id4, 2);
    ASSERT_EQ(nodes.size(), 3);

    ASSERT_EQ(nodes.getState(id1), state1);
    ASSERT_EQ(nodes.getState(id2), state2);
    ASSERT_EQ(nodes.getState(id4), state4);

    nodes.popBack();
    ASSERT_EQ(nodes.size(), 2);

    nodes.popBack();
    ASSERT_EQ(nodes.size(), 1);

    nodes.popBack();
    ASSERT_EQ(nodes.size(), 0);
}
