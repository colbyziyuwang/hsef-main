#include <gtest/gtest.h>

#include "engines/engine_components/eval_functions/eval_function_terms.h"
#include "engines/engine_components/eval_functions/weighted_f_cost_evaluator.h"
#include "engines/engine_components/node_containers/node_list.h"
#include "environments/grid_pathfinding/grid_location.h"
#include "environments/grid_pathfinding/grid_pathfinding_action.h"
#include "environments/grid_pathfinding/grid_pathfinding_manhattan_heuristic.h"
#include "test_helpers.h"

class WeightedFCostEvaluatorTests : public ::testing::Test {
protected:
    void SetUp() override {
        manhattan.setGoalState(100, 100);
    }

public:
    NodeList<GridLocation, GridDirection> nodes;
    GridPathfindingManhattanHeuristic manhattan;
};

/**
 * Tests that the correct values are returned for both a g-cost of 0.0 and non-zero value when weight is 1.0.
 */
TEST_F(WeightedFCostEvaluatorTests, mapPathfindingUnWeightedFCostTest) {
    WeightedFCostEvaluator<GridLocation, GridDirection> evaluator(manhattan, 1.0);
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
 * Tests that the correct values are returned when the weight is 3.0.
 */
TEST_F(WeightedFCostEvaluatorTests, mapPathfindingWeightedFCostTest) {
    WeightedFCostEvaluator<GridLocation, GridDirection> evaluator(manhattan, 3.0);
    evaluator.setNodeContainer(nodes);

    GridLocation state(50, 50);
    NodeID node1_id = nodes.addNode(state, 1, 0.0, GridDirection::east, 1);

    // Checks with g-value of 0.0
    ASSERT_TRUE(checkNodeEvaluation(evaluator, node1_id, 300.0, false));

    GridLocation state2(145, 123);
    NodeID node2_id = nodes.addNode(state2, 2, 52.7, GridDirection::north, 1);

    // Checks with g-value of 52.7
    ASSERT_TRUE(checkNodeEvaluation(evaluator, node2_id, 256.7, false));
}

/**
 * Checks that re-evaluate works by changing the g-value and cached h-value
 */
TEST_F(WeightedFCostEvaluatorTests, reEvaluateTest) {
    WeightedFCostEvaluator<GridLocation, GridDirection> evaluator(manhattan, 2.0);
    evaluator.setNodeContainer(nodes);

    GridLocation state(50, 50);
    NodeID node1_id = nodes.addNode(state, 1, 0.0, GridDirection::east, 1);

    // Checks with g-value of 0.0
    ASSERT_TRUE(checkNodeEvaluation(evaluator, node1_id, 200.0, false));

    // Changes the g-value to 20
    nodes.setGValue(node1_id, 20);

    ASSERT_TRUE(checkNodeEvaluation(evaluator, node1_id, 220.0, false, true));

    // changes the g-value and the cached h-value
    nodes.setGValue(node1_id, 35);
    manhattan.setCachedEval(node1_id, 21);

    ASSERT_TRUE(checkNodeEvaluation(evaluator, node1_id, 77.0, false, true));
}

/**
 * Tests the getAllSettings functionality
 */
TEST_F(WeightedFCostEvaluatorTests, getSettingsTest) {
    using namespace evalFunctionTerms;
    WeightedFCostEvaluator<GridLocation, GridDirection> evaluator(manhattan, 3.0);

    evaluator.setWeight(3.0);
    auto settings = evaluator.getAllSettings();
    ASSERT_EQ(settings.m_name, evaluator.CLASS_NAME);
    ASSERT_EQ(settings.m_main_settings.size(), 1);
    ASSERT_EQ(settings.m_main_settings.at(SETTING_HEURISTIC_WEIGHT), "3.0");

    auto& subcomponent_settings = settings.m_sub_component_settings;
    ASSERT_EQ(subcomponent_settings.size(), 1);
    ASSERT_EQ(subcomponent_settings.at(SETTING_HEURISTIC).m_name, manhattan.CLASS_NAME);
}
