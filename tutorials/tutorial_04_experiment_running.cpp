#include "building_tools/goal_tests/single_state_goal_test.h"
#include "engines/best_first_search/best_first_search.h"
#include "engines/best_first_search/best_first_search_params.h"
#include "engines/engine_components/eval_functions/f_cost_evaluator.h"
#include "environments/sliding_tile_puzzle/sliding_tile_action.h"
#include "environments/sliding_tile_puzzle/sliding_tile_hash_function.h"
#include "environments/sliding_tile_puzzle/sliding_tile_manhattan_heuristic.h"
#include "environments/sliding_tile_puzzle/sliding_tile_state.h"
#include "environments/sliding_tile_puzzle/sliding_tile_transitions.h"
#include "experiment_running/experiment_results.h"
#include "experiment_running/experiment_runner.h"
#include "experiment_running/search_resource_limits.h"
#include "logging/experiment_results_writer.h"
#include "utils/io_utils.h"
#include "utils/string_utils.h"

#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

using std::cout;

int main() {
    // READ THROUGH BOTH THE COMMENTS AND CODE BELOW

    cout << "\n------ TUTORIAL OBJECTIVES ------\n";
    cout << "In this tutorial, you will learn how to run and output the results of a set of experiments.\n";

    cout << "\n\n------ PART 1: Setting up a Set of Experiments ------\n";
    // It is common to need to run experiments with a single search engine on a set of problems
    // Here, we will consider the case where all the problems have the same goal state, but a different start state
    // Other cases are similar
    // We begin by defining the transition system and the start states

    SlidingTileTransitions transitions(3, 3, SlidingTileCostType::unit);

    SlidingTileState start_1({5, 7, 2, 1, 0, 3, 8, 4, 6}, 3, 3);
    SlidingTileState start_2({0, 1, 3, 2, 5, 4, 6, 7, 8}, 3, 3);
    SlidingTileState start_3({8, 7, 6, 5, 4, 3, 2, 1, 0}, 3, 3);

    std::vector<SlidingTileState> start_states;
    start_states.push_back(start_1);
    start_states.push_back(start_2);
    start_states.push_back(start_3);

    // Since all problems have the same goal state, we will also need a goal test
    SlidingTileState goal_state({0, 1, 2, 3, 4, 5, 6, 7, 8}, 3, 3);
    SingleStateGoalTest<SlidingTileState> goal_test(goal_state);

    // Next, we will initialize our engine, but we only need to initialize the engine-specific parts
    // The goal test, transitions, and resource limits will all be done for us
    BestFirstSearchParams params;
    BestFirstSearch<SlidingTileState, BlankSlide, uint64_t> engine(params);

    SlidingTileHashFunction hash_function;
    engine.setHashFunction(hash_function);

    SlidingTileManhattanHeuristic manhattan(goal_state, SlidingTileCostType::unit);
    FCostEvaluator<SlidingTileState, BlankSlide> f_cost(manhattan);
    engine.setEvaluator(f_cost);

    // We will also use the following resource limits, but again do not need to actually set the engine
    // The runner will do the setting for us
    SearchResourceLimits limits;  // In this case, we will leave the default value, meaning there are no limits

    // Notice that the engine is not ready to run
    cout << "is engine ready to run: " << boolToString(engine.canRunSearch()) << "\n";

    cout << "\n\n------ PART 2: Running a Set of Experiments With The Same Goal ------\n";
    // We now run the whole batch of experiments with the following call
    // Note that the last parameter is true, meaning that information will be output to standard out after each
    // problem is solved.
    // This output is actually a CSV file, which includes all stats about the output
    std::vector<ExperimentResults<BlankSlide>> results = runExperiments(engine, transitions, goal_test, limits, start_states, true);

    // The output of this experiments is a vector of ExperimentResults objects
    // Each holds information about the run, most of which is in the CSV output
    // However, having this vector means one can interact with the result, or output the results elsewhere
    // For example, the results can be output to the file tutorial_04_output.txt as follows
    std::string results_as_csv = getResultsVectorAsCSV(results);
    writeStringToFile(results_as_csv, "./tutorial_04_output.csv");

    // Now it can easily be loaded into any software that can read CSV information

    cout << "\n\n------ PART 2: Running a Set of Experiments With The Different Goals ------\n";
    // It is also straightforward to run a set of experiments, where each problem consists of a start and goal pair
    // In this case, we just have to call runExperiments with a vector of goal states instead of a goal test
    SlidingTileState goal_1({5, 7, 2, 0, 1, 3, 8, 4, 6}, 3, 3);
    SlidingTileState goal_2({0, 1, 2, 3, 4, 5, 6, 7, 8}, 3, 3);
    SlidingTileState goal_3({8, 7, 6, 5, 4, 3, 2, 0, 1}, 3, 3);

    std::vector<SlidingTileState> goal_states;
    goal_states.push_back(goal_1);
    goal_states.push_back(goal_2);
    goal_states.push_back(goal_3);

    // Now we can run the experiments for each pair (start_1, goal_1), (start_2, goal_2), (start_3, goal_3)
    results = runExperiments(engine, transitions, limits, start_states, goal_states, true);

    // It is important to note that this way of running experiments should only be used with heuristics
    // that are specific to a single-goal, and which have therefore been set to inherit from SingleGoalStateEvaluator
    // When doing so, runExperiments will automatically update any heuristic function to be specific to the given
    // goal state, instead of the original one set when the heuristic was initialized.

    //
    //
    cout << "\n\n------ SUMMARY ------\n";
    cout << " - Utilities have been provided to easily run a set of experiments, not just a single one.\n";
    cout << " - It can run a set of experiments where the start states share a goal, or where they have different goals.\n";
    cout << " - Information can then easily be logged in CSV files.\n";

    return 0;
}
