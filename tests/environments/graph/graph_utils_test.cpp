#include <gtest/gtest.h>

#include "environments/graph/graph.h"
#include "environments/graph/graph_utils.h"

/**
 * Tests that getting a graph from an adjacency matrix works correctly.
 */
TEST(GraphUtilsTests, getGraphFromCSVAdjacencyMatrixTest) {
    std::stringstream csv(";A;B;C;D;E\nA;;4;3;2;1\nB;;;;;\nC;0;0;;0;0\nD;;1;;;\nE;7;8;9;10;");
    Graph graph = getGraphFromCSVAdjacencyMatrix(csv);

    ASSERT_EQ(graph.getNumVertices(), 5);
    ASSERT_EQ(graph.getNumEdges(), 13);

    ASSERT_TRUE(graph.isVertexInGraph("A"));
    ASSERT_TRUE(graph.isVertexInGraph("B"));
    ASSERT_TRUE(graph.isVertexInGraph("C"));
    ASSERT_TRUE(graph.isVertexInGraph("D"));
    ASSERT_TRUE(graph.isVertexInGraph("E"));
    ASSERT_FALSE(graph.isVertexInGraph("x"));

    ASSERT_TRUE(graph.isEdgeInGraph("A", "B"));
    ASSERT_TRUE(graph.isEdgeInGraph("A", "C"));
    ASSERT_TRUE(graph.isEdgeInGraph("A", "D"));
    ASSERT_TRUE(graph.isEdgeInGraph("A", "E"));
    ASSERT_TRUE(graph.isEdgeInGraph("C", "A"));
    ASSERT_TRUE(graph.isEdgeInGraph("C", "B"));
    ASSERT_TRUE(graph.isEdgeInGraph("C", "D"));
    ASSERT_TRUE(graph.isEdgeInGraph("C", "E"));
    ASSERT_TRUE(graph.isEdgeInGraph("D", "B"));
    ASSERT_TRUE(graph.isEdgeInGraph("E", "A"));
    ASSERT_TRUE(graph.isEdgeInGraph("E", "B"));
    ASSERT_TRUE(graph.isEdgeInGraph("E", "C"));
    ASSERT_TRUE(graph.isEdgeInGraph("E", "D"));

    ASSERT_EQ(graph.getEdgeByLabel("A->B").m_cost, 4);
    ASSERT_EQ(graph.getEdgeByLabel("A->C").m_cost, 3);
    ASSERT_EQ(graph.getEdgeByLabel("A->D").m_cost, 2);
    ASSERT_EQ(graph.getEdgeByLabel("A->E").m_cost, 1);
    ASSERT_EQ(graph.getEdgeByLabel("C->A").m_cost, 0);
    ASSERT_EQ(graph.getEdgeByLabel("C->B").m_cost, 0);
    ASSERT_EQ(graph.getEdgeByLabel("C->D").m_cost, 0);
    ASSERT_EQ(graph.getEdgeByLabel("C->E").m_cost, 0);
    ASSERT_EQ(graph.getEdgeByLabel("D->B").m_cost, 1);
    ASSERT_EQ(graph.getEdgeByLabel("E->A").m_cost, 7);
    ASSERT_EQ(graph.getEdgeByLabel("E->B").m_cost, 8);
    ASSERT_EQ(graph.getEdgeByLabel("E->C").m_cost, 9);
    ASSERT_EQ(graph.getEdgeByLabel("E->D").m_cost, 10);
}

/**
 * Tests that getting a graph from a CSV adjacency list works when no costs are included.
 */
TEST(GraphUtilsTests, getGraphFromCSVAdjacencyListWithoutCostsTest) {
    std::stringstream csv("a;b\nb;c;d");
    Graph graph = getGraphFromCSVAdjacencyList(csv);

    ASSERT_EQ(graph.getNumVertices(), 4);
    ASSERT_EQ(graph.getNumEdges(), 3);

    ASSERT_TRUE(graph.isVertexInGraph("a"));
    ASSERT_TRUE(graph.isVertexInGraph("b"));
    ASSERT_TRUE(graph.isVertexInGraph("c"));
    ASSERT_TRUE(graph.isVertexInGraph("d"));
    ASSERT_FALSE(graph.isVertexInGraph("z"));

    ASSERT_TRUE(graph.isEdgeInGraph("a", "b"));
    ASSERT_TRUE(graph.isEdgeInGraph("b", "c"));
    ASSERT_TRUE(graph.isEdgeInGraph("b", "d"));

    ASSERT_EQ(graph.getEdgeByLabel("a->b").m_cost, 1);
    ASSERT_EQ(graph.getEdgeByLabel("b->c").m_cost, 1);
    ASSERT_EQ(graph.getEdgeByLabel("b->d").m_cost, 1);
}

/**
 * Tests that getting a graph from a CSV adjacency list works when costs are included.
 */
TEST(GraphUtilsTests, getGraphFromCSVAdjacencyListWithCostsTest) {
    std::stringstream csv("a;b 17\nb;c 99;d 6;z");
    Graph graph = getGraphFromCSVAdjacencyList(csv);

    ASSERT_EQ(graph.getNumVertices(), 5);
    ASSERT_EQ(graph.getNumEdges(), 4);

    ASSERT_TRUE(graph.isVertexInGraph("a"));
    ASSERT_TRUE(graph.isVertexInGraph("b"));
    ASSERT_TRUE(graph.isVertexInGraph("c"));
    ASSERT_TRUE(graph.isVertexInGraph("d"));
    ASSERT_TRUE(graph.isVertexInGraph("z"));

    ASSERT_TRUE(graph.isEdgeInGraph("a", "b"));
    ASSERT_TRUE(graph.isEdgeInGraph("b", "c"));
    ASSERT_TRUE(graph.isEdgeInGraph("b", "d"));
    ASSERT_TRUE(graph.isEdgeInGraph("b", "z"));

    ASSERT_EQ(graph.getEdgeByLabel("a->b").m_cost, 17);
    ASSERT_EQ(graph.getEdgeByLabel("b->c").m_cost, 99);
    ASSERT_EQ(graph.getEdgeByLabel("b->d").m_cost, 6);
    ASSERT_EQ(graph.getEdgeByLabel("b->z").m_cost, 1);
}
