#include <gtest/gtest.h>

#include "environments/graph/graph.h"
#include "environments/graph/graph_transitions.h"
#include "environments/graph/vertex_hash_function.h"

/**
 * Tests hash value for a given graph
 */
TEST(VertexHashFunctionTests, getVertexHashValue) {
    Graph graph;

    graph.addVertex("a");
    graph.addVertex("b");
    graph.addVertex("c");

    GraphTransitions transitions(graph);
    VertexHashFunction hasher;

    ASSERT_EQ(hasher.getHashValue(transitions.getVertexState("a")), 0u);
    ASSERT_EQ(hasher.getHashValue(transitions.getVertexState("b")), 1u);
    ASSERT_EQ(hasher.getHashValue(transitions.getVertexState("c")), 2u);
}

/**
 * Checks that getAllSettings returns the correct values.
 */
TEST(VertexHashFunctionTests, getSettingsTest) {
    VertexHashFunction hasher;

    auto settings = hasher.getAllSettings();
    ASSERT_EQ(settings.m_name, VertexHashFunction::CLASS_NAME);
    ASSERT_EQ(settings.m_main_settings.size(), 0);
    ASSERT_EQ(settings.m_sub_component_settings.size(), 0);
}