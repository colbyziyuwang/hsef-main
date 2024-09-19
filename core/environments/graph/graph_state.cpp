#include "graph_state.h"

#include <ostream>

std::ostream& operator<<(std::ostream& out, const GraphState& state) {
    out << state.m_graph->getVertexLabel(state.m_vertex_id);
    return out;
}

bool operator==(const GraphState& state1, const GraphState& state2) {
    return (state1.m_vertex_id == state2.m_vertex_id) && (state1.m_graph == state2.m_graph);
}

bool operator!=(const GraphState& state1, const GraphState& state2) {
    return !(state1 == state2);
}