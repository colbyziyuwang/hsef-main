#include <algorithm>
#include <gtest/gtest.h>

#include "environments/graph/graph_transitions.h"

/**
 * Creates a fixture with a test graph and transitions based on it.
 *
 * @class GraphTransistionsTest
 */
class GraphTransistionsTests : public ::testing::Test {
protected:
    void SetUp() override {
        base_graph.addVertex("a");
        base_graph.addVertex("b");
        base_graph.addVertex("c");
        base_graph.addVertex("d");

        base_graph.addEdge("a", "b", 1);
        base_graph.addEdge("a", "c", 5);
        base_graph.addEdge("a", "d", 15);
        base_graph.addEdge("b", "c", 2);
        base_graph.addEdge("b", "a", 100);
        base_graph.addEdge("c", "d", 3);
    }

public:
    Graph base_graph;  ///< The graph for the tests
    GraphTransitions transitions = GraphTransitions(base_graph);  ///< The graph transitions object for the tests
};

/**
 * Tests that getting the vertex state for a given label works correctly.
 */
TEST_F(GraphTransistionsTests, getVertexStateTest) {
    GraphState state_a = transitions.getVertexState("a");
    GraphState state_c = transitions.getVertexState("c");

    ASSERT_EQ(state_a.m_vertex_id, base_graph.getVertexID("a"));
    ASSERT_EQ(state_a.m_graph, &base_graph);

    ASSERT_EQ(state_c.m_vertex_id, base_graph.getVertexID("c"));
    ASSERT_EQ(state_c.m_graph, &base_graph);
}

/**
 * Tests that getting the edge action for a given pair of vertices works correctly
 */
TEST_F(GraphTransistionsTests, getEdgeActionTest) {
    GraphAction a_to_c = transitions.getEdgeAction("a", "c");
    GraphAction b_to_c = transitions.getEdgeAction("b", "c");

    ASSERT_EQ(a_to_c.m_edge_id, base_graph.getEdgeID("a", "c"));
    ASSERT_EQ(a_to_c.m_graph, &base_graph);

    ASSERT_EQ(b_to_c.m_edge_id, base_graph.getEdgeID("b", "c"));
    ASSERT_EQ(b_to_c.m_graph, &base_graph);
}

/**
 * Checks that isApplicable works correctly on the test graph.
 */
TEST_F(GraphTransistionsTests, isApplicableTest) {
    GraphState state_a = transitions.getVertexState("a");
    GraphAction a_to_c = transitions.getEdgeAction("a", "c");
    GraphAction b_to_c = transitions.getEdgeAction("b", "c");

    ASSERT_TRUE(transitions.isApplicable(state_a, a_to_c));
    ASSERT_FALSE(transitions.isApplicable(state_a, b_to_c));
}

/**
 * Tests that get action cost works correctly on the test graph.
 */
TEST_F(GraphTransistionsTests, getActionCostTest) {
    GraphState state_a = transitions.getVertexState("a");
    GraphState state_c = transitions.getVertexState("c");

    GraphAction a_to_c = transitions.getEdgeAction("a", "c");
    GraphAction c_to_d = transitions.getEdgeAction("c", "d");

    ASSERT_EQ(transitions.getActionCost(state_a, a_to_c), 5);
    ASSERT_EQ(transitions.getActionCost(state_c, c_to_d), 3);
}

/**
 * Tests that applying the action works correctly on the test graph.
 */
TEST_F(GraphTransistionsTests, applyActionTest) {
    GraphState state = transitions.getVertexState("a");
    GraphAction a_to_c = transitions.getEdgeAction("a", "c");
    transitions.applyAction(state, a_to_c);

    ASSERT_EQ(state.m_vertex_id, base_graph.getVertexID("c"));
    ASSERT_EQ(state.m_graph, &base_graph);

    GraphAction c_to_d = transitions.getEdgeAction("c", "d");
    transitions.applyAction(state, c_to_d);

    ASSERT_EQ(state.m_vertex_id, base_graph.getVertexID("d"));
    ASSERT_EQ(state.m_graph, &base_graph);
}

/**
 * Checks that getActions works correctly on the test graph
 */
TEST_F(GraphTransistionsTests, getActionsTest) {
    auto actions = transitions.getActions(transitions.getVertexState("a"));

    ASSERT_EQ(actions.size(), 3);
    ASSERT_TRUE(std::find(actions.begin(), actions.end(),
                          transitions.getEdgeAction("a", "b")) != actions.end());
    ASSERT_TRUE(std::find(actions.begin(), actions.end(),
                          transitions.getEdgeAction("a", "c")) != actions.end());
    ASSERT_TRUE(std::find(actions.begin(), actions.end(),
                          transitions.getEdgeAction("a", "d")) != actions.end());

    actions = transitions.getActions(transitions.getVertexState("c"));

    ASSERT_EQ(actions.size(), 1);
    ASSERT_EQ(actions[0], transitions.getEdgeAction("c", "d"));
}


/**
 * Checks that getInverse works correctly on the test graph
 */
TEST_F(GraphTransistionsTests, getInverseTest) {
    GraphState state_a = transitions.getVertexState("a");
    GraphAction a_to_b = transitions.getEdgeAction("a", "b");
    GraphAction a_to_c = transitions.getEdgeAction("a", "c");

    ASSERT_TRUE(transitions.getInverse(state_a, a_to_b).has_value());
    ASSERT_FALSE(transitions.getInverse(state_a, a_to_c).has_value());
    ASSERT_EQ(transitions.getInverse(state_a, a_to_b), transitions.getEdgeAction("b", "a"));
}


/**
 * Checks that isValidState works correctly on the test graph
 */
TEST_F(GraphTransistionsTests, isValidStateTest) {
    ASSERT_TRUE(transitions.isValidState(transitions.getVertexState("a")));
    ASSERT_TRUE(transitions.isValidState(transitions.getVertexState("b")));
    ASSERT_FALSE(transitions.isValidState({100, &base_graph}));
}

/**
 * Tests the getAllSettings functionality
 */
TEST_F(GraphTransistionsTests, getSettingsTest) {
    auto settings = transitions.getAllSettings();
    ASSERT_EQ(settings.m_name, GraphTransitions::CLASS_NAME);
    ASSERT_EQ(settings.m_main_settings.size(), 0);
    ASSERT_EQ(settings.m_sub_component_settings.size(), 0);
}
