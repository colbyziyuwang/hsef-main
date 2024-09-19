#include <gtest/gtest.h>

#include "engines/engine_components/eval_functions/g_cost_evaluator.h"
#include "engines/engine_components/node_containers/node_list.h"
#include "environments/grid_pathfinding/grid_location.h"
#include "environments/grid_pathfinding/grid_pathfinding_action.h"
#include "search_basics/node_container.h"
#include "test_helpers.h"

class GCostEvaluatorTests : public ::testing::Test {
protected:
    void SetUp() override {
        g_cost.setNodeContainer(nodes);

        nodes.setGValue(node1_id, 0);
        nodes.setGValue(node2_id, 1);
        nodes.setGValue(node3_id, 1);
        nodes.setGValue(node4_id, 1);
        nodes.setGValue(node5_id, 1);
        nodes.setGValue(node6_id, 2);
        nodes.setGValue(node7_id, 2);
        nodes.setGValue(node8_id, 2);
    }

public:
    NodeList<GridLocation, GridDirection> nodes;
    GCostEvaluator<GridLocation, GridDirection> g_cost;

    NodeID node1_id = nodes.addNode(GridLocation(10, 12));
    NodeID node2_id = nodes.addNode(GridLocation(11, 12));
    NodeID node3_id = nodes.addNode(GridLocation(9, 12));
    NodeID node4_id = nodes.addNode(GridLocation(10, 11));
    NodeID node5_id = nodes.addNode(GridLocation(10, 13));
    NodeID node6_id = nodes.addNode(GridLocation(9, 13));
    NodeID node7_id = nodes.addNode(GridLocation(9, 11));
    NodeID node8_id = nodes.addNode(GridLocation(9, 10));
};

/**
 * Tests the G-cost evaluator works in a step-by-step way.
 */
TEST_F(GCostEvaluatorTests, stepByStepGCostTest) {
    g_cost.prepareToEvaluate();
    ASSERT_FALSE(g_cost.isEvalComputed());

    g_cost.evaluate(node2_id);
    ASSERT_TRUE(g_cost.isEvalComputed());
    ASSERT_EQ(g_cost.getIDofLastEvaluatedNode(), node2_id);

    g_cost.prepareToEvaluate();
    ASSERT_FALSE(g_cost.isEvalComputed());
    g_cost.reEvaluate(node3_id);
    ASSERT_TRUE(g_cost.isEvalComputed());
    ASSERT_EQ(g_cost.getIDofLastEvaluatedNode(), node3_id);
}

/**
 * Tests that evaluate gets the right values at every step.
 */
TEST_F(GCostEvaluatorTests, basicGCostEvaluateTest) {
    ASSERT_TRUE(checkNodeEvaluation(g_cost, node1_id, 0.0, false));
    ASSERT_TRUE(checkNodeEvaluation(g_cost, node2_id, 1.0, false));
    ASSERT_TRUE(checkNodeEvaluation(g_cost, node3_id, 1.0, false));
    ASSERT_TRUE(checkNodeEvaluation(g_cost, node4_id, 1.0, false));
    ASSERT_TRUE(checkNodeEvaluation(g_cost, node5_id, 1.0, false));
    ASSERT_TRUE(checkNodeEvaluation(g_cost, node6_id, 2.0, false));
    ASSERT_TRUE(checkNodeEvaluation(g_cost, node7_id, 2.0, false));
    ASSERT_TRUE(checkNodeEvaluation(g_cost, node8_id, 2.0, false));
}

/**
 * Tests that re-evaluate gets the right values at every step.
 */
TEST_F(GCostEvaluatorTests, basicGCostReEvaluateTest) {
    ASSERT_TRUE(checkNodeEvaluation(g_cost, node1_id, 0.0, false, true));
    ASSERT_TRUE(checkNodeEvaluation(g_cost, node2_id, 1.0, false, true));
    ASSERT_TRUE(checkNodeEvaluation(g_cost, node3_id, 1.0, false, true));
    ASSERT_TRUE(checkNodeEvaluation(g_cost, node4_id, 1.0, false, true));
    ASSERT_TRUE(checkNodeEvaluation(g_cost, node5_id, 1.0, false, true));
    ASSERT_TRUE(checkNodeEvaluation(g_cost, node6_id, 2.0, false, true));
    ASSERT_TRUE(checkNodeEvaluation(g_cost, node7_id, 2.0, false, true));
    ASSERT_TRUE(checkNodeEvaluation(g_cost, node8_id, 2.0, false, true));
}

/**
 * Tests that settings returns the right things.
 */
TEST_F(GCostEvaluatorTests, getSettingsTest) {

    auto settings = g_cost.getAllSettings();
    ASSERT_EQ(settings.m_name, g_cost.CLASS_NAME);
    ASSERT_EQ(settings.m_main_settings.size(), 0);
    ASSERT_EQ(settings.m_sub_component_settings.size(), 0);
}
