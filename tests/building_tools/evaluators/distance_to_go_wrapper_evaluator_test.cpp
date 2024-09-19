#include <gtest/gtest.h>

#include "building_tools/evaluators/distance_to_go_wrapper_evaluator.h"
#include "building_tools/evaluators/evaluator_tools_terms.h"
#include "engines/engine_components/node_containers/node_list.h"
#include "environments/grid_pathfinding/grid_location.h"
#include "environments/grid_pathfinding/grid_pathfinding_lifecost_heuristic.h"
#include "environments/grid_pathfinding/grid_pathfinding_transitions.h"
#include "test_helpers.h"

/**
 * Tests the wrapper works when the distance-to-go evaluator is called directly. Needs to propagate call down to the
 * base cost-to-go heuristic.
 */
TEST(DistanceToGoWrapperEvaluatorTests, mapPathfindingLifecostDirectTest) {
    GridLocation goal_state(100, 2);

    GridPathfindingLifecostHeuristic lifecost;
    lifecost.setGoalState(goal_state);

    DistanceToGoWrapperEvaluator<GridLocation, GridDirection> d_to_go{lifecost};

    GridLocation state(0, 2);

    // Call distance-to-go directly
    ASSERT_TRUE(checkStateEvaluation(d_to_go, state, 104, false));

    ASSERT_NEAR(lifecost.getLastNodeEval(), 4, 0.00001);
    ASSERT_FALSE(lifecost.isLastNodeADeadEnd());
}

/**
 * Tests the wrapper works when the cost-to-go evaluator is called first, and then used by the distance-to-go
 * evaluator.
 */
TEST(DistanceToGoWrapperEvaluatorTests, mapPathfindingLifecostBackwardTest) {
    GridLocation goal_state(100, 2);

    GridPathfindingLifecostHeuristic lifecost;
    lifecost.setGoalState(goal_state);

    DistanceToGoWrapperEvaluator<GridLocation, GridDirection> d_to_go{lifecost};
    NodeList<GridLocation, GridDirection> nodes;
    d_to_go.setNodeContainer(nodes);

    GridLocation state(0, 2);

    // Call distance-to-go directly
    lifecost.prepareToEvaluate();
    d_to_go.prepareToEvaluate();


    NodeID node_id = nodes.addNode(state);

    lifecost.evaluate(node_id);
    d_to_go.evaluate(node_id);

    ASSERT_NEAR(d_to_go.getLastNodeEval(), 104, 0.00001);
    ASSERT_FALSE(d_to_go.isLastNodeADeadEnd());
}

TEST(DistanceToGoWrapperEvaluatorTests, getSettingsTest) {
    using namespace evaluatorToolsTerms;
    GridPathfindingLifecostHeuristic lifecost;
    DistanceToGoWrapperEvaluator<GridLocation, GridDirection> d_to_go{lifecost};

    auto settings = d_to_go.getAllSettings();
    ASSERT_EQ(settings.m_name, lifecost.CLASS_NAME + d_to_go.CLASS_NAME_POSTFIX);
    ASSERT_EQ(settings.m_main_settings.size(), 0);
    ASSERT_EQ(settings.m_sub_component_settings.size(), 1);
    ASSERT_EQ(settings.m_sub_component_settings.at("base_evaluator").m_name, lifecost.CLASS_NAME);
}