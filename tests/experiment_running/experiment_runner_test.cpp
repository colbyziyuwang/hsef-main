#include <cstddef>
#include <gtest/gtest.h>
#include <vector>

#include "building_tools/evaluators/constant_heuristic.h"
#include "building_tools/goal_tests/goal_tests_tools_terms.h"
#include "building_tools/goal_tests/single_state_goal_test.h"
#include "engines/engine_components/eval_functions/f_cost_evaluator.h"
#include "engines/iterative_deepening/id_engine.h"
#include "engines/iterative_deepening/id_engine_params.h"
#include "environments/sliding_tile_puzzle/sliding_tile_action.h"
#include "environments/sliding_tile_puzzle/sliding_tile_manhattan_heuristic.h"
#include "environments/sliding_tile_puzzle/sliding_tile_state.h"
#include "environments/sliding_tile_puzzle/sliding_tile_transitions.h"
#include "experiment_running/experiment_results.h"
#include "experiment_running/experiment_runner.h"
#include "search_basics/node_evaluator.h"

class ExperimentRunnerSlidingTileTests : public ::testing::Test {
protected:
    void SetUp() override {
        limits.m_get_actions_call_limit = 10;
    }

public:
    IDEngineParams params;
    IDEngine<SlidingTileState, BlankSlide> engine = IDEngine<SlidingTileState, BlankSlide>(params);
    IDEngine<SlidingTileState, BlankSlide> engine2 = IDEngine<SlidingTileState, BlankSlide>(params);

    SlidingTileState start1 = SlidingTileState({1, 4, 2, 3, 0, 5}, 2, 3);
    SlidingTileState start2 = SlidingTileState({1, 2, 3, 5, 4, 0}, 2, 3);

    SlidingTileState goal1 = SlidingTileState({0, 1, 2, 3, 4, 5}, 2, 3);
    SlidingTileState goal2 = SlidingTileState({1, 2, 5, 3, 4, 0}, 2, 3);

    SlidingTileTransitions transition_system = SlidingTileTransitions(2, 3, SlidingTileCostType::unit);

    ConstantHeuristic<SlidingTileState, BlankSlide> zero_heuristic;
    FCostEvaluator<SlidingTileState, BlankSlide> f_cost_zero = FCostEvaluator<SlidingTileState, BlankSlide>(zero_heuristic);

    SlidingTileManhattanHeuristic heuristic = SlidingTileManhattanHeuristic(goal1, SlidingTileCostType::unit);
    SingleStateGoalTest<SlidingTileState> test_for_goal1 = SingleStateGoalTest<SlidingTileState>(goal1);
    SingleStateGoalTest<SlidingTileState> test_for_goal2 = SingleStateGoalTest<SlidingTileState>(goal2);

    FCostEvaluator<SlidingTileState, BlankSlide> f_cost_manhattan = FCostEvaluator<SlidingTileState, BlankSlide>(heuristic);

    SearchResourceLimits limits;
};

/**
 * Tests the behaviour when running an experiment with a single start state
 */
TEST_F(ExperimentRunnerSlidingTileTests, singleStartStateTest) {
    engine.setEvaluator(f_cost_manhattan);
    engine.setTransitionSystem(transition_system);
    engine.setGoalTest(test_for_goal1);
    engine.setResourceLimits(limits);

    ExperimentResults<BlankSlide> output = runExperiment(engine, start1);

    // Checks that the experiment results are set correctly
    ASSERT_TRUE(output.m_has_found_plan);
    ASSERT_EQ(output.m_plan, engine.getLastSolutionPlan());
    ASSERT_EQ(output.m_plan_cost, engine.getLastSolutionPlanCost());

    // Checks that the settings have been set correctly
    for (const auto& [setting_name, setting_value] : output.m_engine_settings.m_main_settings) {
        ASSERT_EQ(setting_value, engine.getAllSettings().m_main_settings[setting_name]);
    }
}

/**
 * Tests the behaviour and output of experiment runner when a list of start states are provided.
 */
TEST_F(ExperimentRunnerSlidingTileTests, multipleStartStateAndSingleGoalTest) {
    std::vector<SlidingTileState> init_states = {start1, start2, goal1, goal2};

    engine.setEvaluator(f_cost_manhattan);

    std::vector<ExperimentResults<BlankSlide>> results = runExperiments(engine, transition_system, test_for_goal1, limits, init_states, true);

    engine2.setEvaluator(f_cost_manhattan);
    engine2.setTransitionSystem(transition_system);
    engine2.setGoalTest(test_for_goal1);
    engine2.setResourceLimits(limits);

    for (std::size_t i = 0; i < init_states.size(); i++) {
        engine2.searchForPlan(init_states[i]);

        // Checks that same is found in both cases
        ASSERT_EQ(results[i].m_has_found_plan, engine2.hasFoundSolution());
        ASSERT_EQ(results[i].m_plan, engine2.getLastSolutionPlan());
        ASSERT_EQ(results[i].m_plan_cost, engine2.getLastSolutionPlanCost());
        ASSERT_EQ(results[i].m_standard_stats.m_num_goal_tests, engine2.getStandardEngineStatistics().m_num_goal_tests);
    }
}

/**
 * Checks that experiment running works when given a single start and single goal state
 */
TEST_F(ExperimentRunnerSlidingTileTests, singleStartAndGoalTest) {
    // Set it to wrong goal state to make sure it changes correctly
    heuristic.setGoalState(goal2);

    engine.setEvaluator(f_cost_manhattan);
    engine.setTransitionSystem(transition_system);
    engine.setResourceLimits(limits);

    std::vector<NodeEvaluator<SlidingTileState, BlankSlide>*> evaluators = {&heuristic, &f_cost_manhattan};
    ExperimentResults<BlankSlide> output = runExperiment(engine, start1, goal1, evaluators);

    // Checks that the experiment results are set correctly
    ASSERT_TRUE(output.m_has_found_plan);
    ASSERT_EQ(output.m_plan, engine.getLastSolutionPlan());
    ASSERT_EQ(output.m_plan_cost, engine.getLastSolutionPlanCost());

    // Checks that the settings have been set correctly
    for (const auto& [setting_name, setting_value] : output.m_engine_settings.m_main_settings) {
        ASSERT_EQ(setting_value, engine.getAllSettings().m_main_settings[setting_name]);
    }
    ASSERT_EQ(output.m_goal_test_settings.m_main_settings[goalTestToolsTerms::SETTING_GOAL_STATE], "(2x3)-[0 1 2, 3 4 5]");
}

/**
 * Tests behaviour and output when a list of start and a list of goal states are provided.
 */
TEST_F(ExperimentRunnerSlidingTileTests, multipleStateAndGoalStateTest) {
    std::vector<SlidingTileState> starts{start1, start2};
    std::vector<SlidingTileState> goals{goal1, goal2};

    engine.setEvaluator(f_cost_manhattan);
    auto results = runExperiments(engine, transition_system, limits, starts, goals, true);

    // Checks that right number of results and correct goal was used for each
    ASSERT_EQ(results.size(), 2);
    ASSERT_EQ(results[0].m_goal_test_settings.m_main_settings[goalTestToolsTerms::SETTING_GOAL_STATE], "(2x3)-[0 1 2, 3 4 5]");
    ASSERT_EQ(results[1].m_goal_test_settings.m_main_settings[goalTestToolsTerms::SETTING_GOAL_STATE], "(2x3)-[1 2 5, 3 4 0]");

    // Runs a second identical engine manually and compares results
    // Run on first problem
    heuristic.setGoalState(goal1);
    engine2.setEvaluator(f_cost_manhattan);
    engine2.setTransitionSystem(transition_system);
    engine2.setGoalTest(test_for_goal1);
    engine2.setResourceLimits(limits);

    engine2.searchForPlan(start1);

    ASSERT_EQ(results[0].m_has_found_plan, engine2.hasFoundSolution());
    ASSERT_EQ(results[0].m_plan_cost, engine2.getLastSolutionPlanCost());
    ASSERT_EQ(results[0].m_standard_stats.m_num_goal_tests, engine2.getStandardEngineStatistics().m_num_goal_tests);

    // Runs on second problem
    heuristic.setGoalState(goal2);
    engine2.setGoalTest(test_for_goal1);
    engine2.searchForPlan(start2);

    ASSERT_EQ(results[1].m_has_found_plan, engine2.hasFoundSolution());
    ASSERT_EQ(results[1].m_plan_cost, engine2.getLastSolutionPlanCost());
    ASSERT_EQ(results[1].m_standard_stats.m_num_goal_tests, engine2.getStandardEngineStatistics().m_num_goal_tests);
}
