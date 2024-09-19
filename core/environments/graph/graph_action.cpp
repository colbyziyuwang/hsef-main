#include "graph_action.h"
#include "graph.h"

#include <ostream>

GraphAction::GraphAction(EdgeID edge_id, const Graph* graph_for_edge)
          : m_edge_id(edge_id), m_graph(graph_for_edge) {
}

std::ostream& operator<<(std::ostream& out, const GraphAction& action) {
    out << action.m_graph->getEdgeByID(action.m_edge_id).m_label;
    return out;
}

bool operator==(const GraphAction& action1, const GraphAction& action2) {
    return (action1.m_edge_id == action2.m_edge_id) && (action1.m_graph == action2.m_graph);
}

bool operator!=(const GraphAction& action1, const GraphAction& action2) {
    return !(action1 == action2);
}