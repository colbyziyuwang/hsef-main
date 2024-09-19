#include "engines/best_first_search/best_first_search.h"
#include "engines/best_first_search/best_first_search_params.h"
#include "engines/engine_components/eval_functions/f_cost_evaluator.h"
#include "environments/grid_pathfinding/grid_location_hash_function.h"
#include "environments/grid_pathfinding/grid_pathfinding_action.h"
#include "environments/grid_pathfinding/grid_pathfinding_octile_heuristic.h"
#include "environments/grid_pathfinding/grid_pathfinding_scenario_running.h"
#include "experiment_running/search_resource_limits.h"
#include "utils/floating_point_utils.h"
#include "utils/string_utils.h"

#include <cstdint>
#include <gtest/gtest.h>
#include <vector>

#define TEST_DIRECTORY_ HSEF_DIR "/tests/environments/grid_pathfinding/scenario_loader_tests/"

/**
* Tests that a senario can be loaded with senario loader and will return the correct data 
*/
TEST(SenarioLoaderTests, canLoadExperimentTest) {
    std::vector<GridPathfindingScenario> data = loadScenarioFile(TEST_DIRECTORY_ "scenarios/arena.scen", TEST_DIRECTORY_);

    ASSERT_EQ(data.size(), 5);

    ASSERT_EQ(data[0].m_map_path, TEST_DIRECTORY_ "maps/square.map");
    ASSERT_EQ(data[0].m_start_state, GridLocation(3, 1));
    ASSERT_EQ(data[0].m_goal_state, GridLocation(3, 2));
    ASSERT_TRUE(fpEqual(data[0].m_octile_optimal_cost, 1));

    ASSERT_EQ(data[1].m_map_path, TEST_DIRECTORY_ "maps/random.map");
    ASSERT_EQ(data[1].m_start_state, GridLocation(4, 8));
    ASSERT_EQ(data[1].m_goal_state, GridLocation(4, 1));
    ASSERT_TRUE(fpEqual(data[1].m_octile_optimal_cost, 7));

    ASSERT_EQ(data[2].m_map_path, TEST_DIRECTORY_ "maps/square.map");
    ASSERT_EQ(data[2].m_start_state, GridLocation(1, 1));
    ASSERT_EQ(data[2].m_goal_state, GridLocation(4, 4));
    ASSERT_TRUE(fpEqual(data[2].m_octile_optimal_cost, 4.24264));

    ASSERT_EQ(data[3].m_map_path, TEST_DIRECTORY_ "maps/square.map");
    ASSERT_EQ(data[3].m_start_state, GridLocation(2, 2));
    ASSERT_EQ(data[3].m_goal_state, GridLocation(2, 2));
    ASSERT_TRUE(fpEqual(data[3].m_octile_optimal_cost, 0));

    ASSERT_EQ(data[4].m_map_path, TEST_DIRECTORY_ "maps/random.map");
    ASSERT_EQ(data[4].m_start_state, GridLocation(1, 1));
    ASSERT_EQ(data[4].m_goal_state, GridLocation(6, 8));
    ASSERT_TRUE(fpEqual(data[4].m_octile_optimal_cost, 14.24264));
}

/**
* Tests behaviour of the Scenario runner and checks for correct outputs
*/
TEST(SenarioRunnerTests, runExperimentTests) {
    BestFirstSearchParams params;
    BestFirstSearch<GridLocation, GridDirection, uint32_t> engine = BestFirstSearch<GridLocation, GridDirection, uint32_t>(params);

    GridLocationHashFunction hash_function;
    SearchResourceLimits limits;
    GridPathfindingOctileHeuristic octile;
    FCostEvaluator<GridLocation, GridDirection> f_cost_evaluator(octile);
    limits.m_time_limit_seconds = 10;

    engine.setHashFunction(hash_function);
    engine.setEvaluator(f_cost_evaluator);

    std::vector<ExperimentResults<GridDirection>> results = runScenarioExperiments(engine, limits, loadScenarioFile(TEST_DIRECTORY_ "scenarios/arena.scen", TEST_DIRECTORY_), true);

    ASSERT_EQ(results.size(), 5);

    ASSERT_TRUE(results[0].m_has_found_plan);
    ASSERT_EQ(vectorToString(results[0].m_plan), "[south]");
    ASSERT_TRUE(fpEqual(results[0].m_plan_cost, 1));

    ASSERT_TRUE(results[1].m_has_found_plan);
    ASSERT_EQ(vectorToString(results[1].m_plan), "[north north north north north north north]");
    ASSERT_TRUE(fpEqual(results[1].m_plan_cost, 7));

    ASSERT_TRUE(results[2].m_has_found_plan);
    ASSERT_EQ(vectorToString(results[2].m_plan), "[southeast southeast southeast]");
    ASSERT_TRUE(fpEqual(results[2].m_plan_cost, 4.24264));

    ASSERT_TRUE(results[3].m_has_found_plan);
    ASSERT_EQ(vectorToString(results[3].m_plan), "[]");
    ASSERT_TRUE(fpEqual(results[3].m_plan_cost, 0));

    ASSERT_TRUE(results[4].m_has_found_plan);
    ASSERT_EQ(vectorToString(results[4].m_plan), "[east south southeast southeast east east east southeast south south south west west]");
    ASSERT_TRUE(fpEqual(results[4].m_plan_cost, 14.24264));
}