#ifndef GRAPH_H_
#define GRAPH_H_

#include <cstddef>
#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>

using VertexID = std::size_t;  ///< ID of a vertex is a unique value for each
using EdgeID = std::size_t;  ///< ID of an edge is a unique value for each

/**
 * Stores edge information.
 *
 * @struct Edge
 */
struct Edge {
    /**
     * Builds an edge from the given inputs.
     *
     * @param edge_id ID of this edge in the graph
     * @param from_id ID of the from vertex
     * @param to_id ID of the to vertex
     * @param edge_cost Edge cost
     * @param edge_label Edge label
     */
    Edge(EdgeID edge_id, VertexID from_id, VertexID to_id, double edge_cost, const std::string& edge_label);

    EdgeID m_edge_id;  ///< The ID of this edge in the graph
    VertexID m_from_vertex_id;  ///< ID of the from vertex
    VertexID m_to_vertex_id;  ///< ID of the to vertex
    double m_cost;  ///< Edge cost
    std::string m_label;  ///< Edge label
    EdgeID m_inverse_id;  ///< The inverse of the edge ID. Equal to itself if there is no inverse
};

/**
 * Stores a graph. Graph is stored as a vector of labels, a vector of edges, and incoming/outgoing lists of IDs
 * to the corresponding edges.
 *
 * Note, we store the adjacency lists (in and out edges) as lists of IDs to the corresponding edge. The purpose is
 * to make generation of nodes faster during search.
 *
 * @class Graph
 */
class Graph {
public:
    /**
     * Creates an empty graph.
     */
    Graph() = default;

    /**
     * Gets the number of vertices in the graph.
     *
     * @return The number of vertices in the graph
     */
    std::size_t getNumVertices() const;

    /**
     * Gets the number of edges in the graph.
     *
     * @return The number of edges in the graph.
     */
    std::size_t getNumEdges() const;

    /**
     * Checks if a vertex with the given label is already in the graph.
     *
     * @param vertex_label The vertex label to check for
     * @return Whether a vertex with that label is in the graph
     */
    bool isVertexInGraph(const std::string& vertex_label) const;

    /**
     * Checks if an edge with the given label is in the graph.
     *
     * @param edge_label The edge label to search for
     * @return If the edge is in the graph
     */
    bool isEdgeInGraph(const std::string& edge_label) const;

    /**
     * Checks if an edge between the vertices with the given labels are in the graph.
     *
     * @param from_vertex_label The from vertex label
     * @param to_vertex_label The to vertex label
     * @return If the edge is in the graph
     */
    bool isEdgeInGraph(const std::string& from_vertex_label, const std::string& to_vertex_label) const;

    /**
     * Gets the label of the vertex with the given ID. Assumes that there is a vertex with that ID in the graph.
     *
     * @param vertex_id The ID of the vertex of interest
     * @return The label of the corresponding vertex
     */
    std::string getVertexLabel(VertexID vertex_id) const;

    /**
     * Gets the ID of the vertex with the given label. Assumes there is a vertex with that ID in the graph.
     *
     * @param vertex_label The vertex label
     * @return Whether the label is in the graph, and the vertex ID if it is
     */
    VertexID getVertexID(const std::string& vertex_label) const;

    /**
     * Gets the ID of the edge with the given edge label. Assumes there is an edge with that label in the graph.
     *
     * @param edge_label The edge label
     * @return The ID of the edge with the given label
     */
    EdgeID getEdgeID(const std::string& edge_label) const;

    /**
     * Gets the ID of the edge with the given labels for the from and to vertex. Assumes there is an edge with that
     * label in the graph.
     *
     * @param from_vertex_label The label of the from vertex
     * @param to_vertex_label The label of the to vertex
     * @return The ID of the edge with the given labels for the vertices
     */
    EdgeID getEdgeID(const std::string& from_vertex_label, const std::string& to_vertex_label) const;

    /**
     * Gets the edge by ID. Assumes that there is an edge with that ID.
     *
     * @param edge_id The ID of the edge
     * @return The edge
     */
    const Edge& getEdgeByID(EdgeID edge_id) const;

    /**
     * Gets the edge by label. Assumes an edge with that label is in the graph.
     *
     * @param edge_label The edge label
     * @return The edge corresponding to that label
     */
    const Edge& getEdgeByLabel(const std::string& edge_label) const;

    /**
     * Gets the edge by the vertex labels. Assumes an edge between the vertices with those labels is in the graph.
     *
     * @param from_vertex_label The from vertex label
     * @param to_vertex_label The to vertex label
     * @return The edge corresponding to that label
     */
    const Edge& getEdgeByVertexLabels(const std::string& from_vertex_label,
              const std::string& to_vertex_label) const;

    /**
     * Adds a vertex to the graph with the given vertex label. The returned value includes a boolean that is true if
     * there is no existing vertex with that label (and false otherwise), as well as the ID of the vertex added (or
     * already existing).
     *
     * @param vertex_label The vertex label
     * @return Whether the vertex was added, and the ID of the vertex.
     */
    std::pair<bool, VertexID> addVertex(const std::string& vertex_label);

    /**
     * Adds an edge to the graph between the vertices with the given labels. The returned value includes a boolean that
     * is true if there is no existing edge with that label (and false otherwise), as well as the ID of the edge
     * added (or already existing).
     *
     * If vertices with the given labels are not already in the graph, they are added first.
     *
     * An add will fail if there is already an edge in the graph with the given name, there is already an edge between
     * those vertices, or the from and to vertex are the same.
     *
     * @param from_vertex_label The label of the from vertex
     * @param to_vertex_label The label of the to vertex
     * @param cost The cost of the edge
     * @param edge_label The label of the edge
     * @return Whether the edge wa added, and the ID of the edge.
     */
    std::pair<bool, EdgeID> addEdge(const std::string& from_vertex_label, const std::string& to_vertex_label,
              double cost, std::string edge_label = "");

    /**
     * Returns the list of out edges for the vertex at the given ID.
     *
     * @param vertex_id The vertex ID
     * @return The list of out edges for the corresponding vertex
     */
    const std::vector<EdgeID>& getOutEdgeIDs(VertexID vertex_id) const;

    /**
     * Returns the list of in edges for the vertex at the given ID.
     *
     * @param vertex_id The vertex ID
     * @return The list of in edges for the corresponding vertex
     */
    const std::vector<EdgeID>& getInEdgeIDs(VertexID vertex_id) const;

    /**
     * Sorts the lists of outgoing edges by the vertex label of the tail of each edge.
     *
     * @param ascending Whether we are sorting in ascending or descending alphabetical order
     */
    void sortOutEdgesByToVertexLabel(bool ascending = true);

    /**
     * Sorts the list of incoming edges by vertex label of the head of each edge.
     *
     * @param ascending Whether we are sorting in ascending or descending alphabetical order
     */
    void sortInEdgesByFromVertexLabel(bool ascending = true);

private:
    std::vector<std::string> m_vertex_labels;  ///< The list of vertex labels
    std::vector<Edge> m_edges;  ///< The list of edges in the graph

    std::vector<std::vector<EdgeID>> m_out_edges;  ///< The adjancency list. Each vector is the list of the edge IDs of the out edges for the corresponding vertex
    std::vector<std::vector<EdgeID>> m_in_edges;  ///< Each vector is the list of the edge IDs of in edges for the corresponding vertex

    std::unordered_map<std::string, VertexID> m_vertex_ids;  ///< Map of vertex label to vertex ID
    std::unordered_map<std::string, EdgeID> m_edge_ids;  ///< Map of edge label to edge ID
};

#endif  //GRAPH_H_
