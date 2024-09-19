#include "building_tools/goal_tests/single_state_goal_test.h"
#include "engines/best_first_search/best_first_search.h"
#include "engines/best_first_search/best_first_search_params.h"
#include "engines/engine_components/eval_functions/f_cost_evaluator.h"
#include "environments/sliding_tile_puzzle/sliding_tile_action.h"
#include "environments/sliding_tile_puzzle/sliding_tile_hash_function.h"
#include "environments/sliding_tile_puzzle/sliding_tile_manhattan_heuristic.h"
#include "environments/sliding_tile_puzzle/sliding_tile_state.h"
#include "environments/sliding_tile_puzzle/sliding_tile_transitions.h"
#include "experiment_running/search_resource_limits.h"
#include "logging/logging_terms.h"
#include "logging/standard_search_statistics.h"
#include "search_basics/search_engine.h"
#include "utils/string_utils.h"

#include <cstdint>
#include <iostream>

using std::cout;

int main() {
    // READ THROUGH BOTH THE COMMENTS AND CODE BELOW

    cout << "\n------ TUTORIAL OBJECTIVES ------\n";
    cout << "In this tutorial, you will walk through the use of a SearchEngine to find a plan.\n";
    cout << "This includes defining a problem, initializing a search engine, running the engine, and viewing the result.\n";

    cout << "\n\n------ PART 1: Defining a Search Problem and Goal Testing ------\n";
    // In order to use a search engine to solve a problem, we first have to define all the components
    // needed to define the problem first.
    // There are three main parts to a search problem:
    //      1. The start state
    //      2. The transition system
    //      3. The goal test

    // We begin by defining the start state and transition system as done in Tutorial 01
    SlidingTileState start_state({5, 7, 2, 1, 0, 3, 8, 4, 6}, 3, 3);
    SlidingTileTransitions transitions(3, 3, SlidingTileCostType::unit);

    // HSEF allows for an arbitrary goal test function to be defined and used. The abstract class defining the
    // interface for a goal test is given in "core/search_basics/goal_test.h"
    // The most common goal test simply stores a goal and checks if a given state is a goal
    // We can construct it as follows
    SlidingTileState goal_state({0, 1, 2, 3, 4, 5, 6, 7, 8}, 3, 3);
    SlidingTileState goal_state_2({0, 1, 2, 3, 4, 5, 6, 7, 8}, 3, 3);
    SingleStateGoalTest<SlidingTileState> goal_test(goal_state);

    // We can now check if a given state is a goal sate using this object
    cout << "is the start state a goal: " << boolToString(goal_test.isGoal(start_state)) << "\n";
    cout << "is goal_state_2 a goal: " << boolToString(goal_test.isGoal(goal_state_2)) << "\n";

    //
    //
    cout << "\n\n------ PART 2: Initializing a Search Algorithm ------\n";
    // We will now create an instance of A* and use it find a plan from the start state to the goal state
    // Recall that A* is a best-first search that uses the f-cost evaluation function
    //
    // To set different parameters for this method, we start by creating a BestFirstSearchParams object
    // This will allow us to set if the algorithm should reopen nodes, as well as other options
    BestFirstSearchParams params;
    params.m_use_reopened = true;  // sets the search to reopen nodes

    // Now we will create a best-first search engine.
    // The template parameters correspond to the state type, the action type, and type used for the hash function
    // Note that a hash function is needed for identifying if states have already been encountered
    BestFirstSearch<SlidingTileState, BlankSlide, uint64_t> engine(params);

    // At this point, the engine is not "ready" to be used, as it needs information about the problem, and additional
    // components to be set. We can see this as follows
    cout << "Can the engine be used: " << boolToString(engine.canRunSearch()) << "\n";

    // All engines need to be given a transition system and a goal test
    engine.setTransitionSystem(transitions);
    engine.setGoalTest(goal_test);

    // All engines may optionally be given a set of resource limits (ie. time or expansion limits) after which the
    // algorithm will be terminated
    SearchResourceLimits limits;
    limits.m_goal_test_limit = 1000000;  // The search will be terminated after 1 million goal tests
    limits.m_time_limit_seconds = 20;  // The search will be terminated after 20 seconds

    engine.setResourceLimits(limits);

    // Now we need to set the engine specific parts
    // In the case of BestFirstSearch, we first need a hash function. An abstract class for state-hash function
    // can be found in "core/building_tools/hashing/state_hash_function.h". We will be using a sliding tile
    // specific hash function, which is actually just an alias of "core/building_tools/hashing/permutation_hash_function.h"
    SlidingTileHashFunction hash_function;
    engine.setHashFunction(hash_function);

    // Next, we need an evaluation function to guide the search.
    // Recall that these are evaluators
    // We will use an f-cost which uses the Manhattan distance
    SlidingTileManhattanHeuristic manhattan(goal_state, SlidingTileCostType::unit);
    FCostEvaluator<SlidingTileState, BlankSlide> f_cost(manhattan);

    // Technically, BestFirstSearch can take in a vector of evaluators to allow for a hierarchy of
    // evaluators (ie. order according to the first, then tiebreak according to the second, etc.)
    // We ignore this complication for now
    engine.setEvaluator(f_cost);

    // Finally, the engine is ready to run
    cout << "Can the engine be used after initialization: " << boolToString(engine.canRunSearch()) << "\n";

    //
    //
    cout << "\n\n------ PART 3: Finding a Plan ------\n";
    // Finding a plan for a given start state is now as easy as calling searchForPlan
    EngineStatus status = engine.searchForPlan(start_state);

    // Here, the status lets us know the outcome
    cout << "The engine status is: " << status << "\n";

    // search_completed means the search has succeeded with a plan
    cout << "plan found: " << vectorToString(engine.getLastSolutionPlan()) << "\n";
    cout << "plan cost: " << engine.getLastSolutionPlanCost() << "\n\n";

    // We can also get additional statistics about the search
    // This stores a variety of statistics about the search
    StandardSearchStatistics stats = engine.getStandardEngineStatistics();
    cout << "number of goal tests: " << stats.m_num_goal_tests << "\n";
    cout << "search time (seconds): " << stats.m_search_time_seconds << "\n";

    // The standard statistics contains other items as well
    // In addition, different search engines contain algorithm-specific stats
    // These are returned as a StringMap, which is a dictionary (ie. a map with string keys and string values)
    StringMap extra_stats = engine.getEngineSpecificStatistics();
    for (const auto& [key, value] : extra_stats) {
        cout << key << " : " << value << "\n";
    }

    // We also note the following:
    //      - You can also run SingleSearchStepEngines one step at a time if there is a need to "view" search information
    //          after each point of the search.
    //      - The above is not the suggested way you run most experiments. See tutorial 04 for information on using
    //          the HSEF experiment running capabilities. However, this approach is available

    //
    //
    cout << "\n\n------ SUMMARY ------\n";
    cout << " - SearchEngines provide a way to find a plan to solve a given problem.\n";
    cout << " - The first step is to create a goal test and the other problem components.\n";
    cout << " - Next, the search engine must be initialized with the problem and engine-specific components.\n";
    cout << " - Finally, you can run the engine to find a plan, and analyze the results.\n";

    return 0;
}
