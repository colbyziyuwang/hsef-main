#include "building_tools/evaluators/hash_map_heuristic.h"
#include "building_tools/goal_tests/single_state_goal_test.h"
#include "building_tools/hashing/state_string_hash_function.h"
#include "engines/best_first_search/a_star_epsilon.h"
#include "engines/best_first_search/best_first_search.h"
#include "engines/engine_components/eval_functions/f_cost_evaluator.h"
#include "engines/iterative_deepening/id_engine.h"
#include "environments/graph/graph_transitions.h"
#include "environments/graph/graph_utils.h"
#include "logging/yaml_logging.h"

#include <gtest/gtest.h>

/**
* Tests that the yaml string for A Star Epsilon is constructed as intended
*/
TEST(SearchComponentSettingsTests, aStarEpsilonGetYAMLStringTest) {
    std::stringstream csv = std::stringstream("a;b 5;c 1\nb;d 1\nc;b 1\nd;goal 4");
    Graph graph = getGraphFromCSVAdjacencyList(csv);
    GraphTransitions transitions(graph);

    AStarEpsilonParams params;
    AStarEpsilon<GraphState, GraphAction, std::string> engine = AStarEpsilon<GraphState, GraphAction, std::string>(params);

    SingleStateGoalTest<GraphState> goal_test(transitions.getVertexState("goal"));

    StateStringHashFunction<GraphState> hasher;
    HashMapHeuristic<GraphState, GraphAction, std::string> heuristic(hasher);

    engine.setHeuristic(heuristic);
    engine.setTransitionSystem(transitions);
    engine.setGoalTest(goal_test);
    engine.setHashFunction(hasher);

    ASSERT_EQ(getYAMLString(engine.getAllSettings()),
              "name: AStarEpsilon\n"
              "settings: \n"
              "\t- weight: 1.0\n"
              "\t- use_stored_seed: false\n"
              "\t- use_reopened: true\n"
              "\t- store_expansion_order: false\n"
              "\t- random_seed: 0\n"
              "\t- parent_heuristic_updating: false\n"
              "components: \n"
              "\t- hash_function: \n"
              "\t\tname: StateStringHashFunction\n"
              "\t- heuristic: \n"
              "\t\tname: HashMapHeuristic\n"
              "\t\tsettings: \n"
              "\t\t\t- default_h_value: 0.0\n"
              "\t\tcomponents: \n"
              "\t\t\t- hash_function: \n"
              "\t\t\t\tname: StateStringHashFunction\n");
}

/**
* Tests that the yaml string for Best first seach is constructed as intended
*/
TEST(SearchComponentSettingsTests, bestFirstSearchGetYMALStringTest) {
    std::stringstream csv = std::stringstream("a;b 5;c 1\nb;d 1\nc;b 1\nd;goal 4");
    Graph graph = getGraphFromCSVAdjacencyList(csv);
    GraphTransitions transitions(graph);

    BestFirstSearchParams params;
    BestFirstSearch<GraphState, GraphAction, std::string> engine = BestFirstSearch<GraphState, GraphAction, std::string>(params);

    SingleStateGoalTest<GraphState> goal_test(transitions.getVertexState("goal"));

    StateStringHashFunction<GraphState> hasher;
    HashMapHeuristic<GraphState, GraphAction, std::string> heuristic(hasher);

    FCostEvaluator<GraphState, GraphAction> f_cost_evaluator(heuristic);
    engine.setEvaluator(f_cost_evaluator);
    engine.setTransitionSystem(transitions);
    engine.setGoalTest(goal_test);
    engine.setHashFunction(hasher);

    ASSERT_EQ(getYAMLString(engine.getAllSettings()),
              "name: BestFirstSearch\n"
              "settings: \n"
              "\t- use_stored_seed: false\n"
              "\t- use_reopened: true\n"
              "\t- store_expansion_order: false\n"
              "\t- random_seed: 0\n"
              "components: \n"
              "\t- eval_function: \n"
              "\t\tname: FCost\n"
              "\t\tcomponents: \n"
              "\t\t\t- heuristic: \n"
              "\t\t\t\tname: HashMapHeuristic\n"
              "\t\t\t\tsettings: \n"
              "\t\t\t\t\t- default_h_value: 0.0\n"
              "\t\t\t\tcomponents: \n"
              "\t\t\t\t\t- hash_function: \n"
              "\t\t\t\t\t\tname: StateStringHashFunction\n"
              "\t- hash_function: \n"
              "\t\tname: StateStringHashFunction\n");
}

/**
* Tests that the yaml string for the IDEngine is constructed as intended
*/
TEST(SearchComponentSettingsTests, IDEngineGetYMALStringTest) {
    std::stringstream csv = std::stringstream("a;b;c 2;d\nb;e;f\nc;g;h\ng;goal");
    Graph graph = getGraphFromCSVAdjacencyList(csv);
    GraphTransitions transitions(graph);

    IDEngineParams params;
    IDEngine<GraphState, GraphAction> engine = IDEngine<GraphState, GraphAction>(params);

    SingleStateGoalTest<GraphState> goal_test(transitions.getVertexState("goal"));
    engine.setGoalTest(goal_test);

    StateStringHashFunction<GraphState> hasher;
    HashMapHeuristic<GraphState, GraphAction, std::string> heuristic(hasher);
    FCostEvaluator<GraphState, GraphAction> f_cost_evaluator(heuristic);

    engine.setEvaluator(f_cost_evaluator);
    engine.setTransitionSystem(transitions);
    engine.setGoalTest(goal_test);

    ASSERT_EQ(getYAMLString(engine.getAllSettings()),
              "name: IDEngine\n"
              "settings: \n"
              "\t- use_stored_seed: false\n"
              "\t- use_random_op_ordering: false\n"
              "\t- use_parent_pruning: true\n"
              "\t- random_seed: 0\n"
              "components: \n"
              "\t- eval_function: \n"
              "\t\tname: FCost\n"
              "\t\tcomponents: \n"
              "\t\t\t- heuristic: \n"
              "\t\t\t\tname: HashMapHeuristic\n"
              "\t\t\t\tsettings: \n"
              "\t\t\t\t\t- default_h_value: 0.0\n"
              "\t\t\t\tcomponents: \n"
              "\t\t\t\t\t- hash_function: \n"
              "\t\t\t\t\t\tname: StateStringHashFunction\n");
}