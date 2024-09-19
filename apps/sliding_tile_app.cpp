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

#include <string>
#include <vector>


int main() {
    int num_rows = 3;
    int num_cols = 4;

    std::string problems_file = HSEF_DIR "/apps/input/3x4_puzzle.probs";  // HSEF_DIR is the root directory of the HSEF code
    std::vector<SlidingTileState> start_states = readSlidingTileStatesFromFile(problems_file, num_rows, num_cols);

    // Set up problem settings
    SlidingTileCostType cost_type = SlidingTileCostType::unit;
    SlidingTileState goal_state(num_rows, num_cols);
    SingleStateGoalTest<SlidingTileState> goal_test(goal_state);
    SlidingTileTransitions transitions = SlidingTileTransitions(num_rows, num_cols, cost_type);

    // Sets up engine
    IDEngineParams params;
    IDEngine<SlidingTileState, BlankSlide> engine = IDEngine<SlidingTileState, BlankSlide>(params);

    SlidingTileManhattanHeuristic heuristic = SlidingTileManhattanHeuristic(goal_state, cost_type);
    FCostEvaluator<SlidingTileState, BlankSlide> f_cost_evaluator(heuristic);
    engine.setEvaluator(f_cost_evaluator);

    SearchResourceLimits limits;  // no limits

    // runs experiments
    std::vector<ExperimentResults<BlankSlide>> multiple_output = runExperiments(engine, transitions, goal_test, limits, start_states, true);

    return 0;
}
