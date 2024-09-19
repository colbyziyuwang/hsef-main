#ifndef GRAPH_STATE_H_
#define GRAPH_STATE_H_

#include <ostream>

#include "graph.h"

/**
 * The state which is a vertex that a search algorithm will interact with through a GraphTransitions object.
 *
 * @struct GraphState
 */
struct GraphState {
    VertexID m_vertex_id;  ///< The ID of the vertex in the graph
    const Graph* m_graph;  ///< A pointer to the graph this vertex is part of
};

/**
 * Outputs a string representation of the GraphState to the given output stream as the label of that vertex.
 *
 * @param out The output stream.
 * @param state The vertes state to output.
 * @return The output stream.
 */
std::ostream& operator<<(std::ostream& out, const GraphState& state);

/**
 * Defines equality of two vertex states.
 *
 * @param state1 The first state to test.
 * @param state2 The second state to test.
 * @return If the states are equal or not.
 */
bool operator==(const GraphState& state1, const GraphState& state2);

/**
 * Defines inequality for vertex states.
 *
 * @param state1 The first state to compare.
 * @param state2 The second state to compare.
 * @return If the states are not equal.
 */
bool operator!=(const GraphState& state1, const GraphState& state2);

#endif  //GRAPH_STATE_H_
