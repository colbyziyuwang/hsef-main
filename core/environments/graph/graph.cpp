#include "graph.h"
#include "graph_utils.h"

#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <string>
#include <tuple>
#include <vector>

Edge::Edge(EdgeID edge_id, VertexID from_id, VertexID to_vertex, double edge_cost, const std::string& edge_label)
          : m_edge_id(edge_id), m_from_vertex_id(from_id), m_to_vertex_id(to_vertex), m_cost(edge_cost),
            m_label(edge_label), m_inverse_id(edge_id) {
}

std::size_t Graph::getNumVertices() const {
    return m_vertex_labels.size();
}

std::size_t Graph::getNumEdges() const {
    return m_edges.size();
}

std::string Graph::getVertexLabel(VertexID vertex_id) const {
    assert(vertex_id < m_vertex_labels.size());
    return m_vertex_labels[vertex_id];
}

const Edge& Graph::getEdgeByID(EdgeID edge_id) const {
    assert(edge_id < m_edges.size());
    return m_edges[edge_id];
}

const Edge& Graph::getEdgeByLabel(const std::string& edge_label) const {
    return getEdgeByID(getEdgeID(edge_label));
}

const Edge& Graph::getEdgeByVertexLabels(const std::string& from_vertex_label,
          const std::string& to_vertex_label) const {
    return getEdgeByID(getEdgeID(from_vertex_label, to_vertex_label));
}

VertexID Graph::getVertexID(const std::string& vertex_label) const {
    assert(m_vertex_ids.find(vertex_label) != m_vertex_ids.end());
    return m_vertex_ids.at(vertex_label);
}

EdgeID Graph::getEdgeID(const std::string& edge_label) const {
    assert(m_edge_ids.find(edge_label) != m_edge_ids.end());
    return m_edge_ids.at(edge_label);
}

EdgeID Graph::getEdgeID(const std::string& from_vertex_label, const std::string& to_vertex_label) const {
    VertexID from_id = getVertexID(from_vertex_label);
    VertexID to_id = getVertexID(to_vertex_label);

    for (EdgeID current_out_edge : m_out_edges[from_id]) {
        if (m_edges[current_out_edge].m_to_vertex_id == to_id) {
            return current_out_edge;
        }
    }

    return static_cast<EdgeID>(m_edges.size());
}

std::pair<bool, VertexID> Graph::addVertex(const std::string& vertex_label) {
    assert(m_vertex_labels.size() == m_vertex_ids.size());
    assert(m_vertex_ids.size() == m_out_edges.size());
    assert(m_out_edges.size() == m_in_edges.size());

    if (isVertexInGraph(vertex_label)) {
        return {false, m_vertex_ids.at(vertex_label)};
    }

    auto new_id = static_cast<VertexID>(m_vertex_labels.size());
    m_vertex_labels.emplace_back(vertex_label);
    m_vertex_ids[vertex_label] = new_id;

    m_out_edges.emplace_back();
    m_in_edges.emplace_back();
    return {true, new_id};
}

std::pair<bool, EdgeID> Graph::addEdge(const std::string& from_vertex_label, const std::string& to_vertex_label,
          double cost, std::string edge_label) {
    if (from_vertex_label == to_vertex_label) {  // Edge is a self-loop
        return {false, static_cast<EdgeID>(m_edges.size())};
    }
    if (edge_label.empty()) {
        edge_label = getEdgeLabel(from_vertex_label, to_vertex_label);
    }
    if (isEdgeInGraph(edge_label)) {  // Checks if the edge label is already in the graph
        return {false, m_edge_ids.at(edge_label)};
    }

    VertexID from_id = addVertex(from_vertex_label).second;
    VertexID to_id = addVertex(to_vertex_label).second;

    // Looks for an existing edge between those vertices
    for (EdgeID current_out_edge : m_out_edges[from_id]) {
        if (m_edges[current_out_edge].m_to_vertex_id == to_id) {
            return {false, current_out_edge};
        }
    }

    auto new_id = static_cast<EdgeID>(m_edges.size());
    m_edges.emplace_back(new_id, from_id, to_id, cost, edge_label);
    m_edge_ids[edge_label] = new_id;
    m_out_edges[from_id].emplace_back(new_id);
    m_in_edges[to_id].emplace_back(new_id);

    // Search for inverse edge
    for (EdgeID current_in_edge : m_in_edges[from_id]) {
        if (m_edges[current_in_edge].m_from_vertex_id == to_id) {
            assert(m_edges[current_in_edge].m_inverse_id == m_edges[current_in_edge].m_edge_id);
            m_edges[new_id].m_inverse_id = current_in_edge;
            m_edges[current_in_edge].m_inverse_id = new_id;
            break;
        }
    }

    return {true, static_cast<EdgeID>(new_id)};
}

void Graph::sortOutEdgesByToVertexLabel(bool ascending) {
    for (auto& edges : m_out_edges) {
        if (ascending) {
            std::sort(edges.begin(), edges.end(),
                      [this](EdgeID edge1, EdgeID edge2) {
                          return m_vertex_labels[m_edges[edge1].m_to_vertex_id] <
                                 m_vertex_labels[m_edges[edge2].m_to_vertex_id];
                      });
        } else {
            std::sort(edges.begin(), edges.end(),
                      [this](EdgeID edge1, EdgeID edge2) {
                          return m_vertex_labels[m_edges[edge1].m_to_vertex_id] >
                                 m_vertex_labels[m_edges[edge2].m_to_vertex_id];
                      });
        }
    }
}

void Graph::sortInEdgesByFromVertexLabel(bool ascending) {
    for (auto& edges : m_in_edges) {
        if (ascending) {
            std::sort(edges.begin(), edges.end(),
                      [this](EdgeID edge1, EdgeID edge2) {
                          return m_vertex_labels[m_edges[edge1].m_from_vertex_id] <
                                 m_vertex_labels[m_edges[edge2].m_from_vertex_id];
                      });
        } else {
            std::sort(edges.begin(), edges.end(),
                      [this](EdgeID edge1, EdgeID edge2) {
                          return m_vertex_labels[m_edges[edge1].m_from_vertex_id] >
                                 m_vertex_labels[m_edges[edge2].m_from_vertex_id];
                      });
        }
    }
}

const std::vector<EdgeID>& Graph::getOutEdgeIDs(VertexID vertex_id) const {
    return m_out_edges[vertex_id];
}

const std::vector<EdgeID>& Graph::getInEdgeIDs(VertexID vertex_id) const {
    return m_in_edges[vertex_id];
}

bool Graph::isVertexInGraph(const std::string& vertex_label) const {
    return m_vertex_ids.find(vertex_label) != m_vertex_ids.end();
}

bool Graph::isEdgeInGraph(const std::string& edge_label) const {
    return m_edge_ids.find(edge_label) != m_edge_ids.end();
}

bool Graph::isEdgeInGraph(const std::string& from_vertex_label, const std::string& to_vertex_label) const {
    auto from_iter = m_vertex_ids.find(from_vertex_label);
    if (from_iter == m_vertex_ids.end()) {
        return false;
    }

    auto to_iter = m_vertex_ids.find(to_vertex_label);
    if (to_iter == m_vertex_ids.end()) {
        return false;
    }

    VertexID from_id = from_iter->second;
    VertexID to_id = to_iter->second;

    return std::any_of(
              m_out_edges[from_id].begin(), m_out_edges[from_id].end(),
              [this, to_id](EdgeID current_out_edge) {
                  return m_edges[current_out_edge].m_to_vertex_id == to_id;
              });
}
