#include <gtest/gtest.h>

#include "engines/engine_components/eval_functions/f_cost_evaluator.h"
#include "engines/iterative_deepening/id_engine.h"
#include "engines/iterative_deepening/id_engine_params.h"
#include "environments/sliding_tile_puzzle/sliding_tile_action.h"
#include "environments/sliding_tile_puzzle/sliding_tile_manhattan_heuristic.h"
#include "environments/sliding_tile_puzzle/sliding_tile_state.h"
#include "environments/sliding_tile_puzzle/sliding_tile_transitions.h"
#include "experiment_running/experiment_results.h"
#include "experiment_running/experiment_runner.h"
#include "logging/experiment_results_writer.h"
#include "logging/logging_terms.h"
#include "utils/string_utils.h"

#include <optional>
#include <sstream>
#include <string>
#include <vector>

using std::string;
using std::vector;

class OutputWriterTests : public ::testing::Test {
protected:
    void SetUp() override {
        limits.m_get_actions_call_limit = 10;
        engine.setEvaluator(f_cost_evaluator);
    }

public:
    IDEngineParams engine_params;
    IDEngine<SlidingTileState, BlankSlide> engine = IDEngine<SlidingTileState, BlankSlide>(engine_params);

    SlidingTileState init_state1 = SlidingTileState({1, 4, 2, 3, 0, 5}, 2, 3);
    SlidingTileState init_state2 = SlidingTileState({1, 2, 3, 5, 4, 0}, 2, 3);
    SlidingTileState goal_state1 = SlidingTileState({0, 1, 2, 3, 4, 5}, 2, 3);
    SlidingTileState goal_state2 = SlidingTileState({1, 2, 5, 3, 4, 0}, 2, 3);

    SlidingTileTransitions trans_func = SlidingTileTransitions(2, 3, SlidingTileCostType::unit);
    SlidingTileManhattanHeuristic heuristic = SlidingTileManhattanHeuristic(goal_state1, SlidingTileCostType::unit);
    FCostEvaluator<SlidingTileState, BlankSlide> f_cost_evaluator = FCostEvaluator<SlidingTileState, BlankSlide>(heuristic);
    std::vector<NodeEvaluator<SlidingTileState, BlankSlide>*> evaluators = {&heuristic, &f_cost_evaluator};

    SearchResourceLimits limits;

    /**
    * Constructs the expected string for the ymal string tests
    * 
    * @param add_engine_name If the engine name was added
    * @param add_plan If the plan was added
    * @param add_stats If the stats were added
    * @param add_params If the engine parameters were added
    */
    string constructYAMLTestString(ExperimentResults<BlankSlide> result, bool add_engine_name, bool add_plan, bool add_stats, bool add_params) {
        StringMap stats_log = result.m_standard_stats.getStatsLog();
        std::stringstream output_string;

        string transition_sys_str = "transition_system: \n"
                                    "\tname: SlidingTileTransitions\n"
                                    "\tsettings: \n"
                                    "\t\t- num_rows: 2\n"
                                    "\t\t- num_cols: 3\n"
                                    "\t\t- cost_type: unit\n";

        string goal_test_str = "goal_test: \n"
                               "\tname: SingleStateGoalTest\n"
                               "\tsettings: \n"
                               "\t\t- goal_state: (2x3)-[0 1 2, 3 4 5]\n";

        string engine_settings = "\tsettings: \n"
                                 "\t\t- use_stored_seed: false\n"
                                 "\t\t- use_random_op_ordering: false\n"
                                 "\t\t- use_parent_pruning: true\n"
                                 "\t\t- random_seed: " +
                                 result.m_engine_settings.m_main_settings.at("random_seed") +
                                 "\n"
                                 "\tcomponents: \n"
                                 "\t\t- eval_function: \n"
                                 "\t\t\tname: FCost\n"
                                 "\t\t\tcomponents: \n"
                                 "\t\t\t\t- heuristic: \n"
                                 "\t\t\t\t\tname: SlidingTileManhattanHeuristic\n"
                                 "\t\t\t\t\tsettings: \n"
                                 "\t\t\t\t\t\t- goal_state: (2x3)-[0 1 2, 3 4 5]\n"
                                 "\t\t\t\t\t\t- cost_type: unit\n";

        string def = "\tplan_found: true\n\tplan_cost: 2.0\n";
        string plan = "\tplan: ['up','left']\n";
        string stats = "\tstats: \n"
                       "\t\t- num_actions_generated: 6\n"
                       "\t\t- num_evals: 4\n"
                       "\t\t- num_get_actions_calls: 2\n"
                       "\t\t- num_goal_tests: 3\n"
                       "\t\t- num_states_generated: 3\n"
                       "\t\t- search_time_seconds: " +
                       stats_log.at("search_time_seconds") + "\n" +
                       "\t\t- num_iterations: 1\n"
                       "\t\t- num_search_steps: 4\n";

        output_string << transition_sys_str << goal_test_str;

        if (add_engine_name || add_params) {
            output_string << "search_engine: \n";
            if (add_engine_name) {
                output_string << "\tname: IDEngine\n";
            }
            if (add_params) {
                output_string << engine_settings;
            }
        }

        output_string << "results: \n";
        output_string << def;
        if (add_plan) {
            output_string << plan;
        }
        if (add_stats) {
            output_string << stats;
        }

        return output_string.str();
    }
};

/**
 * Tests that turning a result into a CSV works as expected.
 */
TEST_F(OutputWriterTests, getResultAsCSVTest) {
    engine.setResourceLimits(limits);
    engine.setTransitionSystem(trans_func);
    ExperimentResults<BlankSlide> result = runExperiment(engine, init_state1, goal_state1, evaluators);

    string expected = "true;2.0;2;6;4;2;3;3;" + roundAndToString(result.m_standard_stats.m_search_time_seconds, 4) + ";1;4";
    ASSERT_EQ(getResultAsCSV(result, std::nullopt, ";"), expected);

    // Try default delimiter and experiment name
    string expected2 = "exp01,true,2.0,2,6,4,2,3,3," + roundAndToString(result.m_standard_stats.m_search_time_seconds, 4) + ",1,4";
    ASSERT_EQ(getResultAsCSV(result, "exp01"), expected2);
}

/**
 * Tests that the header output is correct
 */
TEST_F(OutputWriterTests, getHeaderTest) {
    vector<SlidingTileState> init_states{init_state1, init_state2};
    vector<SlidingTileState> goal_states{goal_state1, goal_state2};

    vector<ExperimentResults<BlankSlide>> results = runExperiments(engine, trans_func, limits, init_states, goal_states);

    string expected = "run_id,plan_found,plan_cost,plan_length,num_actions_generated,num_evals,num_get_actions_calls,num_goal_tests,num_states_generated,search_time_seconds,num_iterations,num_search_steps";

    ASSERT_EQ(getCSVHeader(results[0]), expected);
}

/*
 * Tests that turning a vector of results into a CSV works as expected.
 */
TEST_F(OutputWriterTests, getResultsVectorAsCSVTest) {
    vector<SlidingTileState> init_states{init_state1, init_state2};
    vector<SlidingTileState> goal_states{goal_state1, goal_state2};

    vector<ExperimentResults<BlankSlide>> results = runExperiments(engine, trans_func, limits, init_states, goal_states);

    string header = "run_id,plan_found,plan_cost,plan_length,num_actions_generated,num_evals,num_get_actions_calls,num_goal_tests,num_states_generated,search_time_seconds,num_iterations,num_search_steps";

    string output = getResultsVectorAsCSV(results);
    vector<string> lines = split(output, '\n');
    ASSERT_EQ(lines.size(), 3);
    ASSERT_EQ(lines[0], header);
    ASSERT_EQ(lines[1], "1," + getResultAsCSV(results[0]));
    ASSERT_EQ(lines[2], "2," + getResultAsCSV(results[1]));
}

/**
* Tests that the yaml string for a single result works as expected.
*/
TEST_F(OutputWriterTests, yamlStringTest) {
    engine.setTransitionSystem(trans_func);
    ExperimentResults<BlankSlide> result = runExperiment(engine, init_state1, goal_state1, evaluators);

    string output_string = getResultAsYAML(result);
    ASSERT_EQ(output_string, constructYAMLTestString(result, true, true, true, true));
}