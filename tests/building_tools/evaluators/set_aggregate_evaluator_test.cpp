#include <gtest/gtest.h>

#include "building_tools/evaluators/constant_heuristic.h"
#include "building_tools/evaluators/evaluator_tools_terms.h"
#include "building_tools/evaluators/hash_map_heuristic.h"
#include "building_tools/evaluators/set_aggregate_evaluator.h"
#include "building_tools/hashing/state_string_hash_function.h"
#include "engines/engine_components/node_containers/node_list.h"
#include "environments/grid_pathfinding/grid_location.h"
#include "environments/grid_pathfinding/grid_pathfinding_action.h"
#include "environments/grid_pathfinding/grid_pathfinding_euclidean_heuristic.h"
#include "environments/grid_pathfinding/grid_pathfinding_octile_heuristic.h"
#include "search_basics/node_container.h"
#include "test_helpers.h"

#include <algorithm>
#include <string>

/**
 * Tests that the correct values and default values are used after states are added. Map pathfinding is used for the
 * test. Specifies a custom aggregator function (minimum).
 */
TEST(SetAggregateEvaluatorTest, mapPathfindingMinTest) {
    GridLocation state1(123, 839);
    GridLocation state2(69, 420);
    GridLocation state3(901, 2048);

    StateStringHashFunction<GridLocation> hash_function;
    HashMapHeuristic<GridLocation, GridDirection, std::string> heuristic1(hash_function, 32.0);
    heuristic1.addHeuristicValue(state1, 55.0, false);
    heuristic1.addHeuristicValue(state2, 11.0, true);

    HashMapHeuristic<GridLocation, GridDirection, std::string> heuristic2(hash_function, 32.0);
    heuristic2.addHeuristicValue(state1, 69.0, false);
    heuristic2.addHeuristicValue(state2, 5.0, true);

    HashMapHeuristic<GridLocation, GridDirection, std::string> heuristic3(hash_function, 32.0);
    heuristic3.addHeuristicValue(state1, 45.0, false);
    heuristic3.addHeuristicValue(state2, 5.0, true);
    heuristic3.addHeuristicValue(state3, 777.0, true);

    auto min = [](double first, double second) {
        return std::min(first, second);
    };
    SetAggregateEvaluator<GridLocation, GridDirection> min_evaluator({&heuristic1, &heuristic2, &heuristic3}, min, "min");

    ASSERT_TRUE(checkStateEvaluation(min_evaluator, state1, 45.0, false));
    ASSERT_TRUE(checkStateEvaluation(min_evaluator, state2, 5.0, true));
    ASSERT_TRUE(checkStateEvaluation(min_evaluator, state3, 32.0, true));
}

/**
 * Tests that re-evaluate works as expected.
 */
TEST(SetAggregateEvaluatorTest, reEvaluateTest) {
    GridLocation state1(123, 839);

    // Using two constant heuristics for the re-evaluate test
    StateStringHashFunction<GridLocation> hash_function;
    ConstantHeuristic<GridLocation, GridDirection> constant1(1.0);
    ConstantHeuristic<GridLocation, GridDirection> constant5(5.0);

    NodeList<GridLocation, GridDirection> node_list;
    constant1.setNodeContainer(node_list);
    constant5.setNodeContainer(node_list);

    node_list.addNode(state1);

    NodeID node_id1 = node_list.addNode(state1);

    auto sum = [](double first, double second) {
        return first + second;
    };
    // Creating 3 sum evaluators that all differ by re-evaluation type
    SetAggregateEvaluator<GridLocation, GridDirection> sum_no_reevaluate({&constant1, &constant5}, sum, "sum", SetAggregateReEvaluateType::none);
    SetAggregateEvaluator<GridLocation, GridDirection> sum_standard_reevaluate({&constant1, &constant5}, sum, "sum", SetAggregateReEvaluateType::standard);
    SetAggregateEvaluator<GridLocation, GridDirection> sum_max_reevaluate({&constant1, &constant5}, sum, "sum", SetAggregateReEvaluateType::max);

    // Sets the node containers
    sum_no_reevaluate.setNodeContainer(node_list);
    sum_standard_reevaluate.setNodeContainer(node_list);
    sum_max_reevaluate.setNodeContainer(node_list);

    // Evaluates a node
    sum_no_reevaluate.prepareToEvaluate();
    sum_standard_reevaluate.prepareToEvaluate();
    sum_max_reevaluate.prepareToEvaluate();

    sum_no_reevaluate.evaluate(node_id1);
    sum_standard_reevaluate.evaluate(node_id1);
    sum_max_reevaluate.evaluate(node_id1);

    // Since it is just an evaluation, they should all return 6
    ASSERT_EQ(sum_no_reevaluate.getCachedEval(node_id1), 6);
    ASSERT_EQ(sum_standard_reevaluate.getCachedEval(node_id1), 6);
    ASSERT_EQ(sum_max_reevaluate.getCachedEval(node_id1), 6);

    // Changes sub-evaluation to 6
    constant5.setCachedEval(node_id1, 2);

    sum_no_reevaluate.prepareToEvaluate();
    sum_standard_reevaluate.prepareToEvaluate();
    sum_max_reevaluate.prepareToEvaluate();

    sum_no_reevaluate.reEvaluate(node_id1);
    sum_standard_reevaluate.reEvaluate(node_id1);
    sum_max_reevaluate.reEvaluate(node_id1);

    // Check that we get the right values now
    ASSERT_EQ(sum_no_reevaluate.getCachedEval(node_id1), 6);  // Shouldn't update since no new calculation
    ASSERT_EQ(sum_standard_reevaluate.getCachedEval(node_id1), 3);  // Should update
    ASSERT_EQ(sum_max_reevaluate.getCachedEval(node_id1), 6);  // Shouldn't update since previous value was bigger

    // Changes sub-evaluation to 6
    constant5.setCachedEval(node_id1, 6);

    sum_no_reevaluate.prepareToEvaluate();
    sum_standard_reevaluate.prepareToEvaluate();
    sum_max_reevaluate.prepareToEvaluate();

    sum_no_reevaluate.reEvaluate(node_id1);
    sum_standard_reevaluate.reEvaluate(node_id1);
    sum_max_reevaluate.reEvaluate(node_id1);

    ASSERT_EQ(sum_no_reevaluate.getCachedEval(node_id1), 6);  // Shouldn't update since no new calculation
    ASSERT_EQ(sum_standard_reevaluate.getCachedEval(node_id1), 7);  // Should update
    ASSERT_EQ(sum_max_reevaluate.getCachedEval(node_id1), 7);  // Should update since new value is higher
}

/**
 * Tests the singleEvaluator
 */
TEST(SetAggregateEvaluatorTest, singleEvaluatorTest) {
    GridLocation state1(123, 839);
    GridLocation state2(69, 420);
    GridLocation state3(901, 2048);

    StateStringHashFunction<GridLocation> hash_function;
    HashMapHeuristic<GridLocation, GridDirection, std::string> heuristic1(hash_function, 32.0);
    heuristic1.addHeuristicValue(state1, 55.0, false);
    heuristic1.addHeuristicValue(state2, 11.0, true);

    auto min = [](double first, double second) {
        return std::min(first, second);
    };
    SetAggregateEvaluator<GridLocation, GridDirection> min_evaluator({&heuristic1}, min, "min");

    ASSERT_TRUE(checkStateEvaluation(min_evaluator, state1, 55.0, false));
    ASSERT_TRUE(checkStateEvaluation(min_evaluator, state2, 11.0, true));
    ASSERT_TRUE(checkStateEvaluation(min_evaluator, state3, 32.0, false));
}

/**
 * Tests the getAllSettings functionality
 */
TEST(SetAggregateEvaluatorTest, getSettingsTest) {
    GridLocation goal_state(120, 120);
    GridPathfindingEuclideanHeuristic euclidean(goal_state);
    GridPathfindingOctileHeuristic octile(goal_state);

    auto min = [](double first, double second) {
        return std::min(first, second);
    };

    SetAggregateEvaluator<GridLocation, GridDirection> min_evaluator({&euclidean, &octile}, min, "min");

    auto settings = min_evaluator.getAllSettings();
    ASSERT_EQ(settings.m_name, min_evaluator.CLASS_NAME);
    ASSERT_EQ(settings.m_main_settings.size(), 1);
    ASSERT_EQ(settings.m_main_settings.at(evaluatorToolsTerms::SETTING_AGGREGATION_OPERATOR), "min");
    ASSERT_EQ(settings.m_sub_component_settings.size(), 2);

    auto& euclidean_settings = settings.m_sub_component_settings.at("subevaluator_0");
    ASSERT_EQ(euclidean_settings.m_name, GridPathfindingEuclideanHeuristic::CLASS_NAME);
    ASSERT_EQ(euclidean_settings.m_main_settings.size(), 1);
    ASSERT_EQ(euclidean_settings.m_main_settings.at("goal_state"), "(120, 120)");
    ASSERT_EQ(euclidean_settings.m_sub_component_settings.size(), 0);

    auto& octile_settings = settings.m_sub_component_settings.at("subevaluator_1");
    ASSERT_EQ(octile_settings.m_name, GridPathfindingOctileHeuristic::CLASS_NAME);
    ASSERT_EQ(octile_settings.m_main_settings.size(), 2);
    ASSERT_EQ(octile_settings.m_main_settings.at("goal_state"), "(120, 120)");
    ASSERT_EQ(octile_settings.m_main_settings.at("diagonal_cost"), "1.414214");
    ASSERT_EQ(octile_settings.m_sub_component_settings.size(), 0);
}