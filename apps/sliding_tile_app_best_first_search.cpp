#include "building_tools/goal_tests/single_state_goal_test.h"
#include "engines/engine_components/eval_functions/f_cost_evaluator.h"
#include "engines/iterative_deepening/id_engine.h"
#include "engines/iterative_deepening/id_engine_params.h"
#include "environments/sliding_tile_puzzle/sliding_tile_action.h"
#include "environments/sliding_tile_puzzle/sliding_tile_manhattan_heuristic.h"
#include "environments/sliding_tile_puzzle/sliding_tile_state.h"
#include "environments/sliding_tile_puzzle/sliding_tile_transitions.h"
#include "environments/sliding_tile_puzzle/sliding_tile_utils.h"
#include "experiment_running/experiment_results.h"
#include "experiment_running/experiment_runner.h"
#include "experiment_running/search_resource_limits.h"
#include "utils/io_utils.h"
#include "utils/string_utils.h"

#include "engines/best_first_search/best_first_search.h"
#include "engines/best_first_search/best_first_search_params.h"
#include "environments/sliding_tile_puzzle/sliding_tile_hash_function.h"
#include "engines/engine_components/eval_functions/weighted_f_cost_evaluator.h"
#include "engines/engine_components/eval_functions/g_cost_evaluator.h"

#include <string>
#include <vector>
#include <iostream>
#include <fstream>

int main() {
    int num_rows = 3;
    int num_cols = 4;

    std::string problems_file = HSEF_DIR "/apps/input/3x4_puzzle.probs";  // HSEF_DIR is the root directory of the HSEF code
    std::vector<SlidingTileState> start_states = readSlidingTileStatesFromFile(problems_file, num_rows, num_cols);

    // Set up problem settings
    SlidingTileCostType cost_type = SlidingTileCostType::inverse;
    SlidingTileState goal_state(num_rows, num_cols);
    SingleStateGoalTest<SlidingTileState> goal_test(goal_state);
    SlidingTileTransitions transitions = SlidingTileTransitions(num_rows, num_cols, cost_type);

    // Sets up engine
    // IDEngineParams params;
    // IDEngine<SlidingTileState, BlankSlide> engine = IDEngine<SlidingTileState, BlankSlide>(params);
    
    // Sets up engine
    BestFirstSearchParams params;
    params.m_use_reopened = false;
    BestFirstSearch<SlidingTileState, BlankSlide, uint64_t> engine(params);

    SlidingTileHashFunction hash_function;
    engine.setHashFunction(hash_function);

    SlidingTileManhattanHeuristic heuristic = SlidingTileManhattanHeuristic(goal_state, cost_type);
    FCostEvaluator<SlidingTileState, BlankSlide> f_cost_evaluator(heuristic);
    // WeightedFCostEvaluator<SlidingTileState, BlankSlide> weighted_f_cost_evaluator(heuristic, 10.0);
    GCostEvaluator<SlidingTileState, BlankSlide> g_cost_evaluator;

    // Compute Heuristic for Initial Node
    // std::vector<double> result;
    // for (std::size_t i = 0; i < start_states.size(); i++) {
    //     // Create node container and get start state
    //     NodeList<SlidingTileState, BlankSlide> nodes;
    //     SlidingTileManhattanHeuristic heur = SlidingTileManhattanHeuristic(goal_state, cost_type);
    //     SlidingTileState start = start_states[i];
    //     NodeID start_id = nodes.addNode(start);

    //     // Prepare and execute the evaluator
    //     heur.setNodeContainer(nodes);
    //     heur.prepareToEvaluate();
    //     heur.evaluate(start_id);
    //     // cout << "Last computed evaluation: " << heur.getLastNodeEval() << "\n";
    //     result.emplace_back(heur.getLastNodeEval());
    // }

    // // Write the heuristic values to a CSV file
    // std::ofstream outputFile("output_inverse.csv");

    // // Write the column header
    // outputFile << "heuristic\n";

    // // Write each value from the vector on a new line
    // for (const auto& value : result) {
    //     outputFile << value << "\n";  // Write the value followed by a newline
    // }

    // // Close the file when done
    // outputFile.close();

    EvalsAndUsageVec<SlidingTileState, BlankSlide> evals;
    evals.emplace_back(f_cost_evaluator, true);
    evals.emplace_back(g_cost_evaluator, false);

    engine.setEvaluators(evals);

    SearchResourceLimits limits;  // no limits

    // runs experiments
    std::vector<ExperimentResults<BlankSlide>> multiple_output = runExperiments(engine, transitions, goal_test, limits, start_states, true);

    // store in csv
    std::string results_as_csv = getResultsVectorAsCSV(multiple_output);
    writeStringToFile(results_as_csv, "a2_part2_inv.csv");

    return 0;
}