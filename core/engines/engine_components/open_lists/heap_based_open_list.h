#ifndef HEAP_BASED_OPEN_LIST_H_
#define HEAP_BASED_OPEN_LIST_H_

#include "evaluator_and_comparing_usage.h"
#include "search_basics/node_container.h"
#include "utils/floating_point_utils.h"

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <vector>

using HeapIndex = int;  ///< The location of a node in the open list heap.

/**
 * A heap-based open list.
 *
 * @tparam State_t The type of state
 * @tparam Action_t The type of action
 */
template<class State_t, class Action_t>
class HeapBasedOpenList {
public:
    /**
     * Creates a open list corresponding to the given NodeContainer
     *
     * @param nodes The node container this open list corresponds to
     */
    HeapBasedOpenList() = default;

    /**
     * Sets the evaluators to use to order the heap
     *
     * @param evaluators The evaluator used for comparing nodes
     */
    void setEvaluators(const EvalsAndUsageVec<State_t, Action_t>& evaluators) { m_evaluators = evaluators; }

    /**
     * Adds the ID of a node to the open list and moves it to the correct location in the heap.
     *
     * @param node_id The ID of the node to put on the open list.
     */
    void addToOpen(NodeID node_id);

    /**
     * Returns the ID of the node with the best evaluation in the open list. The ID is NOT removed from the open list.
     *
     * @return The ID of the best node on the open list.
     */
    NodeID getIDOfBestNode() const;

    /**
     * Returns the ID of the node with the best evaluation in the open list. The ID is removed from the open list.
     *
     * @return The ID of the node to put on the open list.
     */
    NodeID getAndRemoveIDOfBestNode();

    /**
     * Removes the node with the given ID from the open list.
     *
     * Assumes that the node is in the open list.
     *
     * @param node_id The ID of the node to remove
     */
    void removeFromHeap(NodeID node_id);

    /**
     * Adjusts the location of the node with the given ID if needed if needed due to the evaluation of the node changing.
     *
     * @param node_id The ID of the node whose evaluation has changed.
     */
    void evalChanged(NodeID node_id);

    /**
     * Checks if the node with the given ID is in this open list.
     *
     * @param node_id The ID of the given node
     * @return Is the node in this open list
     */
    bool isNodeInOpen(NodeID node_id) const { return node_id < m_loc_in_heap.size() && m_loc_in_heap[node_id] >= 0; }

    /**
     * Clears the open closed list by emptying the table and heap.
     */
    void clear();

    /**
     * Returns the number of nodes in the open list.
     *
     * @return The size of the open list.
     */
    std::size_t getSize() const { return m_heap.size(); }

    /**
     * Returns if the open list is empty or not.
     *
     * @return If the open list is empty or not.
     */
    bool isEmpty() const { return m_heap.empty(); }

    /**
     * Returns the heap index of the node with the given node index.
     *
     * @param node_id The node ID
     * @return The heap index of the given node.
     */
    HeapIndex getHeapLocation(NodeID node_id) const { return m_loc_in_heap[node_id]; }

    /**
     * Gets the ID of the node at the given heap index.
     *
     * Assumes a valid heap index is given.
     *
     * @param heap_index The index in the heap
     * @return The ID of the node at that heap location
     */
    NodeID getHeapEntry(HeapIndex heap_index) const;

private:
    /**
     * Returns true if the evaluation of the node in the open list at the first location is less than or equal to the second.
     *
     * @param heap_index_1 The location in the open list of the first node being compared.
     * @param heap_index_2 The location in the open list of the second node being compared.
     * @return If the evaluation of the first node is less than or equal to the second.
     */
    bool nodeNoWorse(HeapIndex heap_index_1, HeapIndex heap_index_2) const;

    /**
     * Heapify's up the node at the given open list location if it needs to be moved up.
     *
     * @param heap_index The location to heapify up from.
     * @return If the node at the given location has been heapified up at least one level in the heap.
     */
    bool heapifyUp(HeapIndex heap_index);

    /**
     * Heapify's down the node at the given open list location if it needs to be moved down.
     *
     * @param heap_index The location to heapify down from.
     * @return If the node at the given location has been heapified down at least one level in the heap.
     */
    bool heapifyDown(HeapIndex heap_index);

    /**
     * Swaps the elements in their locations in the heaps at the given locations, and updates the nodes accordingly.
     *
     * @param heap_index_1 The first location in the heap.
     * @param heap_index_2 The second location in the heap.
     */
    void swapOpenLocations(HeapIndex heap_index_1, HeapIndex heap_index_2);

    /**
     * Closes the node at the given heap index.
     *
     * @param heap_index
     */
    void closeNodeByHeapIndex(HeapIndex heap_index);

    /**
     * Checks if the given index is a valid location in the heap.
     *
     * @param heap_index The heap index
     * @return If the location is valid or not
     */
    bool isValidHeapIndex(HeapIndex heap_index) const { return heap_index >= 0 && heap_index < static_cast<HeapIndex>(m_heap.size()); }

    std::vector<NodeID> m_heap;  ///< The heap holding node ids representing the open list.
    std::vector<HeapIndex> m_loc_in_heap;  ///< The location of each node (by index) in the heap. -1 means not in the heap

    EvalsAndUsageVec<State_t, Action_t> m_evaluators;  ///< The evaluators in order they will be applied
};

template<class State_t, class Action_t>
void HeapBasedOpenList<State_t, Action_t>::closeNodeByHeapIndex(HeapIndex heap_index) {
    assert(isValidHeapIndex(heap_index));
    NodeID index = m_heap[heap_index];

    // Put last entry in location
    m_heap[heap_index] = m_heap.back();
    m_loc_in_heap[m_heap[heap_index]] = heap_index;
    m_heap.pop_back();

    m_loc_in_heap[index] = -1;  // record node as not being in heap

    // Find correct location for the moved node
    if (heap_index < static_cast<HeapIndex>(m_heap.size()) && !heapifyUp(heap_index)) {
        heapifyDown(heap_index);
    }
}

template<class State_t, class Action_t>
void HeapBasedOpenList<State_t, Action_t>::clear() {
    m_heap.clear();
    m_loc_in_heap.clear();
}

template<class State_t, class Action_t>
void HeapBasedOpenList<State_t, Action_t>::removeFromHeap(NodeID node_id) {
    assert(isNodeInOpen(node_id));

    closeNodeByHeapIndex(m_loc_in_heap[node_id]);
}

template<class State_t, class Action_t>
NodeID HeapBasedOpenList<State_t, Action_t>::getAndRemoveIDOfBestNode() {
    assert(!isEmpty());

    NodeID best_index = m_heap[0];
    closeNodeByHeapIndex(0);

    return best_index;
}

template<class State_t, class Action_t>
NodeID HeapBasedOpenList<State_t, Action_t>::getIDOfBestNode() const {
    assert(!isEmpty());
    return m_heap[0];
}

template<class State_t, class Action_t>
void HeapBasedOpenList<State_t, Action_t>::evalChanged(NodeID node_id) {
    assert(isNodeInOpen(node_id));

    if (!heapifyUp(m_loc_in_heap[node_id])) {
        heapifyDown(m_loc_in_heap[node_id]);
    }
}

template<class State_t, class Action_t>
void HeapBasedOpenList<State_t, Action_t>::addToOpen(NodeID node_id) {
    assert(!isNodeInOpen(node_id));
    if (node_id >= static_cast<NodeID>(m_loc_in_heap.size())) {
        m_loc_in_heap.resize(node_id + 1, -1);
    }

    m_loc_in_heap[node_id] = static_cast<HeapIndex>(m_heap.size());
    m_heap.push_back(node_id);

    heapifyUp(m_loc_in_heap[node_id]);
}

template<class State_t, class Action_t>
bool HeapBasedOpenList<State_t, Action_t>::nodeNoWorse(HeapIndex heap_index_1, HeapIndex heap_index_2) const {
    NodeID node1_id = m_heap[heap_index_1];
    NodeID node2_id = m_heap[heap_index_2];

    for (auto& eval_and_usage : m_evaluators) {
        double node1_eval = eval_and_usage.m_evaluator->getCachedEval(node1_id);
        double node2_eval = eval_and_usage.m_evaluator->getCachedEval(node2_id);

        if (fpLess(node1_eval, node2_eval)) {
            return eval_and_usage.m_lower_is_better;  // node1_eval is less than node2_eval, so return if it is better to be lower
        } else if (fpGreater(node1_eval, node2_eval)) {
            return !eval_and_usage.m_lower_is_better;  // node1_eval is greater than node2_eval, so return if it is better to be higher
        }
        // If they are equal, move on to the next evaluator
    }
    return true;  // all evaluators say they are equal, so node1 is no worse
}

template<class State_t, class Action_t>
bool HeapBasedOpenList<State_t, Action_t>::heapifyUp(HeapIndex heap_index) {
    assert(isValidHeapIndex(heap_index));
    if (heap_index == 0) {
        return false;
    }

    HeapIndex parent_loc = (heap_index - 1) / 2;

    if (!nodeNoWorse(parent_loc, heap_index)) {
        swapOpenLocations(heap_index, parent_loc);

        heapifyUp(parent_loc);
        return true;
    }
    return false;
}

template<class State_t, class Action_t>
bool HeapBasedOpenList<State_t, Action_t>::heapifyDown(HeapIndex heap_index) {
    HeapIndex left_child_loc = heap_index * 2 + 1;

    if (left_child_loc >= static_cast<HeapIndex>(m_heap.size())) {
        return false;
    }

    HeapIndex right_child_loc = heap_index * 2 + 2;
    HeapIndex best_child_loc = 0;

    if (right_child_loc >= static_cast<HeapIndex>(m_heap.size()) || nodeNoWorse(left_child_loc, right_child_loc)) {
        best_child_loc = left_child_loc;
    } else {
        best_child_loc = right_child_loc;
    }

    if (!nodeNoWorse(heap_index, best_child_loc)) {
        swapOpenLocations(heap_index, best_child_loc);

        heapifyDown(best_child_loc);
        return true;
    }

    return false;
}

template<class State_t, class Action_t>
void HeapBasedOpenList<State_t, Action_t>::swapOpenLocations(HeapIndex heap_index_1, HeapIndex heap_index_2) {
    assert(isValidHeapIndex(heap_index_1));
    assert(isValidHeapIndex(heap_index_2));

    NodeID node_index = m_heap[heap_index_1];
    m_heap[heap_index_1] = m_heap[heap_index_2];
    m_heap[heap_index_2] = node_index;

    m_loc_in_heap[m_heap[heap_index_1]] = heap_index_1;
    m_loc_in_heap[m_heap[heap_index_2]] = heap_index_2;
}

template<class State_t, class Action_t>
NodeID HeapBasedOpenList<State_t, Action_t>::getHeapEntry(HeapIndex heap_index) const {
    assert(isValidHeapIndex(heap_index));
    return m_heap[heap_index];
}

#endif  //HEAP_BASED_OPEN_LIST_H_