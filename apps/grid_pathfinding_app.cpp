#include "engines/best_first_search/best_first_search.h"
#include "engines/best_first_search/best_first_search_params.h"
#include "engines/engine_components/eval_functions/f_cost_evaluator.h"
#include "engines/engine_components/eval_functions/g_cost_evaluator.h"
#include "engines/engine_components/open_lists/evaluator_and_comparing_usage.h"
#include "environments/grid_pathfinding/grid_location.h"
#include "environments/grid_pathfinding/grid_location_hash_function.h"
#include "environments/grid_pathfinding/grid_map.h"
#include "environments/grid_pathfinding/grid_pathfinding_action.h"
#include "environments/grid_pathfinding/grid_pathfinding_manhattan_heuristic.h"
#include "environments/grid_pathfinding/grid_pathfinding_transitions.h"
#include "experiment_running/experiment_results.h"
#include "experiment_running/experiment_runner.h"
#include "experiment_running/search_resource_limits.h"

#include <stdint.h>
#include <string>
#include <vector>

int main() {
    // In this app, we run A* on custom problems on a custom map
    // The A* instance uses the Manhattan distance heuristic with Hi-G tie-breaking

    // The following is the file name of the custom map
    // It is in hsef/apps , which is accessed using the HSEF_DIR macro
    std::string map_file = HSEF_DIR "/apps/input/simple.map";
    GridMap grid_4x4(map_file);  // loads in the map file

    // Set up problem settings
    GridLocation start_state_1(0, 0);
    GridLocation goal_state_1(3, 1);

    GridLocation start_state_2(1, 1);
    GridLocation goal_state_2(2, 3);

    std::vector<GridLocation> start_states{start_state_1, start_state_2};
    std::vector<GridLocation> goal_states{goal_state_1, goal_state_2};

    GridPathfindingTransitions trans_func = GridPathfindingTransitions(&grid_4x4);

    // Sets up engine
    BestFirstSearchParams params;
    BestFirstSearch<GridLocation, GridDirection, uint32_t> engine(params);
    GridLocationHashFunction hash_func;
    engine.setHashFunction(hash_func);

    // Sets up the evaluators
    GridPathfindingManhattanHeuristic heuristic;
    FCostEvaluator<GridLocation, GridDirection> f_cost_evaluator(heuristic);
    GCostEvaluator<GridLocation, GridDirection> g_cost_evaluator;

    EvalsAndUsageVec<GridLocation, GridDirection> evals;  // this vector holds the evaluators used, in order
    evals.emplace_back(f_cost_evaluator, true);  // the main evaluator is f-cost. The true makes in prioritize by minimum f-cost
    evals.emplace_back(g_cost_evaluator, false);  // Ties are broken by g-cost. The false means priority is given to high g-cost
    engine.setEvaluators(evals);

    SearchResourceLimits limits;  // no limits

    // runs experiments
    std::vector<ExperimentResults<GridDirection>> multiple_output = runExperiments(engine, trans_func, limits, start_states, goal_states, true);
    return 0;
}