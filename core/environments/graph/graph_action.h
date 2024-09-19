#ifndef GRAPH_ACTION_H_
#define GRAPH_ACTION_H_

#include "graph.h"

#include <ostream>

/**
 * An edge will be used as an action when interacting with a graph through a GraphTransitions object.
 *
 * @struct GraphAction
 */
struct GraphAction {
    /**
     * Creates an GraphAction for the given edge ID and graph pointer
     *
     * @param edge_id
     * @param graph_for_edge
     */
    GraphAction(EdgeID edge_id, const Graph* graph_for_edge);

    EdgeID m_edge_id;  ///< The ID of the edge in the graph
    const Graph* m_graph;  ///< A pointer to the graph this edge is part of
};

/**
 * Outputs a string representation of the edge action to the given output stream as the label of that edge.
 *
 * @param out The output stream.
 * @param state The vertes state to output.
 * @return The output stream.
 */
std::ostream& operator<<(std::ostream& out, const GraphAction& action);

/**
 * Defines equality of two edge actions.
 *
 * @param state1 The first state to test.
 * @param state2 The second state to test.
 * @return If the states are equal or not.
 */
bool operator==(const GraphAction& action1, const GraphAction& action2);

/**
 * Defines inequality for vertex states.
 *
 * @param state1 The first state to compare.
 * @param state2 The second state to compare.
 * @return If the states are not equal.
 */
bool operator!=(const GraphAction& action1, const GraphAction& action2);

#endif  //GRAPH_ACTION_H_
