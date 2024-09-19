#include <gtest/gtest.h>

#include "building_tools/evaluators/constant_heuristic.h"
#include "building_tools/evaluators/hash_map_heuristic.h"
#include "building_tools/goal_tests/single_state_goal_test.h"
#include "building_tools/hashing/state_string_hash_function.h"
#include "engines/best_first_search/best_first_search.h"
#include "engines/best_first_search/best_first_search_params.h"
#include "engines/engine_components/eval_functions/eval_function_terms.h"
#include "engines/engine_components/eval_functions/f_cost_evaluator.h"
#include "engines/engine_components/eval_functions/g_cost_evaluator.h"
#include "environments/graph/graph_transitions.h"
#include "environments/graph/graph_utils.h"
#include "environments/graph/vertex_hash_function.h"

/**
 * Creates a fixture for IDEngine tests. Just a simple complete tree to depth 2 and will use a zero heuristic.
 */
class BestFirstSearchSimpleGraphTests : public ::testing::Test {
protected:
    void SetUp() override {
        graph.sortOutEdgesByToVertexLabel();
    }

public:
    std::stringstream csv = std::stringstream("a;b;c;d\nb;b1;b2\nc;c1;c2;c3\nd;d1;d2;d3;d4");  ///< Defines simple 3 level tree
    Graph graph = getGraphFromCSVAdjacencyList(csv);  ///< Turns adjacency list into graph
    GraphTransitions transitions = GraphTransitions(graph);  ///< Turns graph into transitions

    BestFirstSearchParams params;  ///< Using basic parameters
    BestFirstSearch<GraphState, GraphAction, uint32_t> engine = BestFirstSearch<GraphState, GraphAction, uint32_t>(params);  ///< Here is the engine

    SingleStateGoalTest<GraphState> goal_test = SingleStateGoalTest<GraphState>(transitions.getVertexState("d3"));  ///<Sets goal as d3
    ConstantHeuristic<GraphState, GraphAction> zero_heuristic;  ///< Use the zero heuristic
    FCostEvaluator<GraphState, GraphAction> f_cost_evaluator = FCostEvaluator<GraphState, GraphAction>(zero_heuristic);
    VertexHashFunction vertex_hash;
    GraphState init_state = transitions.getVertexState("a");  ///< Sets the initial state as vertex a
};

TEST_F(BestFirstSearchSimpleGraphTests, constructorTest) {
    ASSERT_FALSE(engine.canRunSearch());
    ASSERT_EQ(engine.getStatus(), EngineStatus::not_ready);

    StandardSearchStatistics stats = engine.getStandardEngineStatistics();
    ASSERT_EQ(stats.m_num_goal_tests, 0);

    ASSERT_FALSE(engine.hasFoundSolution());
    ASSERT_TRUE(engine.getLastSolutionPlan().empty());
    ASSERT_EQ(engine.getLastSolutionPlanCost(), -1.0);
}

/**
 * Sets that the Engine can run properly once all necessary parts have been set.
 */
TEST_F(BestFirstSearchSimpleGraphTests, setAndCanRunTest) {
    engine.setEvaluator(f_cost_evaluator);
    ASSERT_FALSE(engine.canRunSearch());
    ASSERT_EQ(engine.getStatus(), EngineStatus::not_ready);

    engine.setTransitionSystem(transitions);
    ASSERT_FALSE(engine.canRunSearch());
    ASSERT_EQ(engine.getStatus(), EngineStatus::not_ready);

    engine.setGoalTest(goal_test);
    ASSERT_FALSE(engine.canRunSearch());
    ASSERT_EQ(engine.getStatus(), EngineStatus::not_ready);

    engine.setHashFunction(vertex_hash);
    ASSERT_TRUE(engine.canRunSearch());
    ASSERT_EQ(engine.getStatus(), EngineStatus::ready);
}

/**
 * Checks that initialize search sets things up correctly.
 */
TEST_F(BestFirstSearchSimpleGraphTests, initializeSearchTest) {
    engine.setEvaluator(f_cost_evaluator);
    engine.setTransitionSystem(transitions);
    engine.setGoalTest(goal_test);
    engine.setHashFunction(vertex_hash);

    engine.initializeSearch(init_state);
    ASSERT_EQ(engine.getStatus(), EngineStatus::active);
    ASSERT_EQ(engine.getOpenListSize(), 1);

    const auto& nodes = engine.getNodes();
    ASSERT_EQ(nodes.getState(0), init_state);
    ASSERT_EQ(nodes.getLastAction(0), std::nullopt);
    ASSERT_EQ(nodes.getGValue(0), 0.0);
    ASSERT_EQ(zero_heuristic.getCachedEval(0), 0.0);
    ASSERT_FALSE(f_cost_evaluator.getCachedIsDeadEnd(0));
    ASSERT_EQ(f_cost_evaluator.getCachedEval(0), 0.0);

    const std::vector<GraphAction>& current_action_list = engine.getCurrentActionList();
    ASSERT_EQ(current_action_list.size(), 0);
}

/**
 * Runs step by step through a search on the simple graph to make sure the right thing is expanded at every step.
 *
 * Order of expansion is a, b, d, c, c3, c2, c1, d4, d3
 */
TEST_F(BestFirstSearchSimpleGraphTests, singleSearchStepTest) {
    engine.setEvaluator(f_cost_evaluator);
    engine.setTransitionSystem(transitions);
    engine.setGoalTest(goal_test);
    engine.setHashFunction(vertex_hash);
    params.m_store_expansion_order = true;
    engine.setEngineParams(params);
    ASSERT_EQ(engine.getExpansionOrder().size(), 0);
    engine.initializeSearch(init_state);

    ASSERT_EQ(engine.getOpenList().getIDOfBestNode(), 0);  // Expanding "a" first

    EngineStatus res = engine.singleSearchStep();
    ASSERT_EQ(engine.getCurrentActionList().size(), 3);
    ASSERT_EQ(engine.getOpenListSize(), 3);
    ASSERT_EQ(engine.getClosedListSize(), 1);
    ASSERT_EQ(engine.getExpansionOrder().size(), 1);
    ASSERT_EQ(engine.getLastExpandedNodeID(), 0);
    ASSERT_EQ(res, EngineStatus::active);

    // Check that children have been added correctly
    ASSERT_EQ(engine.getNodes().getState(1), transitions.getVertexState("b"));
    ASSERT_EQ(engine.getNodes().getState(2), transitions.getVertexState("c"));
    ASSERT_EQ(engine.getNodes().getState(3), transitions.getVertexState("d"));

    // Extra checking for vertex b
    const auto& nodes = engine.getNodes();
    ASSERT_EQ(nodes.getState(1), transitions.getVertexState("b"));
    ASSERT_FALSE(f_cost_evaluator.getCachedIsDeadEnd(1));
    ASSERT_EQ(zero_heuristic.getCachedEval(1), 0.0);
    ASSERT_EQ(nodes.getGValue(1), 1.0);
    ASSERT_EQ(f_cost_evaluator.getCachedEval(1), 1.0);
    ASSERT_EQ(nodes.getLastAction(1), transitions.getEdgeAction("a", "b"));
    ASSERT_EQ(nodes.getParentID(1), 0);

    ASSERT_EQ(engine.getOpenList().getIDOfBestNode(), 1);  // Expanding "b" next
    res = engine.singleSearchStep();
    ASSERT_EQ(engine.getOpenListSize(), 4);
    ASSERT_EQ(engine.getClosedListSize(), 2);
    ASSERT_EQ(engine.getExpansionOrder().size(), 2);
    ASSERT_EQ(engine.getLastExpandedNodeID(), 1);
    ASSERT_EQ(res, EngineStatus::active);

    ASSERT_EQ(engine.getOpenList().getIDOfBestNode(), 3);  // Expanding "d" next because d gets pulled to fron when remove b
    res = engine.singleSearchStep();
    ASSERT_EQ(engine.getOpenListSize(), 7);
    ASSERT_EQ(engine.getClosedListSize(), 3);
    ASSERT_EQ(engine.getExpansionOrder().size(), 3);
    ASSERT_EQ(engine.getLastExpandedNodeID(), 3);
    ASSERT_EQ(res, EngineStatus::active);

    ASSERT_EQ(engine.getOpenList().getIDOfBestNode(), 2);  // Expanding "c" next
    res = engine.singleSearchStep();
    ASSERT_EQ(engine.getOpenListSize(), 9);
    ASSERT_EQ(engine.getClosedListSize(), 4);
    ASSERT_EQ(engine.getExpansionOrder().size(), 4);
    ASSERT_EQ(engine.getLastExpandedNodeID(), 2);
    ASSERT_EQ(res, EngineStatus::active);

    ASSERT_FALSE(engine.hasFoundSolution());
    while (res != EngineStatus::search_completed) {
        res = engine.singleSearchStep();
    }
    ASSERT_TRUE(engine.hasFoundSolution());
    ASSERT_EQ(engine.getStatus(), EngineStatus::search_completed);
    ASSERT_EQ(engine.getLastSolutionPlanCost(), 2);

    auto plan = engine.getLastSolutionPlan();
    ASSERT_EQ(plan.size(), 2);
    ASSERT_EQ(plan[0], transitions.getEdgeAction("a", "d"));
    ASSERT_EQ(plan[1], transitions.getEdgeAction("d", "d3"));
}

/**
 * Checks that the searchForPlan function works correctly, as does reset.
 */
TEST_F(BestFirstSearchSimpleGraphTests, searchForPlanAndResetTest) {
    engine.setEvaluator(f_cost_evaluator);
    engine.setTransitionSystem(transitions);
    engine.setGoalTest(goal_test);
    engine.setHashFunction(vertex_hash);

    engine.searchForPlan(init_state);
    ASSERT_TRUE(engine.hasFoundSolution());
    ASSERT_EQ(engine.getStatus(), EngineStatus::search_completed);
    ASSERT_EQ(engine.getLastSolutionPlanCost(), 2);

    auto plan = engine.getLastSolutionPlan();
    ASSERT_EQ(plan.size(), 2);
    ASSERT_EQ(plan[0], transitions.getEdgeAction("a", "d"));
    ASSERT_EQ(plan[1], transitions.getEdgeAction("d", "d3"));

    engine.reset();
    ASSERT_TRUE(engine.canRunSearch());
    ASSERT_EQ(engine.getOpenListSize(), 0);
    ASSERT_EQ(engine.getClosedListSize(), 0);
    ASSERT_EQ(engine.getCurrentActionList().size(), 0);
    ASSERT_EQ(engine.getStatus(), EngineStatus::ready);
    ASSERT_EQ(engine.getExpansionOrder().size(), 0);
}

/**
 * Checks that the statistics are being calculated correctly.
 */
TEST_F(BestFirstSearchSimpleGraphTests, getSearchStatisticsTest) {
    engine.setEvaluator(f_cost_evaluator);
    engine.setTransitionSystem(transitions);
    engine.setGoalTest(goal_test);
    engine.setHashFunction(vertex_hash);

    engine.searchForPlan(init_state);

    auto stats = engine.getStandardEngineStatistics();

    ASSERT_EQ(stats.m_num_actions_generated, 12);
    ASSERT_EQ(stats.m_num_get_actions_calls, 8);
    ASSERT_EQ(stats.m_num_goal_tests, 9);
    ASSERT_EQ(stats.m_num_states_generated, 12);
    ASSERT_EQ(stats.m_num_evals, 13);
}

/**
 * Checks that resource limits are applied correctly.
 */
TEST_F(BestFirstSearchSimpleGraphTests, resourceLimitsTest) {
    engine.setEvaluator(f_cost_evaluator);
    engine.setTransitionSystem(transitions);
    engine.setGoalTest(goal_test);
    engine.setHashFunction(vertex_hash);

    SearchResourceLimits limits;
    limits.m_get_actions_call_limit = 4;
    engine.setResourceLimits(limits);

    engine.searchForPlan(init_state);
    ASSERT_FALSE(engine.hasFoundSolution());
    ASSERT_EQ(engine.getStatus(), EngineStatus::resource_limit_hit);
    ASSERT_EQ(engine.getLastSolutionPlanCost(), -1);
    ASSERT_EQ(engine.getStandardEngineStatistics().m_num_get_actions_calls, 4);

    limits.m_get_actions_call_limit = 0;
    limits.m_goal_test_limit = 5;
    engine.reset();
    engine.setResourceLimits(limits);

    engine.searchForPlan(init_state);
    ASSERT_FALSE(engine.hasFoundSolution());
    ASSERT_EQ(engine.getStatus(), EngineStatus::resource_limit_hit);
    ASSERT_EQ(engine.getLastSolutionPlanCost(), -1);
    ASSERT_EQ(engine.getStandardEngineStatistics().m_num_goal_tests, 5);

    limits.m_goal_test_limit = 0;
    limits.m_node_eval_limit = 12;
    engine.reset();
    engine.setResourceLimits(limits);

    engine.searchForPlan(init_state);
    ASSERT_FALSE(engine.hasFoundSolution());
    ASSERT_EQ(engine.getStatus(), EngineStatus::resource_limit_hit);
    ASSERT_EQ(engine.getLastSolutionPlanCost(), -1);
    ASSERT_EQ(engine.getStandardEngineStatistics().m_num_evals, 12);

    limits.m_node_eval_limit = 0;
    limits.m_state_generation_limit = 11;
    engine.reset();
    engine.setResourceLimits(limits);

    engine.searchForPlan(init_state);
    ASSERT_FALSE(engine.hasFoundSolution());
    ASSERT_EQ(engine.getStatus(), EngineStatus::resource_limit_hit);
    ASSERT_EQ(engine.getLastSolutionPlanCost(), -1);
    ASSERT_EQ(engine.getStandardEngineStatistics().m_num_states_generated, 11);
}

/**
 * Checks that the real heuristic are worked correctly.
 */
TEST(BestFirstSearchGraphTests, problemWithHeuristicTest) {
    std::stringstream csv = std::stringstream("a;b;c 2;d\nb;e;f\nc;g;h\ng;goal");
    Graph graph = getGraphFromCSVAdjacencyList(csv);
    GraphTransitions transitions(graph);

    BestFirstSearchParams params;
    BestFirstSearch<GraphState, GraphAction, std::string> engine(params);

    SingleStateGoalTest<GraphState> goal_test(transitions.getVertexState("goal"));

    StateStringHashFunction<GraphState> hasher;
    HashMapHeuristic<GraphState, GraphAction, std::string> heuristic(hasher);
    heuristic.addHeuristicValue(transitions.getVertexState("a"), 2, false);
    heuristic.addHeuristicValue(transitions.getVertexState("b"), 1, true);
    heuristic.addHeuristicValue(transitions.getVertexState("c"), 1, false);
    heuristic.addHeuristicValue(transitions.getVertexState("d"), 1, true);
    heuristic.addHeuristicValue(transitions.getVertexState("g"), 1, false);
    heuristic.addHeuristicValue(transitions.getVertexState("h"), 2, false);
    heuristic.addHeuristicValue(transitions.getVertexState("goal"), 0, false);

    GraphState init_state = transitions.getVertexState("a");

    FCostEvaluator<GraphState, GraphAction> f_cost_evaluator(heuristic);
    engine.setEvaluator(f_cost_evaluator);
    engine.setTransitionSystem(transitions);
    engine.setGoalTest(goal_test);
    engine.setHashFunction(hasher);

    engine.searchForPlan(init_state);
    ASSERT_TRUE(engine.hasFoundSolution());
    ASSERT_EQ(engine.getStatus(), EngineStatus::search_completed);
    ASSERT_EQ(engine.getLastSolutionPlanCost(), 4);
    auto plan = engine.getLastSolutionPlan();
    ASSERT_EQ(plan.size(), 3);
    ASSERT_EQ(plan[0], transitions.getEdgeAction("a", "c"));
    ASSERT_EQ(plan[1], transitions.getEdgeAction("c", "g"));
    ASSERT_EQ(plan[2], transitions.getEdgeAction("g", "goal"));
}

/**
 * Checks that the tie breaking rule and weight are worked correctly.
 */
TEST(BestFirstSearchGraphTests, paramsInputTest) {
    std::stringstream csv = std::stringstream("a;c 2;b 1\nb;d 1\nc;e 2\nd;f 1\nf;g 1\ne;goal 1\ng;goal 1");
    Graph graph = getGraphFromCSVAdjacencyList(csv);
    GraphTransitions transitions(graph);

    BestFirstSearchParams params;
    BestFirstSearch<GraphState, GraphAction, std::string> engine(params);

    SingleStateGoalTest<GraphState> goal_test(transitions.getVertexState("goal"));

    StateStringHashFunction<GraphState> hasher;
    HashMapHeuristic<GraphState, GraphAction, std::string> heuristic(hasher);
    heuristic.addHeuristicValue(transitions.getVertexState("a"), 4, false);
    heuristic.addHeuristicValue(transitions.getVertexState("b"), 4, false);
    heuristic.addHeuristicValue(transitions.getVertexState("c"), 3, false);
    heuristic.addHeuristicValue(transitions.getVertexState("d"), 3, false);
    heuristic.addHeuristicValue(transitions.getVertexState("e"), 1, false);
    heuristic.addHeuristicValue(transitions.getVertexState("f"), 2, false);
    heuristic.addHeuristicValue(transitions.getVertexState("g"), 1, false);
    heuristic.addHeuristicValue(transitions.getVertexState("goal"), 0, false);

    GraphState init_state = transitions.getVertexState("a");

    FCostEvaluator<GraphState, GraphAction> f_cost_evaluator(heuristic);
    engine.setEvaluator(f_cost_evaluator);
    engine.setTransitionSystem(transitions);
    engine.setGoalTest(goal_test);
    engine.setHashFunction(hasher);

    // Default tie-breaking should find the short path
    // Expansion order is a, c, b, e, d, goal
    engine.searchForPlan(init_state);
    ASSERT_TRUE(engine.hasFoundSolution());
    ASSERT_EQ(engine.getStatus(), EngineStatus::search_completed);
    ASSERT_EQ(engine.getLastSolutionPlanCost(), 5);
    auto regular_plan = engine.getLastSolutionPlan();
    ASSERT_EQ(regular_plan.size(), 3);
    ASSERT_EQ(engine.getExpansionOrder().size(), 0);  // default m_store_expansion_order is false

    auto stats = engine.getStandardEngineStatistics();
    ASSERT_EQ(stats.m_num_goal_tests, 6);
    params.m_store_expansion_order = true;  // set m_store_expansion_order to true

    // High-G tie-breaking should find the short path
    // Expansion order is a, c, e, goal
    GCostEvaluator<GraphState, GraphAction> g_cost;
    EvalsAndUsageVec<GraphState, GraphAction> evals;
    evals.emplace_back(f_cost_evaluator, true);
    evals.emplace_back(g_cost, false);  // Hi-G tie-breaking
    engine.setEvaluators(evals);

    engine.setEngineParams(params);
    engine.searchForPlan(init_state);
    auto high_g_plan = engine.getLastSolutionPlan();
    ASSERT_EQ(high_g_plan.size(), 3);
    ASSERT_TRUE(engine.hasFoundSolution());
    ASSERT_EQ(engine.getExpansionOrder().size(), 4);  // m_store_expansion_order set to true, so m_expansion_order should contain nodes

    stats = engine.getStandardEngineStatistics();
    ASSERT_EQ(stats.m_num_goal_tests, 4);
    params.m_store_expansion_order = false;

    // Finds shorter path though expands everything
    // Expansion order is a, b, c, d, f, e, g, goal
    evals.pop_back();
    evals.emplace_back(g_cost, true);  // Low-G tiebreaking
    engine.setEvaluators(evals);

    engine.setEngineParams(params);
    engine.searchForPlan(init_state);
    auto low_g_plan = engine.getLastSolutionPlan();
    ASSERT_EQ(low_g_plan.size(), 3);
    ASSERT_TRUE(engine.hasFoundSolution());
    ASSERT_EQ(engine.getExpansionOrder().size(), 0);  // m_store_expansion_order set to false again, to make sure it doesn't store the expansion order anymore

    stats = engine.getStandardEngineStatistics();
    ASSERT_EQ(stats.m_num_goal_tests, 8);

    //    // High weight should just follow short path
    //    params.m_use_tie_breaking = TieBreaking::no_rule;
    //    f_cost_evaluator.setWeight(3);
    //    engine.setEngineParams(params);
    //    engine.searchForPlan(init_state);
    //    auto large_weight_plan = engine.getLastSolutionPlan();
    //    ASSERT_EQ(large_weight_plan.size(), 3);
    //    ASSERT_TRUE(engine.hasFoundSolution());
    //
    //    stats = engine.getSearchStatistics();
    //    // large weight also cost fewer actions
    //    ASSERT_EQ(stats.m_num_goal_tests, 4);
}

// TODO Re-enable parent updating tests
///**
// * Checks that the updating of parent heuristic are worked correctly.
// */
//TEST(BestFirstSearchGraphTests, setUpdateParentHeuristicTest) {
//    std::stringstream csv = std::stringstream("a;b 6;c 7;d 7\nc;goal 5");
//    Graph graph = getGraphFromCSVAdjacencyList(csv);
//    GraphTransitions transitions(graph);
//
//    BestFirstSearchParams params;
//    BestFirstSearch<GraphState, GraphAction, std::string> engine(params);
//
//    SingleGoalTest<GraphState> goal_test(transitions.getVertexState("goal"));
//
//    StateStringHashFunction<GraphState> hasher;
//    HashMapHeuristic<GraphState, GraphAction, std::string> heuristic(&hasher);
//    heuristic.addHeuristicValue(transitions.getVertexState("a"), 10, false);
//    heuristic.addHeuristicValue(transitions.getVertexState("b"), 5, false);
//    heuristic.addHeuristicValue(transitions.getVertexState("c"), 5, false);
//    heuristic.addHeuristicValue(transitions.getVertexState("d"), 5, true);
//    heuristic.addHeuristicValue(transitions.getVertexState("goal"), 0, false);
//
//    GraphState init_state = transitions.getVertexState("a");
//
//    FCostEvaluator<GraphState, GraphAction> f_cost_evaluator(heuristic);
//    engine.setEvaluator(f_cost_evaluator);
//    engine.setTransitionSystem(transitions);
//    engine.setGoalTest(goal_test);
//    engine.setHashFunction(&hasher);
//
//    engine.searchForPlan(init_state);
//    ASSERT_TRUE(engine.hasFoundSolution());
//    ASSERT_EQ(engine.getStatus(), EngineStatus::search_completed);
//    ASSERT_EQ(engine.getLastSolutionPlanCost(), 12);
//    auto regular_plan = engine.getLastSolutionPlan();
//    ASSERT_EQ(regular_plan.size(), 2);
//
//    auto stats = engine.getSearchStatistics();
//    ASSERT_EQ(stats.m_num_actions_generated, 4);
//    ASSERT_EQ(stats.m_num_parent_updates, 0);
//    const std::vector<Node<GraphState, GraphAction>>& cur = engine.getCurrentExpandNode();
//    Node<GraphState, GraphAction> parent_node = cur[0];
//    ASSERT_EQ(parent_node.m_state.m_graph->getVertexLabel(parent_node.m_state.m_vertex_id), "a");
//    ASSERT_EQ(parent_node.m_evals[0], 10);
//
//    params.m_parent_heuristic_updating = true;
//    engine.setEngineParams(params);
//    engine.searchForPlan(init_state);
//    auto updating_plan = engine.getLastSolutionPlan();
//    ASSERT_EQ(updating_plan.size(), 2);
//
//    stats = engine.getSearchStatistics();
//    ASSERT_EQ(stats.m_num_actions_generated, 4);
//    ASSERT_EQ(stats.m_num_parent_updates, 1);
//    const std::vector<Node<GraphState, GraphAction>>& update_cur = engine.getCurrentExpandNode();
//    Node<GraphState, GraphAction> update_parent_node = update_cur[0];
//
//    ASSERT_EQ(update_parent_node.m_state.m_graph->getVertexLabel(update_parent_node.m_state.m_vertex_id), "a");
//    ASSERT_EQ(update_parent_node.m_evals[0], 11);
//    ASSERT_FALSE(update_parent_node.m_is_dead_end);
//
//    ASSERT_EQ(update_cur[1].m_state.m_graph->getVertexLabel(update_cur[1].m_state.m_vertex_id), "b");
//    ASSERT_EQ(update_cur[1].m_evals[0], 5);
//    ASSERT_TRUE(update_cur[1].m_is_dead_end);
//}
//
///**
// * Checks that the updating of parent heuristic are worked correctly if there is no need to update
// * Same graph with the previous test, just increase the h_value of node a
// */
//TEST(BestFirstSearchGraphTests, updateParentHeuristicNoNeedTest) {
//    std::stringstream csv = std::stringstream("a;b 6;c 7;d 7\nc;goal 5");
//    Graph graph = getGraphFromCSVAdjacencyList(csv);
//    GraphTransitions transitions(graph);
//
//    BestFirstSearchParams params;
//    BestFirstSearch<GraphState, GraphAction, std::string> engine(params);
//
//    SingleGoalTest<GraphState> goal_test(transitions.getVertexState("goal"));
//
//    StateStringHashFunction<GraphState> hasher;
//    HashMapHeuristic<GraphState, GraphAction, std::string> heuristic(&hasher);
//    heuristic.addHeuristicValue(transitions.getVertexState("a"), 12, false);
//    heuristic.addHeuristicValue(transitions.getVertexState("b"), 5, false);
//    heuristic.addHeuristicValue(transitions.getVertexState("c"), 5, false);
//    heuristic.addHeuristicValue(transitions.getVertexState("d"), 5, true);
//    heuristic.addHeuristicValue(transitions.getVertexState("goal"), 0, false);
//
//    GraphState init_state = transitions.getVertexState("a");
//
//    FCostEvaluator<GraphState, GraphAction> f_cost_evaluator(heuristic);
//    engine.setEvaluator(f_cost_evaluator);
//    engine.setTransitionSystem(transitions);
//    engine.setGoalTest(goal_test);
//    engine.setHashFunction(&hasher);
//
//    engine.searchForPlan(init_state);
//    ASSERT_TRUE(engine.hasFoundSolution());
//    ASSERT_EQ(engine.getStatus(), EngineStatus::search_completed);
//    ASSERT_EQ(engine.getLastSolutionPlanCost(), 12);
//    auto regular_plan = engine.getLastSolutionPlan();
//    ASSERT_EQ(regular_plan.size(), 2);
//
//    auto stats = engine.getSearchStatistics();
//    ASSERT_EQ(stats.m_num_actions_generated, 4);
//    ASSERT_EQ(stats.m_num_parent_updates, 0);
//    const std::vector<Node<GraphState, GraphAction>>& cur = engine.getCurrentExpandNode();
//    Node<GraphState, GraphAction> parent_node = cur[0];
//
//    ASSERT_EQ(parent_node.m_state.m_graph->getVertexLabel(parent_node.m_state.m_vertex_id), "a");
//    ASSERT_EQ(parent_node.m_evals[0], 12);
//
//    params.m_parent_heuristic_updating = true;
//    engine.setEngineParams(params);
//    engine.searchForPlan(init_state);
//    auto updating_plan = engine.getLastSolutionPlan();
//    ASSERT_EQ(updating_plan.size(), 2);
//
//    stats = engine.getSearchStatistics();
//    ASSERT_EQ(stats.m_num_actions_generated, 4);
//    ASSERT_EQ(stats.m_num_parent_updates, 0);
//    const std::vector<Node<GraphState, GraphAction>>& update_cur = engine.getCurrentExpandNode();
//    Node<GraphState, GraphAction> update_parent_node = update_cur[0];
//
//    ASSERT_EQ(update_parent_node.m_state.m_graph->getVertexLabel(update_parent_node.m_state.m_vertex_id), "a");
//    ASSERT_EQ(update_parent_node.m_evals[0], 12);
//    ASSERT_FALSE(update_parent_node.m_is_dead_end);
//}

/**
 * Checks that the reopened node are Check if the engine can successfully set a node with no children to dead end correctly.
 */
TEST(BestFirstSearchGraphTests, nodeReopenedTest) {
    std::stringstream csv = std::stringstream("a;b 5;c 1\nb;d 1\nc;b 1\nd;goal 4");
    Graph graph = getGraphFromCSVAdjacencyList(csv);
    GraphTransitions transitions(graph);

    BestFirstSearchParams params;
    BestFirstSearch<GraphState, GraphAction, std::string> engine(params);

    SingleStateGoalTest<GraphState> goal_test(transitions.getVertexState("goal"));

    StateStringHashFunction<GraphState> hasher;
    HashMapHeuristic<GraphState, GraphAction, std::string> heuristic(hasher);
    heuristic.addHeuristicValue(transitions.getVertexState("a"), 7, false);
    heuristic.addHeuristicValue(transitions.getVertexState("b"), 1, false);
    heuristic.addHeuristicValue(transitions.getVertexState("c"), 6, false);
    heuristic.addHeuristicValue(transitions.getVertexState("d"), 4, false);
    heuristic.addHeuristicValue(transitions.getVertexState("goal"), 0, false);

    GraphState init_state = transitions.getVertexState("a");

    FCostEvaluator<GraphState, GraphAction> f_cost_evaluator(heuristic);
    engine.setEvaluator(f_cost_evaluator);
    engine.setTransitionSystem(transitions);
    engine.setGoalTest(goal_test);
    engine.setHashFunction(hasher);
    params.m_store_expansion_order = true;
    engine.setEngineParams(params);

    engine.searchForPlan(init_state);
    ASSERT_TRUE(engine.hasFoundSolution());
    ASSERT_EQ(engine.getStatus(), EngineStatus::search_completed);
    ASSERT_EQ(engine.getLastSolutionPlanCost(), 7);
    auto plan = engine.getLastSolutionPlan();
    ASSERT_EQ(plan.size(), 4);
    ASSERT_EQ(plan[0], transitions.getEdgeAction("a", "c"));
    ASSERT_EQ(plan[1], transitions.getEdgeAction("c", "b"));
    ASSERT_EQ(plan[2], transitions.getEdgeAction("b", "d"));
    ASSERT_EQ(plan[3], transitions.getEdgeAction("d", "goal"));
    auto stats = engine.getStandardEngineStatistics();
    // Generate: ABCBDE
    ASSERT_EQ(stats.m_num_get_actions_calls, 5);

    const std::vector<NodeID>& cur = engine.getExpansionOrder();

    ASSERT_EQ(cur.size(), 6);
    ASSERT_EQ((engine.getNodes()).getState(cur[0]).m_graph->getVertexLabel((engine.getNodes()).getState(cur[0]).m_vertex_id), "a");
    ASSERT_EQ(f_cost_evaluator.getCachedEval(0), 7);
    ASSERT_EQ((engine.getNodes()).getState(cur[1]).m_graph->getVertexLabel((engine.getNodes()).getState(cur[1]).m_vertex_id), "b");
    ASSERT_EQ(f_cost_evaluator.getCachedEval(1), 3);
    ASSERT_EQ((engine.getNodes()).getState(cur[2]).m_graph->getVertexLabel((engine.getNodes()).getState(cur[2]).m_vertex_id), "c");
    ASSERT_EQ(f_cost_evaluator.getCachedEval(2), 7);
    ASSERT_EQ((engine.getNodes()).getState(cur[3]).m_graph->getVertexLabel((engine.getNodes()).getState(cur[3]).m_vertex_id), "b");
    ASSERT_EQ((engine.getNodes()).getState(cur[4]).m_graph->getVertexLabel((engine.getNodes()).getState(cur[4]).m_vertex_id), "d");
    ASSERT_EQ(f_cost_evaluator.getCachedEval(3), 7);
    ASSERT_EQ((engine.getNodes()).getState(cur[5]).m_graph->getVertexLabel((engine.getNodes()).getState(cur[5]).m_vertex_id), "goal");
    ASSERT_EQ(f_cost_evaluator.getCachedEval(4), 7);

    params.m_use_reopened = false;
    engine.setEngineParams(params);

    engine.searchForPlan(init_state);
    ASSERT_TRUE(engine.hasFoundSolution());
    ASSERT_EQ(engine.getStatus(), EngineStatus::search_completed);
    ASSERT_EQ(engine.getLastSolutionPlanCost(), 7);
    plan = engine.getLastSolutionPlan();
    ASSERT_EQ(plan.size(), 4);
    ASSERT_EQ(plan[0], transitions.getEdgeAction("a", "c"));
    ASSERT_EQ(plan[1], transitions.getEdgeAction("c", "b"));
    ASSERT_EQ(plan[2], transitions.getEdgeAction("b", "d"));
    ASSERT_EQ(plan[3], transitions.getEdgeAction("d", "goal"));
    stats = engine.getStandardEngineStatistics();
    // Generate: ABCDE
    ASSERT_EQ(stats.m_num_get_actions_calls, 4);

    const std::vector<NodeID>& reopen_cur = engine.getExpansionOrder();

    ASSERT_EQ(reopen_cur.size(), 5);
    ASSERT_EQ((engine.getNodes()).getState(reopen_cur[0]).m_graph->getVertexLabel((engine.getNodes()).getState(reopen_cur[0]).m_vertex_id), "a");
    ASSERT_EQ(f_cost_evaluator.getCachedEval(0), 7);
    ASSERT_EQ((engine.getNodes()).getState(reopen_cur[1]).m_graph->getVertexLabel((engine.getNodes()).getState(reopen_cur[1]).m_vertex_id), "b");
    ASSERT_EQ(f_cost_evaluator.getCachedEval(1), 3);
    ASSERT_EQ((engine.getNodes()).getState(reopen_cur[2]).m_graph->getVertexLabel((engine.getNodes()).getState(reopen_cur[2]).m_vertex_id), "c");
    ASSERT_EQ(f_cost_evaluator.getCachedEval(2), 7);
    ASSERT_EQ((engine.getNodes()).getState(reopen_cur[3]).m_graph->getVertexLabel((engine.getNodes()).getState(reopen_cur[3]).m_vertex_id), "d");
    ASSERT_EQ(f_cost_evaluator.getCachedEval(3), 10);
    ASSERT_EQ((engine.getNodes()).getState(reopen_cur[4]).m_graph->getVertexLabel((engine.getNodes()).getState(reopen_cur[4]).m_vertex_id), "goal");
    ASSERT_EQ(f_cost_evaluator.getCachedEval(4), 10);
}

/**
* Tests that get settings works as intended
*/
TEST(BestFirstSearchGraphTests, getSettingsTest) {
    using namespace evalFunctionTerms;

    std::stringstream csv = std::stringstream("a;b 5;c 1\nb;d 1\nc;b 1\nd;goal 4");
    Graph graph = getGraphFromCSVAdjacencyList(csv);
    GraphTransitions transitions(graph);

    BestFirstSearchParams params;
    BestFirstSearch<GraphState, GraphAction, std::string> engine(params);

    SingleStateGoalTest<GraphState> goal_test(transitions.getVertexState("goal"));

    StateStringHashFunction<GraphState> hasher;
    HashMapHeuristic<GraphState, GraphAction, std::string> heuristic(hasher);

    FCostEvaluator<GraphState, GraphAction> f_cost_evaluator(heuristic);
    engine.setEvaluator(f_cost_evaluator);
    engine.setTransitionSystem(transitions);
    engine.setGoalTest(goal_test);
    engine.setHashFunction(hasher);

    auto engine_settings = engine.getAllSettings();
    ASSERT_EQ(engine_settings.m_name, "BestFirstSearch");
    auto& main_settings = engine_settings.m_main_settings;
    ASSERT_EQ(main_settings.size(), 4);
    ASSERT_EQ(main_settings.at("use_stored_seed"), "false");
    ASSERT_TRUE(main_settings.find("random_seed") != main_settings.end());
    ASSERT_EQ(main_settings.at("use_reopened"), "true");
    ASSERT_EQ(main_settings.at("store_expansion_order"), "false");

    ASSERT_EQ(engine_settings.m_sub_component_settings.size(), 2);

    auto& eval_settings = engine_settings.m_sub_component_settings.at("eval_function");
    ASSERT_EQ(eval_settings.m_name, f_cost_evaluator.CLASS_NAME);
    ASSERT_EQ(eval_settings.m_main_settings.size(), 0);
    ASSERT_EQ(eval_settings.m_sub_component_settings.size(), 1);
    ASSERT_EQ(eval_settings.m_sub_component_settings.at(SETTING_HEURISTIC).m_name, heuristic.CLASS_NAME);

    auto& hash_settings = engine_settings.m_sub_component_settings.at("hash_function");
    ASSERT_EQ(hash_settings.m_name, "StateStringHashFunction");
    ASSERT_EQ(hash_settings.m_main_settings.size(), 0);
    ASSERT_EQ(hash_settings.m_sub_component_settings.size(), 0);
}