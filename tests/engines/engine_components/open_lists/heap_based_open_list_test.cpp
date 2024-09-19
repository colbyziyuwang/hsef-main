#include <gtest/gtest.h>

#include "building_tools/evaluators/constant_heuristic.h"
#include "engines/engine_components/open_lists/evaluator_and_comparing_usage.h"
#include "engines/engine_components/open_lists/heap_based_open_list.h"
#include "environments/sliding_tile_puzzle/sliding_tile_action.h"
#include "environments/sliding_tile_puzzle/sliding_tile_state.h"
#include "search_basics/node_container.h"


/**
 * Creates sets of states and initializes a node list to be used for testing a heap
 */
class HeapBasedOpenListTests : public ::testing::Test {
protected:
    void SetUp() override {
        // initialize all values to 0
        heuristic1.setCachedEval(node0_id, 0);
        heuristic1.setCachedEval(node1_id, 0);
        heuristic1.setCachedEval(node2_id, 0);
        heuristic1.setCachedEval(node3_id, 0);
        heuristic1.setCachedEval(node4_id, 0);
        heuristic1.setCachedEval(node5_id, 0);
        heuristic1.setCachedEval(node6_id, 0);

        EvalsAndUsageVec<SlidingTileState, BlankSlide> zero_and_min_usage;
        zero_and_min_usage.emplace_back(heuristic1, true);

        // Sets evaluator ID
        open_list.setEvaluators(zero_and_min_usage);
    }

public:
    ConstantHeuristic<SlidingTileState, BlankSlide> heuristic1;
    HeapBasedOpenList<SlidingTileState, BlankSlide> open_list;

    // IDs for each node
    NodeID node0_id = 0;
    NodeID node1_id = 1;
    NodeID node2_id = 2;
    NodeID node3_id = 3;
    NodeID node4_id = 4;
    NodeID node5_id = 5;
    NodeID node6_id = 6;
};

/**
* Adds a single node ID to the heap and makes sure everything is initialized correctly
*/
TEST_F(HeapBasedOpenListTests, basicAddTest) {
    // Check initialized correctly
    ASSERT_TRUE(open_list.isEmpty());
    ASSERT_EQ(open_list.getSize(), 0);
    ASSERT_FALSE(open_list.isNodeInOpen(node0_id));
    ASSERT_FALSE(open_list.isNodeInOpen(node1_id));

    open_list.addToOpen(node0_id);

    // Check that everything is correct after first add
    ASSERT_FALSE(open_list.isEmpty());
    ASSERT_EQ(open_list.getSize(), 1);
    ASSERT_TRUE(open_list.isNodeInOpen(node0_id));
    ASSERT_FALSE(open_list.isNodeInOpen(node1_id));
    ASSERT_EQ(open_list.getHeapLocation(node0_id), 0);
    ASSERT_EQ(open_list.getHeapEntry(0), node0_id);
}

/**
 * Does a sequence of add nodes and removes and makes sure nodes are correctly returned and also in
 * the correct order. There are no ties.
 */
TEST_F(HeapBasedOpenListTests, heapOrderTest) {
    heuristic1.setCachedEval(node0_id, 3);
    heuristic1.setCachedEval(node1_id, 4);
    heuristic1.setCachedEval(node2_id, 2);
    heuristic1.setCachedEval(node3_id, 1);
    heuristic1.setCachedEval(node4_id, 3);
    heuristic1.setCachedEval(node5_id, 2);
    heuristic1.setCachedEval(node6_id, 0);

    open_list.addToOpen(node0_id);

    // Check get best when just a single node in open
    ASSERT_EQ(open_list.getIDOfBestNode(), node0_id);

    NodeID remove_1 = open_list.getAndRemoveIDOfBestNode();
    ASSERT_EQ(remove_1, node0_id);
    ASSERT_TRUE(open_list.isEmpty());
    ASSERT_EQ(open_list.getSize(), 0);
    ASSERT_FALSE(open_list.isNodeInOpen(node0_id));

    // Putting "worse" node on first
    open_list.addToOpen(node1_id);
    open_list.addToOpen(node2_id);

    // Remove best and check that nodes are in the list as expected
    ASSERT_EQ(open_list.getIDOfBestNode(), node2_id);
    NodeID remove_2 = open_list.getAndRemoveIDOfBestNode();
    ASSERT_EQ(remove_2, node2_id);
    ASSERT_FALSE(open_list.isNodeInOpen(node0_id));
    ASSERT_TRUE(open_list.isNodeInOpen(node1_id));
    ASSERT_FALSE(open_list.isNodeInOpen(node2_id));

    // Put better node on first
    open_list.addToOpen(node3_id);
    open_list.addToOpen(node4_id);

    // Remove best and check that nodes are in the list as expected
    ASSERT_EQ(open_list.getIDOfBestNode(), node3_id);
    NodeID remove_3 = open_list.getAndRemoveIDOfBestNode();
    ASSERT_EQ(remove_3, node3_id);
    ASSERT_FALSE(open_list.isNodeInOpen(node0_id));
    ASSERT_TRUE(open_list.isNodeInOpen(node1_id));
    ASSERT_FALSE(open_list.isNodeInOpen(node2_id));
    ASSERT_FALSE(open_list.isNodeInOpen(node3_id));
    ASSERT_TRUE(open_list.isNodeInOpen(node4_id));

    open_list.addToOpen(node5_id);
    open_list.addToOpen(node6_id);

    // move goal to close - low eval
    ASSERT_EQ(open_list.getIDOfBestNode(), node6_id);
    NodeID remove_4 = open_list.getAndRemoveIDOfBestNode();
    ASSERT_EQ(remove_4, node6_id);
    ASSERT_FALSE(open_list.isNodeInOpen(node0_id));
    ASSERT_TRUE(open_list.isNodeInOpen(node1_id));
    ASSERT_FALSE(open_list.isNodeInOpen(node2_id));
    ASSERT_FALSE(open_list.isNodeInOpen(node3_id));
    ASSERT_TRUE(open_list.isNodeInOpen(node4_id));
    ASSERT_TRUE(open_list.isNodeInOpen(node5_id));
    ASSERT_FALSE(open_list.isNodeInOpen(node6_id));
}

/**
* Tests that removing items from heap works correctly, and heap stays in right order
*/
TEST_F(HeapBasedOpenListTests, removeAndHeapOrderTest) {
    heuristic1.setCachedEval(node0_id, 4);
    heuristic1.setCachedEval(node1_id, 5);
    heuristic1.setCachedEval(node2_id, 3);
    heuristic1.setCachedEval(node3_id, 6);
    heuristic1.setCachedEval(node4_id, 2);
    heuristic1.setCachedEval(node5_id, 7);

    open_list.addToOpen(node0_id);
    open_list.addToOpen(node1_id);
    open_list.addToOpen(node2_id);
    open_list.addToOpen(node3_id);
    open_list.addToOpen(node4_id);
    open_list.addToOpen(node5_id);

    // expected heap list: [5,3,1,4,2,6]
    ASSERT_EQ(open_list.getHeapLocation(node4_id), 0);
    ASSERT_EQ(open_list.getHeapLocation(node2_id), 1);
    ASSERT_EQ(open_list.getHeapLocation(node0_id), 2);
    ASSERT_EQ(open_list.getHeapLocation(node3_id), 3);
    ASSERT_EQ(open_list.getHeapLocation(node1_id), 4);
    ASSERT_EQ(open_list.getHeapLocation(node5_id), 5);

    ASSERT_EQ(open_list.getHeapEntry(0), node4_id);
    ASSERT_EQ(open_list.getHeapEntry(1), node2_id);
    ASSERT_EQ(open_list.getHeapEntry(2), node0_id);
    ASSERT_EQ(open_list.getHeapEntry(3), node3_id);
    ASSERT_EQ(open_list.getHeapEntry(4), node1_id);
    ASSERT_EQ(open_list.getHeapEntry(5), node5_id);

    ASSERT_EQ(open_list.getSize(), 6);

    //remove value in the middle of the heap
    //expected heap list: [5,2,1,4,6]
    open_list.removeFromHeap(node2_id);
    ASSERT_FALSE(open_list.isNodeInOpen(node2_id));
    ASSERT_EQ(open_list.getHeapLocation(node4_id), 0);
    ASSERT_EQ(open_list.getHeapLocation(node1_id), 1);
    ASSERT_EQ(open_list.getHeapLocation(node0_id), 2);
    ASSERT_EQ(open_list.getHeapLocation(node3_id), 3);
    ASSERT_EQ(open_list.getHeapLocation(node5_id), 4);

    ASSERT_EQ(open_list.getHeapEntry(0), node4_id);
    ASSERT_EQ(open_list.getHeapEntry(1), node1_id);
    ASSERT_EQ(open_list.getHeapEntry(2), node0_id);
    ASSERT_EQ(open_list.getHeapEntry(3), node3_id);
    ASSERT_EQ(open_list.getHeapEntry(4), node5_id);

    ASSERT_EQ(open_list.getSize(), 5);

    //remove 6: value at the end of the heap
    //expected heap list: [5,2,1,4]
    open_list.removeFromHeap(node5_id);
    ASSERT_FALSE(open_list.isNodeInOpen(node5_id));
    ASSERT_EQ(open_list.getHeapLocation(node4_id), 0);
    ASSERT_EQ(open_list.getHeapLocation(node1_id), 1);
    ASSERT_EQ(open_list.getHeapLocation(node0_id), 2);
    ASSERT_EQ(open_list.getHeapLocation(node3_id), 3);

    ASSERT_EQ(open_list.getHeapEntry(0), node4_id);
    ASSERT_EQ(open_list.getHeapEntry(1), node1_id);
    ASSERT_EQ(open_list.getHeapEntry(2), node0_id);
    ASSERT_EQ(open_list.getHeapEntry(3), node3_id);

    ASSERT_EQ(open_list.getSize(), 4);

    //remove 5: value at the begining of the heap
    //expected heap list: [1,2,4]
    open_list.removeFromHeap(node4_id);
    ASSERT_FALSE(open_list.isNodeInOpen(node4_id));
    ASSERT_EQ(open_list.getHeapLocation(node0_id), 0);
    ASSERT_EQ(open_list.getHeapLocation(node1_id), 1);
    ASSERT_EQ(open_list.getHeapLocation(node3_id), 2);

    ASSERT_EQ(open_list.getHeapEntry(0), node0_id);
    ASSERT_EQ(open_list.getHeapEntry(1), node1_id);
    ASSERT_EQ(open_list.getHeapEntry(2), node3_id);

    ASSERT_EQ(open_list.getSize(), 3);
}

/**
 * Tests that removing works correctly and the heap order is maintained when using
 * the evaluator to prioritize larger values.
 */
TEST_F(HeapBasedOpenListTests, removeAndMaxHeapOrderTest) {
    heuristic1.setCachedEval(node0_id, 4);
    heuristic1.setCachedEval(node1_id, 5);
    heuristic1.setCachedEval(node2_id, 3);
    heuristic1.setCachedEval(node3_id, 6);
    heuristic1.setCachedEval(node4_id, 2);
    heuristic1.setCachedEval(node5_id, 7);

    EvalsAndUsageVec<SlidingTileState, BlankSlide> heuristics_and_usage;
    heuristics_and_usage.emplace_back(heuristic1, false);
    open_list.setEvaluators(heuristics_and_usage);

    open_list.addToOpen(node0_id);
    open_list.addToOpen(node1_id);
    open_list.addToOpen(node2_id);
    open_list.addToOpen(node3_id);
    open_list.addToOpen(node4_id);
    open_list.addToOpen(node5_id);

    // expected heap list: [6, 2, 4, 1, 5, 3]
    ASSERT_EQ(open_list.getHeapLocation(node5_id), 0);
    ASSERT_EQ(open_list.getHeapLocation(node1_id), 1);
    ASSERT_EQ(open_list.getHeapLocation(node3_id), 2);
    ASSERT_EQ(open_list.getHeapLocation(node0_id), 3);
    ASSERT_EQ(open_list.getHeapLocation(node4_id), 4);
    ASSERT_EQ(open_list.getHeapLocation(node2_id), 5);

    ASSERT_EQ(open_list.getHeapEntry(0), node5_id);
    ASSERT_EQ(open_list.getHeapEntry(1), node1_id);
    ASSERT_EQ(open_list.getHeapEntry(2), node3_id);
    ASSERT_EQ(open_list.getHeapEntry(3), node0_id);
    ASSERT_EQ(open_list.getHeapEntry(4), node4_id);
    ASSERT_EQ(open_list.getHeapEntry(5), node2_id);

    // Remove two entries
    ASSERT_EQ(open_list.getAndRemoveIDOfBestNode(), node5_id);
    open_list.removeFromHeap(node1_id);

    // expected heap is [4, 1, 3, 5]
    ASSERT_EQ(open_list.getHeapLocation(node3_id), 0);
    ASSERT_EQ(open_list.getHeapLocation(node0_id), 1);
    ASSERT_EQ(open_list.getHeapLocation(node2_id), 2);
    ASSERT_EQ(open_list.getHeapLocation(node4_id), 3);

    ASSERT_EQ(open_list.getHeapEntry(0), node3_id);
    ASSERT_EQ(open_list.getHeapEntry(1), node0_id);
    ASSERT_EQ(open_list.getHeapEntry(2), node2_id);
    ASSERT_EQ(open_list.getHeapEntry(3), node4_id);
}

/**
 * Tests that evalChanged works as expected with variety of cases where heap entries should be propagated up and down.
 */
TEST_F(HeapBasedOpenListTests, evalChangedTest) {
    // Starting heap is [5,3,1,4,2,6]
    heuristic1.setCachedEval(node0_id, 4);
    heuristic1.setCachedEval(node1_id, 5);
    heuristic1.setCachedEval(node2_id, 3);
    heuristic1.setCachedEval(node3_id, 6);
    heuristic1.setCachedEval(node4_id, 2);
    heuristic1.setCachedEval(node5_id, 7);

    open_list.addToOpen(node0_id);
    open_list.addToOpen(node1_id);
    open_list.addToOpen(node2_id);
    open_list.addToOpen(node3_id);
    open_list.addToOpen(node4_id);
    open_list.addToOpen(node5_id);

    // 1 should get pulled to top of the heap: [1, 3, 5, 3, 2, 6]
    heuristic1.setCachedEval(node0_id, 1);
    open_list.evalChanged(node0_id);

    ASSERT_EQ(open_list.getHeapLocation(node0_id), 0);
    ASSERT_EQ(open_list.getHeapLocation(node2_id), 1);
    ASSERT_EQ(open_list.getHeapLocation(node4_id), 2);
    ASSERT_EQ(open_list.getHeapLocation(node3_id), 3);
    ASSERT_EQ(open_list.getHeapLocation(node1_id), 4);
    ASSERT_EQ(open_list.getHeapLocation(node5_id), 5);

    ASSERT_EQ(open_list.getHeapEntry(0), node0_id);
    ASSERT_EQ(open_list.getHeapEntry(1), node2_id);
    ASSERT_EQ(open_list.getHeapEntry(2), node4_id);
    ASSERT_EQ(open_list.getHeapEntry(3), node3_id);
    ASSERT_EQ(open_list.getHeapEntry(4), node1_id);
    ASSERT_EQ(open_list.getHeapEntry(5), node5_id);

    // 1 should drop back to where it was, and heap goes back to [5,3,1,4,2,6]
    heuristic1.setCachedEval(node0_id, 4);
    open_list.evalChanged(node0_id);

    ASSERT_EQ(open_list.getHeapLocation(node4_id), 0);
    ASSERT_EQ(open_list.getHeapLocation(node2_id), 1);
    ASSERT_EQ(open_list.getHeapLocation(node0_id), 2);
    ASSERT_EQ(open_list.getHeapLocation(node3_id), 3);
    ASSERT_EQ(open_list.getHeapLocation(node1_id), 4);
    ASSERT_EQ(open_list.getHeapLocation(node5_id), 5);

    ASSERT_EQ(open_list.getHeapEntry(0), node4_id);
    ASSERT_EQ(open_list.getHeapEntry(1), node2_id);
    ASSERT_EQ(open_list.getHeapEntry(2), node0_id);
    ASSERT_EQ(open_list.getHeapEntry(3), node3_id);
    ASSERT_EQ(open_list.getHeapEntry(4), node1_id);
    ASSERT_EQ(open_list.getHeapEntry(5), node5_id);

    // 5 should go all the way to the bottom. Heap should be [3,2,1,4,5,6]
    heuristic1.setCachedEval(node4_id, 100);
    open_list.evalChanged(node4_id);

    ASSERT_EQ(open_list.getHeapLocation(node2_id), 0);
    ASSERT_EQ(open_list.getHeapLocation(node1_id), 1);
    ASSERT_EQ(open_list.getHeapLocation(node0_id), 2);
    ASSERT_EQ(open_list.getHeapLocation(node3_id), 3);
    ASSERT_EQ(open_list.getHeapLocation(node4_id), 4);
    ASSERT_EQ(open_list.getHeapLocation(node5_id), 5);

    ASSERT_EQ(open_list.getHeapEntry(0), node2_id);
    ASSERT_EQ(open_list.getHeapEntry(1), node1_id);
    ASSERT_EQ(open_list.getHeapEntry(2), node0_id);
    ASSERT_EQ(open_list.getHeapEntry(3), node3_id);
    ASSERT_EQ(open_list.getHeapEntry(4), node4_id);
    ASSERT_EQ(open_list.getHeapEntry(5), node5_id);

    // 6 should go all the way to the bottom. Heap should be [6,2,3,4,5,1]
    heuristic1.setCachedEval(node5_id, 0);
    open_list.evalChanged(node5_id);

    ASSERT_EQ(open_list.getHeapLocation(node5_id), 0);
    ASSERT_EQ(open_list.getHeapLocation(node1_id), 1);
    ASSERT_EQ(open_list.getHeapLocation(node2_id), 2);
    ASSERT_EQ(open_list.getHeapLocation(node3_id), 3);
    ASSERT_EQ(open_list.getHeapLocation(node4_id), 4);
    ASSERT_EQ(open_list.getHeapLocation(node0_id), 5);

    ASSERT_EQ(open_list.getHeapEntry(0), node5_id);
    ASSERT_EQ(open_list.getHeapEntry(1), node1_id);
    ASSERT_EQ(open_list.getHeapEntry(2), node2_id);
    ASSERT_EQ(open_list.getHeapEntry(3), node3_id);
    ASSERT_EQ(open_list.getHeapEntry(4), node4_id);
    ASSERT_EQ(open_list.getHeapEntry(5), node0_id);

    // 3 should stay where it is. Heap should be [6,2,3,4,5,1]
    heuristic1.setCachedEval(node2_id, 3.1);
    open_list.evalChanged(node2_id);

    ASSERT_EQ(open_list.getHeapLocation(node5_id), 0);
    ASSERT_EQ(open_list.getHeapLocation(node1_id), 1);
    ASSERT_EQ(open_list.getHeapLocation(node2_id), 2);
    ASSERT_EQ(open_list.getHeapLocation(node3_id), 3);
    ASSERT_EQ(open_list.getHeapLocation(node4_id), 4);
    ASSERT_EQ(open_list.getHeapLocation(node0_id), 5);

    ASSERT_EQ(open_list.getHeapEntry(0), node5_id);
    ASSERT_EQ(open_list.getHeapEntry(1), node1_id);
    ASSERT_EQ(open_list.getHeapEntry(2), node2_id);
    ASSERT_EQ(open_list.getHeapEntry(3), node3_id);
    ASSERT_EQ(open_list.getHeapEntry(4), node4_id);
    ASSERT_EQ(open_list.getHeapEntry(5), node0_id);
}

/**
 * Tests that tiebreaking works over multiple heuristics
 */
TEST_F(HeapBasedOpenListTests, tiebreakingTest) {
    heuristic1.setCachedEval(node0_id, 1);
    heuristic1.setCachedEval(node1_id, 1);
    heuristic1.setCachedEval(node2_id, 1);
    heuristic1.setCachedEval(node3_id, 1);
    heuristic1.setCachedEval(node4_id, 2);
    heuristic1.setCachedEval(node5_id, 2);
    heuristic1.setCachedEval(node6_id, 2);

    ConstantHeuristic<SlidingTileState, BlankSlide> heuristic2;
    heuristic2.setCachedEval(node0_id, 50);
    heuristic2.setCachedEval(node1_id, 50);
    heuristic2.setCachedEval(node2_id, 50);
    heuristic2.setCachedEval(node3_id, 57);
    heuristic2.setCachedEval(node4_id, 25);
    heuristic2.setCachedEval(node5_id, 19);
    heuristic2.setCachedEval(node6_id, 25);

    ConstantHeuristic<SlidingTileState, BlankSlide> heuristic3;
    heuristic3.setCachedEval(node0_id, 7);
    heuristic3.setCachedEval(node1_id, 9);
    heuristic3.setCachedEval(node2_id, 8);
    heuristic3.setCachedEval(node3_id, 10);
    heuristic3.setCachedEval(node4_id, 25);
    heuristic3.setCachedEval(node5_id, 11);
    heuristic3.setCachedEval(node6_id, 25);

    EvalsAndUsageVec<SlidingTileState, BlankSlide> heuristics_and_usage;
    heuristics_and_usage.emplace_back(heuristic1);
    heuristics_and_usage.emplace_back(heuristic2, false);
    heuristics_and_usage.emplace_back(heuristic3);
    open_list.setEvaluators(heuristics_and_usage);

    open_list.addToOpen(node0_id);
    open_list.addToOpen(node1_id);
    open_list.addToOpen(node2_id);
    open_list.addToOpen(node3_id);
    open_list.addToOpen(node4_id);
    open_list.addToOpen(node5_id);
    open_list.addToOpen(node6_id);

    ASSERT_EQ(open_list.getAndRemoveIDOfBestNode(), node3_id);
    ASSERT_EQ(open_list.getAndRemoveIDOfBestNode(), node0_id);
    ASSERT_EQ(open_list.getAndRemoveIDOfBestNode(), node2_id);
    ASSERT_EQ(open_list.getAndRemoveIDOfBestNode(), node1_id);

    ASSERT_EQ(open_list.getAndRemoveIDOfBestNode(), node6_id);  // node4 and node6 are actually tied for all 3 heuristics
    ASSERT_EQ(open_list.getAndRemoveIDOfBestNode(), node4_id);
    ASSERT_EQ(open_list.getAndRemoveIDOfBestNode(), node5_id);
}