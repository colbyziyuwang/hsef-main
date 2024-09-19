#include <cassert>
#include <optional>
#include <string>
#include <vector>

#include "graph.h"
#include "graph_action.h"
#include "graph_state.h"
#include "graph_transitions.h"

GraphTransitions::GraphTransitions(const Graph& graph)
          : m_graph(&graph) {
}

bool GraphTransitions::isApplicable(const GraphState& state, const GraphAction& action) const {
    assert(state.m_vertex_id < m_graph->getNumVertices());
    return (action.m_edge_id < m_graph->getNumEdges() &&
              m_graph->getEdgeByID(action.m_edge_id).m_from_vertex_id == state.m_vertex_id);
}

double GraphTransitions::getActionCost(const GraphState& state, const GraphAction& action) const {
    assert(state.m_vertex_id < m_graph->getNumVertices());
    const auto& edge = m_graph->getEdgeByID(action.m_edge_id);
    assert(edge.m_from_vertex_id == state.m_vertex_id);
    return edge.m_cost;
}

void GraphTransitions::applyAction(GraphState& state, const GraphAction& action) const {
    assert(state.m_vertex_id < m_graph->getNumVertices());
    const auto& edge = m_graph->getEdgeByID(action.m_edge_id);
    assert(edge.m_from_vertex_id == state.m_vertex_id);
    state.m_vertex_id = edge.m_to_vertex_id;
}

std::vector<GraphAction> GraphTransitions::getActions(const GraphState& state) const {
    assert(state.m_vertex_id < m_graph->getNumVertices());

    std::vector<GraphAction> actions;
    for (EdgeID edge_id : m_graph->getOutEdgeIDs(state.m_vertex_id)) {
        actions.emplace_back(edge_id, state.m_graph);
    }
    return actions;
}

std::optional<GraphAction> GraphTransitions::getInverse(const GraphState& state, const GraphAction& action) const {
    assert(state.m_vertex_id == m_graph->getEdgeByID(action.m_edge_id).m_from_vertex_id);
    if (action.m_edge_id < m_graph->getNumEdges()) {
        assert(state.m_vertex_id == m_graph->getEdgeByID(action.m_edge_id).m_from_vertex_id);
        const auto& edge = m_graph->getEdgeByID(action.m_edge_id);
        if (edge.m_edge_id != edge.m_inverse_id) {
            return GraphAction{m_graph->getEdgeByID(action.m_edge_id).m_inverse_id, action.m_graph};
        }
    }
    return std::nullopt;
}


bool GraphTransitions::isValidState(const GraphState& state) const {
    return state.m_vertex_id < m_graph->getNumVertices() && state.m_graph == m_graph;
}

GraphState GraphTransitions::getVertexState(const std::string& vertex_label) {
    return {m_graph->getVertexID(vertex_label), m_graph};
}

GraphAction GraphTransitions::getEdgeAction(const std::string& from_vertex_label, const std::string& to_vertex_label) {
    return {m_graph->getEdgeID(from_vertex_label, to_vertex_label), m_graph};
}
