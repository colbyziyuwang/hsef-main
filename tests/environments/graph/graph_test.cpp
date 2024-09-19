#include <gtest/gtest.h>

#include <algorithm>

#include "environments/graph/graph.h"

/**
 * Tests that adding vertices works correctly, and that checking if a vertex is in the graph also works.
 */
TEST(GraphTests, addVertexAndIsVertexInGraphTest) {
    Graph graph;

    ASSERT_EQ(graph.getNumVertices(), 0u);

    // Test initial add of "a"
    auto [add_succeeded, add_id] = graph.addVertex("a");
    ASSERT_TRUE(add_succeeded);
    ASSERT_EQ(add_id, 0u);
    ASSERT_EQ(graph.getNumVertices(), 1u);

    ASSERT_TRUE(graph.isVertexInGraph("a"));
    ASSERT_FALSE(graph.isVertexInGraph("b"));
    ASSERT_FALSE(graph.isVertexInGraph("c"));

    // Test that adding "b" leaves things correct
    std::tie(add_succeeded, add_id) = graph.addVertex("b");
    ASSERT_TRUE(add_succeeded);
    ASSERT_EQ(add_id, 1u);
    ASSERT_EQ(graph.getNumVertices(), 2u);

    ASSERT_TRUE(graph.isVertexInGraph("a"));
    ASSERT_TRUE(graph.isVertexInGraph("b"));
    ASSERT_FALSE(graph.isVertexInGraph("c"));

    // Test that adding "c" leaves things correct
    std::tie(add_succeeded, add_id) = graph.addVertex("c");
    ASSERT_TRUE(add_succeeded);
    ASSERT_EQ(add_id, 2u);
    ASSERT_EQ(graph.getNumVertices(), 3u);

    ASSERT_TRUE(graph.isVertexInGraph("a"));
    ASSERT_TRUE(graph.isVertexInGraph("b"));
    ASSERT_TRUE(graph.isVertexInGraph("c"));

    // Tests that the second add of "b" is handled correctly
    std::tie(add_succeeded, add_id) = graph.addVertex("b");
    ASSERT_FALSE(add_succeeded);
    ASSERT_EQ(add_id, 1u);
    ASSERT_EQ(graph.getNumVertices(), 3u);

    ASSERT_TRUE(graph.isVertexInGraph("a"));
    ASSERT_TRUE(graph.isVertexInGraph("b"));
    ASSERT_TRUE(graph.isVertexInGraph("c"));
}

/**
 * Checks that getting the vertex ID works as intended.
 */
TEST(GraphTests, getVertexIDTest) {
    Graph graph;

    graph.addVertex("a");
    graph.addVertex("b");
    graph.addVertex("c");

    ASSERT_EQ(graph.getVertexID("a"), 0u);
    ASSERT_EQ(graph.getVertexID("b"), 1u);
    ASSERT_EQ(graph.getVertexID("c"), 2u);
}

/**
 * Tests that getting the vertex label works as intended
 */
TEST(GraphTests, getVertexLabelTest) {
    Graph graph;

    graph.addVertex("a");
    graph.addVertex("b");
    graph.addVertex("c");

    ASSERT_EQ(graph.getVertexLabel(0), "a");
    ASSERT_EQ(graph.getVertexLabel(1), "b");
    ASSERT_EQ(graph.getVertexLabel(2), "c");
}

/**
 * Tests that dding an edge valid edges works for different cases. These include no new vertices need to be added,
 * new vertices need to be added, and inverse requires updating.
 */
TEST(GraphTests, addEdgeTest) {
    Graph graph;

    ASSERT_EQ(graph.getNumEdges(), 0u);

    graph.addVertex("a");
    graph.addVertex("b");

    ASSERT_FALSE(graph.isEdgeInGraph("blah"));

    // Add first edge
    auto [add_succeeded, add_id] = graph.addEdge("a", "b", 0.5, "edge1");

    ASSERT_TRUE(add_succeeded);
    ASSERT_EQ(add_id, 0u);
    ASSERT_EQ(graph.getNumEdges(), 1u);

    Edge added = graph.getEdgeByID(0);
    ASSERT_EQ(added.m_edge_id, 0u);
    ASSERT_EQ(added.m_from_vertex_id, 0u);
    ASSERT_EQ(added.m_to_vertex_id, 1u);
    ASSERT_EQ(added.m_cost, 0.5);
    ASSERT_EQ(added.m_label, "edge1");
    ASSERT_EQ(added.m_inverse_id, 0u);

    // Add edge which also requires vertex add and the default name
    std::tie(add_succeeded, add_id) = graph.addEdge("c", "b", 1.2);
    ASSERT_TRUE(add_succeeded);
    ASSERT_EQ(add_id, 1u);
    ASSERT_EQ(graph.getNumEdges(), 2u);
    ASSERT_TRUE(graph.isVertexInGraph("c"));

    added = graph.getEdgeByID(1);
    ASSERT_EQ(added.m_edge_id, 1u);
    ASSERT_EQ(added.m_from_vertex_id, 2u);
    ASSERT_EQ(added.m_to_vertex_id, 1u);
    ASSERT_EQ(added.m_cost, 1.2);
    ASSERT_EQ(added.m_label, "c->b");
    ASSERT_EQ(added.m_inverse_id, 1u);

    // Add edge which requires two vertices added
    std::tie(add_succeeded, add_id) = graph.addEdge("gh1", "qe2", 1.7);
    ASSERT_TRUE(add_succeeded);
    ASSERT_EQ(add_id, 2u);
    ASSERT_EQ(graph.getNumEdges(), 3u);
    ASSERT_TRUE(graph.isVertexInGraph("gh1"));
    ASSERT_TRUE(graph.isVertexInGraph("qe2"));

    added = graph.getEdgeByID(2);
    ASSERT_EQ(added.m_edge_id, 2u);
    ASSERT_EQ(added.m_from_vertex_id, 3u);
    ASSERT_EQ(added.m_to_vertex_id, 4u);
    ASSERT_EQ(added.m_cost, 1.7);
    ASSERT_EQ(added.m_label, "gh1->qe2");
    ASSERT_EQ(added.m_inverse_id, 2u);

    // Add second edge between existing vertices
    std::tie(add_succeeded, add_id) = graph.addEdge("c", "qe2", 5);
    ASSERT_TRUE(add_succeeded);
    ASSERT_EQ(add_id, 3u);
    ASSERT_EQ(graph.getNumEdges(), 4);

    added = graph.getEdgeByID(3);
    ASSERT_EQ(added.m_edge_id, 3u);
    ASSERT_EQ(added.m_from_vertex_id, 2u);
    ASSERT_EQ(added.m_to_vertex_id, 4u);
    ASSERT_EQ(added.m_cost, 5);
    ASSERT_EQ(added.m_label, "c->qe2");
    ASSERT_EQ(added.m_inverse_id, 3u);

    // Add edge that requires update of inverses
    std::tie(add_succeeded, add_id) = graph.addEdge("qe2", "c", 5);
    ASSERT_TRUE(add_succeeded);
    ASSERT_EQ(add_id, 4u);
    ASSERT_EQ(graph.getNumEdges(), 5u);

    added = graph.getEdgeByID(4);
    ASSERT_EQ(added.m_edge_id, 4u);
    ASSERT_EQ(added.m_from_vertex_id, 4u);
    ASSERT_EQ(added.m_to_vertex_id, 2u);
    ASSERT_EQ(added.m_cost, 5);
    ASSERT_EQ(added.m_label, "qe2->c");
    ASSERT_EQ(added.m_inverse_id, 3u);

    ASSERT_EQ(graph.getEdgeByID(3).m_inverse_id, 4u);

    // Final checks of isEdgeInGraph
    ASSERT_TRUE(graph.isEdgeInGraph("edge1"));
    ASSERT_TRUE(graph.isEdgeInGraph("c->b"));
    ASSERT_TRUE(graph.isEdgeInGraph("gh1->qe2"));
    ASSERT_TRUE(graph.isEdgeInGraph("c->qe2"));
    ASSERT_TRUE(graph.isEdgeInGraph("qe2->c"));
    ASSERT_FALSE(graph.isEdgeInGraph("blah"));

    ASSERT_EQ(graph.getEdgeID("edge1"), 0u);
    ASSERT_EQ(graph.getEdgeID("c->b"), 1u);
    ASSERT_EQ(graph.getEdgeID("gh1->qe2"), 2u);
    ASSERT_EQ(graph.getEdgeID("c->qe2"), 3u);
    ASSERT_EQ(graph.getEdgeID("qe2->c"), 4u);

    ASSERT_TRUE(graph.isEdgeInGraph("a", "b"));
    ASSERT_TRUE(graph.isEdgeInGraph("c", "b"));
    ASSERT_TRUE(graph.isEdgeInGraph("gh1", "qe2"));
    ASSERT_TRUE(graph.isEdgeInGraph("c", "qe2"));
    ASSERT_TRUE(graph.isEdgeInGraph("qe2", "c"));

    ASSERT_FALSE(graph.isEdgeInGraph("b", "a"));
    ASSERT_FALSE(graph.isEdgeInGraph("qe2", "b"));

    ASSERT_EQ(graph.getEdgeID("a", "b"), 0u);
    ASSERT_EQ(graph.getEdgeID("c", "b"), 1u);
    ASSERT_EQ(graph.getEdgeID("gh1", "qe2"), 2u);
    ASSERT_EQ(graph.getEdgeID("c", "qe2"), 3u);
    ASSERT_EQ(graph.getEdgeID("qe2", "c"), 4u);
}

/**
 * Tests that adding edges fails in appropriate cases.
 */
TEST(GraphTests, addInvalidEdgeTest) {
    Graph graph;

    graph.addVertex("a");
    graph.addVertex("b");
    graph.addVertex("c");

    // Reusing same name
    graph.addEdge("a", "b", 0.5, "edge1");
    auto [add_succeeded, add_id] = graph.addEdge("b", "c", 0.5, "edge1");
    ASSERT_FALSE(add_succeeded);

    // Duplicates earlier edge
    std::tie(add_succeeded, add_id) = graph.addEdge("a", "b", 0.5, "edge2");
    ASSERT_FALSE(add_succeeded);

    // Edge is a self-loop
    std::tie(add_succeeded, add_id) = graph.addEdge("b", "b", 0.5, "edge3");
    ASSERT_FALSE(add_succeeded);
}

/**
 * Tests that getting the edge by labels functions works correctly.
 */
TEST(GraphTests, getEdgeByLabelsTest) {
    Graph graph;

    graph.addVertex("a");
    graph.addVertex("b");
    graph.addVertex("c");

    graph.addEdge("a", "b", 0.5);
    graph.addEdge("a", "c", 0.5);
    graph.addEdge("b", "c", 0.5);

    auto edge = graph.getEdgeByLabel("a->b");
    ASSERT_EQ(edge.m_edge_id, 0u);
    edge = graph.getEdgeByVertexLabels("a", "b");
    ASSERT_EQ(edge.m_edge_id, 0u);

    edge = graph.getEdgeByLabel("a->c");
    ASSERT_EQ(edge.m_edge_id, 1u);
    edge = graph.getEdgeByVertexLabels("a", "c");
    ASSERT_EQ(edge.m_edge_id, 1u);

    edge = graph.getEdgeByLabel("b->c");
    ASSERT_EQ(edge.m_edge_id, 2u);
    edge = graph.getEdgeByVertexLabels("b", "c");
    ASSERT_EQ(edge.m_edge_id, 2u);
}

/**
 * Checks that the adjacency lists are updated properly
 */
TEST(GraphTests, adjacencyListUpdatingTest) {
    Graph graph;

    ASSERT_EQ(graph.getNumEdges(), 0);

    graph.addVertex("a");
    graph.addVertex("b");
    graph.addVertex("c");

    ASSERT_TRUE(graph.getInEdgeIDs(0).empty());
    ASSERT_TRUE(graph.getOutEdgeIDs(0).empty());
    ASSERT_TRUE(graph.getInEdgeIDs(1).empty());
    ASSERT_TRUE(graph.getOutEdgeIDs(1).empty());
    ASSERT_TRUE(graph.getInEdgeIDs(2).empty());
    ASSERT_TRUE(graph.getOutEdgeIDs(2).empty());

    graph.addEdge("a", "b", 0.5, "edge1");
    ASSERT_TRUE(graph.getInEdgeIDs(0).empty());
    ASSERT_EQ(graph.getOutEdgeIDs(0).size(), 1u);
    ASSERT_EQ(graph.getOutEdgeIDs(0)[0], 0u);

    ASSERT_EQ(graph.getInEdgeIDs(1).size(), 1u);
    ASSERT_EQ(graph.getInEdgeIDs(1)[0], 0u);
    ASSERT_TRUE(graph.getOutEdgeIDs(1).empty());

    ASSERT_TRUE(graph.getInEdgeIDs(2).empty());
    ASSERT_TRUE(graph.getOutEdgeIDs(2).empty());

    graph.addEdge("a", "c", 0.5, "edge2");
    graph.addEdge("b", "a", 0.5, "edge3");

    ASSERT_EQ(graph.getOutEdgeIDs(0).size(), 2u);
    ASSERT_EQ(graph.getInEdgeIDs(0).size(), 1u);
    ASSERT_EQ(graph.getOutEdgeIDs(1).size(), 1u);
    ASSERT_EQ(graph.getInEdgeIDs(1).size(), 1u);
    ASSERT_TRUE(graph.getOutEdgeIDs(2).empty());
    ASSERT_EQ(graph.getInEdgeIDs(2).size(), 1u);
}

/**
 * Tests that the adjacency list sorting functions works properly
 */
TEST(GraphTests, adjacencyListSortingTest) {
    Graph graph;

    graph.addVertex("a");
    graph.addVertex("b");
    graph.addVertex("c");
    graph.addVertex("d");

    graph.addEdge("a", "d", 0.5);  // Edge 0
    graph.addEdge("a", "b", 0.5);  // Edge 1
    graph.addEdge("a", "c", 0.5);  // Edge 2
    graph.addEdge("d", "c", 0.5);  // Edge 3
    graph.addEdge("b", "c", 0.5);  // Edge 4

    // Sort out edges ascending and check
    graph.sortOutEdgesByToVertexLabel();

    auto a_out = graph.getOutEdgeIDs(0);
    ASSERT_EQ(a_out.size(), 3u);
    ASSERT_EQ(a_out[0], 1u);
    ASSERT_EQ(a_out[1], 2u);
    ASSERT_EQ(a_out[2], 0u);

    ASSERT_EQ(graph.getOutEdgeIDs(1).size(), 1u);
    ASSERT_EQ(graph.getOutEdgeIDs(1)[0], 4u);

    ASSERT_TRUE(graph.getOutEdgeIDs(2).empty());

    ASSERT_EQ(graph.getOutEdgeIDs(3).size(), 1u);
    ASSERT_EQ(graph.getOutEdgeIDs(3)[0], 3u);

    // Sort out edges descending and check
    graph.sortOutEdgesByToVertexLabel(false);

    a_out = graph.getOutEdgeIDs(0);
    ASSERT_EQ(a_out.size(), 3u);
    ASSERT_EQ(a_out[0], 0u);
    ASSERT_EQ(a_out[1], 2u);
    ASSERT_EQ(a_out[2], 1u);

    ASSERT_EQ(graph.getOutEdgeIDs(1).size(), 1u);
    ASSERT_EQ(graph.getOutEdgeIDs(1)[0], 4u);

    ASSERT_TRUE(graph.getOutEdgeIDs(2).empty());

    ASSERT_EQ(graph.getOutEdgeIDs(3).size(), 1u);
    ASSERT_EQ(graph.getOutEdgeIDs(3)[0], 3u);

    // Sort in edges ascending and check
    graph.sortInEdgesByFromVertexLabel();

    ASSERT_TRUE(graph.getInEdgeIDs(0).empty());
    ASSERT_EQ(graph.getInEdgeIDs(1).size(), 1u);
    ASSERT_EQ(graph.getInEdgeIDs(1)[0], 1u);

    auto c_in = graph.getInEdgeIDs(2);
    ASSERT_EQ(c_in.size(), 3u);
    ASSERT_EQ(c_in[0], 2u);
    ASSERT_EQ(c_in[1], 4u);
    ASSERT_EQ(c_in[2], 3u);

    ASSERT_EQ(graph.getInEdgeIDs(3).size(), 1u);
    ASSERT_EQ(graph.getInEdgeIDs(3)[0], 0u);

    // Sort in edges descending and check
    graph.sortInEdgesByFromVertexLabel(false);

    ASSERT_TRUE(graph.getInEdgeIDs(0).empty());
    ASSERT_EQ(graph.getInEdgeIDs(1).size(), 1u);
    ASSERT_EQ(graph.getInEdgeIDs(1)[0], 1u);

    c_in = graph.getInEdgeIDs(2);
    ASSERT_EQ(c_in.size(), 3u);
    ASSERT_EQ(c_in[0], 3u);
    ASSERT_EQ(c_in[1], 4u);
    ASSERT_EQ(c_in[2], 2u);

    ASSERT_EQ(graph.getInEdgeIDs(3).size(), 1u);
    ASSERT_EQ(graph.getInEdgeIDs(3)[0], 0u);
}