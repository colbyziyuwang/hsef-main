#include "engines/best_first_search/best_first_search.h"
#include "engines/best_first_search/best_first_search_params.h"
#include "engines/engine_components/eval_functions/f_cost_evaluator.h"
#include "engines/engine_components/eval_functions/g_cost_evaluator.h"
#include "engines/engine_components/open_lists/evaluator_and_comparing_usage.h"
#include "environments/grid_pathfinding/grid_location.h"
#include "environments/grid_pathfinding/grid_location_hash_function.h"
#include "environments/grid_pathfinding/grid_pathfinding_action.h"
#include "environments/grid_pathfinding/grid_pathfinding_octile_heuristic.h"
#include "environments/grid_pathfinding/grid_pathfinding_scenario_running.h"
#include "experiment_running/experiment_results.h"
#include "experiment_running/search_resource_limits.h"
#include "logging/experiment_results_writer.h"
#include "utils/io_utils.h"
#include "utils/string_utils.h"
#include "engines/engine_components/node_containers/node_list.h"
#include "engines/engine_components/eval_functions/weighted_f_cost_evaluator.h"

#include <stdint.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>

using namespace std;

int main() {
    // For 2D pathfinding, there are well known benchmarks which can be found here: https://movingai.com/benchmarks/grids.html
    // These benchmarks are given as a map file and a scenario file, which contains a list of problems to test on
    // Note that the scenario file contains the name of the map to run on

    // This sets the file with the scenario
    std::string scenario_file = HSEF_DIR "/apps/input/arena2.map.scen";
    std::string map_dir = HSEF_DIR "/apps/input/";
    std::vector<GridPathfindingScenario> scenarios = loadScenarioFile(scenario_file, map_dir);

    // Sets up engine
    BestFirstSearchParams params;
    params.m_use_reopened = false;
    BestFirstSearch<GridLocation, GridDirection, uint32_t> engine(params);
    GridLocationHashFunction hash_func;
    engine.setHashFunction(hash_func);

    // Sets up the evaluators
    GridPathfindingOctileHeuristic heuristic;
    // FCostEvaluator<GridLocation, GridDirection> f_cost_evaluator(heuristic);
    WeightedFCostEvaluator<GridLocation, GridDirection> weighted_f_cost_evaluator(heuristic, 10.0);
    
    GCostEvaluator<GridLocation, GridDirection> g_cost_evaluator;

    // // Compute Heuristic for Initial Node
    // std::vector<double> result;
    // for (std::size_t i = 0; i < scenarios.size(); i++) {
    //     // Get the GridPathfindingScenario object
    //     GridPathfindingScenario scen = scenarios[i];

    //     // Create node container and get goal, and start state
    //     NodeList<GridLocation, GridDirection> nodes;
    //     GridLocation goal = scen.m_goal_state;
    //     GridPathfindingOctileHeuristic heur(goal);
    //     GridLocation start = scen.m_start_state;
    //     NodeID start_id = nodes.addNode(start);

    //     // Prepare and execute the evaluator
    //     heur.setNodeContainer(nodes);
    //     heur.prepareToEvaluate();
    //     heur.evaluate(start_id);
    //     // cout << "Last computed evaluation: " << heur.getLastNodeEval() << "\n";
    //     result.emplace_back(heur.getLastNodeEval());
    // }

    // // Write the heuristic values to a CSV file
    // std::ofstream outputFile("output.csv");

    // // Write the column header
    // outputFile << "heuristic\n";

    // // Write each value from the vector on a new line
    // for (const auto& value : result) {
    //     outputFile << value << "\n";  // Write the value followed by a newline
    // }

    // // Close the file when done
    // outputFile.close();

    EvalsAndUsageVec<GridLocation, GridDirection> evals;  // this vector holds the evaluators used, in order
    // evals.emplace_back(f_cost_evaluator, true);  // the main evaluator is f-cost. The true makes in prioritize by minimum f-cost
    evals.emplace_back(weighted_f_cost_evaluator, true);
    // evals.emplace_back(g_cost_evaluator, false);  // Ties are broken by g-cost. The false means priority is given to high g-cost
    // evals.emplace_back(g_cost_evaluator, true); // Priority given to low g-cost
    engine.setEvaluators(evals);

    SearchResourceLimits limits;  // no limits

    // runs experiments
    std::vector<ExperimentResults<GridDirection>> multiple_output = runScenarioExperiments(engine, limits, scenarios, true);
    
    // store in csv
    std::string results_as_csv = getResultsVectorAsCSV(multiple_output);
    writeStringToFile(results_as_csv, "./assignment2_part4_default.csv");

    return 0;
}