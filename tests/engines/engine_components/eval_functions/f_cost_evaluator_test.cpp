#include <gtest/gtest.h>

#include "engines/engine_components/eval_functions/eval_function_terms.h"
#include "engines/engine_components/eval_functions/f_cost_evaluator.h"
#include "engines/engine_components/node_containers/node_list.h"
#include "environments/grid_pathfinding/grid_location.h"
#include "environments/grid_pathfinding/grid_pathfinding_action.h"
#include "environments/grid_pathfinding/grid_pathfinding_manhattan_heuristic.h"
#include "test_helpers.h"

class FCostEvaluatorTests : public ::testing::Test {
protected:
    void SetUp() override {
        manhattan.setGoalState(100, 100);
    }

public:
    NodeList<GridLocation, GridDirection> nodes;
    GridPathfindingManhattanHeuristic manhattan;
};

/**
 * Tests that the correct values are returned for both a g-cost of 0.0 and non-zero value
 */
TEST_F(FCostEvaluatorTests, mapPathfindingFCostTest) {
    FCostEvaluator<GridLocation, GridDirection> evaluator(manhattan);
    evaluator.setNodeContainer(nodes);

    GridLocation state(50, 50);
    NodeID node1_id = nodes.addNode(state, 1, 0.0, GridDirection::east, 1);

    // Checks with g-value of 0.0
    ASSERT_TRUE(checkNodeEvaluation(evaluator, node1_id, 100.0, false));

    GridLocation state2(145, 123);
    NodeID node2_id = nodes.addNode(state2, 2, 52.7, GridDirection::north, 1);

    // Checks with g-value of 52.7
    ASSERT_TRUE(checkNodeEvaluation(evaluator, node2_id, 120.7, false));
}

/**
 * Tests the re-evaluate including g-value changes and h-value changes.
 */
TEST_F(FCostEvaluatorTests, reEvaluateTest) {
    FCostEvaluator<GridLocation, GridDirection> evaluator(manhattan);
    evaluator.setNodeContainer(nodes);

    GridLocation state(50, 50);
    NodeID node1_id = nodes.addNode(state, 1, 0.0, GridDirection::east, 1);

    // Checks with g-value of 0.0
    ASSERT_TRUE(checkNodeEvaluation(evaluator, node1_id, 100.0, false));

    // Changes the g-value and calls re-evaluate
    nodes.setGValue(node1_id, 20);
    ASSERT_TRUE(checkNodeEvaluation(evaluator, node1_id, 120.0, false, true));

    // changes the g-value and the cached h-value
    nodes.setGValue(node1_id, 35);
    manhattan.setCachedEval(node1_id, 21);
    ASSERT_TRUE(checkNodeEvaluation(evaluator, node1_id, 56.0, false, true));
}

/**
 * Tests the getAllSettings functionality
 */
TEST_F(FCostEvaluatorTests, getSettingsTest) {
    using namespace evalFunctionTerms;
    FCostEvaluator<GridLocation, GridDirection> evaluator(manhattan);

    auto settings = evaluator.getAllSettings();
    ASSERT_EQ(settings.m_name, evaluator.CLASS_NAME);
    ASSERT_EQ(settings.m_main_settings.size(), 0);

    auto& subcomponent_settings = settings.m_sub_component_settings;
    ASSERT_EQ(subcomponent_settings.size(), 1);
    ASSERT_TRUE(subcomponent_settings.find(SETTING_HEURISTIC) != subcomponent_settings.end());
    ASSERT_EQ(subcomponent_settings[SETTING_HEURISTIC].m_name, manhattan.CLASS_NAME);
}