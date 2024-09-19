#include <gtest/gtest.h>

#include "building_tools/evaluators/constant_heuristic.h"
#include "engines/engine_components/node_containers/node_list.h"
#include "environments/grid_pathfinding/grid_location.h"
#include "environments/grid_pathfinding/grid_pathfinding_action.h"
#include "search_basics/node_container.h"
#include "test_helpers.h"

/**
 * Tests default constructor to ensure 0.0 is returned.
 */
TEST(ConstantHeuristicTests, defaultConstructorTest) {
    GridLocation state1(123, 839);
    GridLocation state2(69, 420);

    ConstantHeuristic<GridLocation, GridDirection> heuristic;

    ASSERT_TRUE(checkStateEvaluation(heuristic, state1, 0.0, false));
    ASSERT_TRUE(checkStateEvaluation(heuristic, state2, 0.0, false));
}

/**
 * Tests constructor works when given non-zero constant
 */
TEST(ConstantHeuristicTests, specifyConstructorTest) {
    GridLocation state1(123, 839);
    GridLocation state2(69, 420);

    ConstantHeuristic<GridLocation, GridDirection> heuristic(5.0);

    ASSERT_TRUE(checkStateEvaluation(heuristic, state1, 5.0, false));
    ASSERT_TRUE(checkStateEvaluation(heuristic, state2, 5.0, false));
}

/**
 * Tests that re-evaluate works as expected.
 */
TEST(ConstantHeuristicTests, reEvaluateTest) {
    GridLocation state1(123, 839);
    GridLocation state2(69, 420);

    ConstantHeuristic<GridLocation, GridDirection> heuristic(5.0);

    NodeList<GridLocation, GridDirection> node_list;
    heuristic.setNodeContainer(node_list);

    NodeID node_id1 = node_list.addNode(state1);
    NodeID node_id2 = node_list.addNode(state2);

    heuristic.prepareToEvaluate();
    heuristic.evaluate(node_id1);

    heuristic.prepareToEvaluate();
    heuristic.evaluate(node_id2);

    // Tests re-evaluate hasn't changed anything
    ASSERT_TRUE(checkNodeEvaluation(heuristic, node_id1, 5.0, false, true));
    ASSERT_TRUE(checkNodeEvaluation(heuristic, node_id2, 5.0, false, true));

    // Manually change cached value and check it hasn't changed anything
    heuristic.setCachedEval(node_id1, 10.0);
    ASSERT_TRUE(checkNodeEvaluation(heuristic, node_id1, 10.0, false, true));
}

/**
 * Tests that getSettings returns correct values
 */
TEST(ConstantHeuristicTests, getSettingsTestForConstant) {
    ConstantHeuristic<GridLocation, GridDirection> heuristic(2);

    auto settings = heuristic.getAllSettings();
    ASSERT_EQ(settings.m_name, heuristic.CLASS_NAME);
    ASSERT_EQ(settings.m_main_settings.size(), 1);
    ASSERT_EQ(settings.m_main_settings["default_h_value"], "2.0");

    ASSERT_EQ(settings.m_sub_component_settings.size(), 0);
}