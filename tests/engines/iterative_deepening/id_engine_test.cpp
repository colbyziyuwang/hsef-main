#include <gtest/gtest.h>

#include "building_tools/evaluators/constant_heuristic.h"
#include "building_tools/evaluators/hash_map_heuristic.h"
#include "building_tools/goal_tests/single_state_goal_test.h"
#include "building_tools/hashing/state_string_hash_function.h"
#include "engines/engine_components/eval_functions/eval_function_terms.h"
#include "engines/engine_components/eval_functions/f_cost_evaluator.h"
#include "engines/iterative_deepening/id_engine.h"
#include "engines/iterative_deepening/id_engine_params.h"
#include "environments/graph/graph_transitions.h"
#include "environments/graph/graph_utils.h"

/**
 * Creates a fixture for IDEngine tests. Just a simple complete tree to depth 2 and will use a zero heuristic.
 */
class IDEngineSimpleGraphTests : public ::testing::Test {
protected:
    void SetUp() override {
        graph.sortOutEdgesByToVertexLabel();
    }

public:
    std::stringstream csv = std::stringstream("a;b;c;d\nb;b1;b2\nc;c1;c2;c3\nd;d1;d2;d3;d4");  ///< Defines simple 3 level tree
    Graph graph = getGraphFromCSVAdjacencyList(csv);  ///< Turns adjacency list into graph
    GraphTransitions transitions = GraphTransitions(graph);  ///< Turns graph into transitions

    IDEngineParams params;  ///< Using basic parameters
    IDEngine<GraphState, GraphAction> engine = IDEngine<GraphState, GraphAction>(params);  ///< Here is the engine

    SingleStateGoalTest<GraphState> goal_test = SingleStateGoalTest<GraphState>(transitions.getVertexState("d3"));  ///<Sets goal as d3
    ConstantHeuristic<GraphState, GraphAction> zero_heuristic;  ///< Use the zero heuristic
    FCostEvaluator<GraphState, GraphAction> f_cost_evaluator = FCostEvaluator<GraphState, GraphAction>(zero_heuristic);
    GraphState init_state = transitions.getVertexState("a");  ///< Sets the initial state as vertex a
};

/**
 * Tests that the constructor sets everything as expected.
 */
TEST_F(IDEngineSimpleGraphTests, constructorTest) {
    ASSERT_FALSE(engine.canRunSearch());
    ASSERT_TRUE(engine.getNodes().size() == 0);
    ASSERT_TRUE(engine.getActionStack().empty());
    ASSERT_TRUE(engine.getActionIndexStack().empty());
    ASSERT_TRUE(engine.getThresholds().empty());
    ASSERT_EQ(engine.getNextThreshold(), -1.0);
    ASSERT_EQ(engine.getStatus(), EngineStatus::not_ready);

    StandardSearchStatistics stats = engine.getStandardEngineStatistics();
    ASSERT_EQ(stats.m_num_goal_tests, 0);

    ASSERT_FALSE(engine.hasFoundSolution());
    ASSERT_TRUE(engine.getLastSolutionPlan().empty());
    ASSERT_EQ(engine.getLastSolutionPlanCost(), -1.0);
}

/**
 * Sets that the IDA* can run properly once all necessary parts have been set.
 */
TEST_F(IDEngineSimpleGraphTests, setAndCanRunTest) {
    engine.setEvaluator(f_cost_evaluator);
    ASSERT_FALSE(engine.canRunSearch());

    engine.setTransitionSystem(transitions);
    ASSERT_FALSE(engine.canRunSearch());

    engine.setGoalTest(goal_test);
    ASSERT_TRUE(engine.canRunSearch());

    ASSERT_EQ(engine.getStatus(), EngineStatus::ready);
}

/**
 * Checks that initialize search sets things up correctly.
 */
TEST_F(IDEngineSimpleGraphTests, initializeSearchTest) {
    engine.setEvaluator(f_cost_evaluator);
    engine.setTransitionSystem(transitions);
    engine.setGoalTest(goal_test);

    engine.initializeSearch(init_state);
    ASSERT_EQ(engine.getStatus(), EngineStatus::active);

    const auto& current_nodes = engine.getNodes();
    ASSERT_EQ(current_nodes.size(), 1);

    ASSERT_EQ(current_nodes.getState(0), init_state);
    ASSERT_EQ(current_nodes.getLastAction(0), std::nullopt);
    ASSERT_EQ(current_nodes.getGValue(0), 0.0);
    ASSERT_EQ(zero_heuristic.getCachedEval(0), 0.0);
    ASSERT_FALSE(f_cost_evaluator.getCachedIsDeadEnd(0));
    ASSERT_EQ(f_cost_evaluator.getCachedEval(0), 0.0);

    ASSERT_TRUE(engine.getActionStack().empty());
    ASSERT_TRUE(engine.getActionIndexStack().empty());
    ASSERT_EQ(engine.getThresholds().size(), 1);
    ASSERT_EQ(engine.getThresholds()[0], 0.0);
    ASSERT_EQ(engine.getNextThreshold(), -1.0);
}

/**
 * Runs step by step through a search on the simple graph to make sure the right thing is expanded at every step.
 */
TEST_F(IDEngineSimpleGraphTests, singleSearchStepTest) {
    engine.setEvaluator(f_cost_evaluator);
    engine.setTransitionSystem(transitions);
    engine.setGoalTest(goal_test);
    engine.initializeSearch(init_state);
    ASSERT_EQ(engine.getNodes().size(), 1);

    engine.singleSearchStep();
    ASSERT_EQ(engine.getActionStack().size(), 1);
    ASSERT_EQ(engine.getActionStack()[0].size(), 3);
    ASSERT_EQ(engine.getActionIndexStack().size(), 1);
    ASSERT_EQ(engine.getActionIndexStack()[0], 0u);
    ASSERT_EQ(engine.getNodes().size(), 2);
    ASSERT_EQ(engine.getNextThreshold(), -1.0);

    const auto& current_nodes = engine.getNodes();
    NodeID last_id = current_nodes.size() - 1;

    ASSERT_EQ(current_nodes.getState(last_id), transitions.getVertexState("b"));
    ASSERT_FALSE(f_cost_evaluator.getCachedIsDeadEnd(engine.getNodes().size() - 1));
    ASSERT_EQ(zero_heuristic.getCachedEval(engine.getNodes().size() - 1), 0.0);
    ASSERT_EQ(current_nodes.getGValue(last_id), 1.0);
    ASSERT_EQ(f_cost_evaluator.getCachedEval(engine.getNodes().size() - 1), 1.0);
    ASSERT_EQ(current_nodes.getLastAction(last_id), transitions.getEdgeAction("a", "b"));
    ASSERT_EQ(current_nodes.getParentID(last_id), 0);

    engine.singleSearchStep();
    ASSERT_EQ(engine.getNodes().size(), 2);
    ASSERT_EQ(engine.getNodes().getState(engine.getNodes().size() - 1), transitions.getVertexState("c"));
    ASSERT_EQ(engine.getNextThreshold(), 1.0);

    engine.singleSearchStep();
    ASSERT_EQ(engine.getNodes().size(), 2);
    ASSERT_EQ(engine.getNodes().getState(engine.getNodes().size() - 1), transitions.getVertexState("d"));

    engine.singleSearchStep();
    ASSERT_EQ(engine.getNodes().size(), 1);
    ASSERT_EQ(engine.getNodes().getState(engine.getNodes().size() - 1), transitions.getVertexState("a"));

    ASSERT_TRUE(engine.getActionStack().empty());
    ASSERT_TRUE(engine.getActionIndexStack().empty());
    ASSERT_EQ(engine.getThresholds().size(), 2);
    ASSERT_EQ(engine.getThresholds().back(), 1.0);
    ASSERT_EQ(engine.getNextThreshold(), -1.0);

    engine.singleSearchStep();
    engine.singleSearchStep();
    ASSERT_EQ(engine.getNodes().size(), 3);
    ASSERT_EQ(engine.getNodes().getState(engine.getNodes().size() - 1), transitions.getVertexState("b1"));
    ASSERT_EQ(engine.getNodes().getParentID(engine.getNodes().size() - 1), 1);

    engine.singleSearchStep();
    ASSERT_EQ(engine.getNodes().getState(engine.getNodes().size() - 1), transitions.getVertexState("b2"));
    ASSERT_EQ(engine.getNodes().size(), 3);

    engine.singleSearchStep();
    ASSERT_EQ(engine.getNodes().getState(engine.getNodes().size() - 1), transitions.getVertexState("c"));
    ASSERT_EQ(engine.getNodes().size(), 2);

    engine.singleSearchStep();
    ASSERT_EQ(engine.getNodes().getState(engine.getNodes().size() - 1), transitions.getVertexState("c1"));
    ASSERT_EQ(engine.getNodes().size(), 3);

    engine.singleSearchStep();
    ASSERT_EQ(engine.getNodes().getState(engine.getNodes().size() - 1), transitions.getVertexState("c2"));
    ASSERT_EQ(engine.getNodes().getParentID(engine.getNodes().size() - 1), 1);
    ASSERT_EQ(engine.getNodes().size(), 3);

    engine.singleSearchStep();
    ASSERT_EQ(engine.getNodes().getState(engine.getNodes().size() - 1), transitions.getVertexState("c3"));
    ASSERT_EQ(engine.getNodes().size(), 3);

    engine.singleSearchStep();
    ASSERT_EQ(engine.getNodes().getState(engine.getNodes().size() - 1), transitions.getVertexState("d"));
    ASSERT_EQ(engine.getNodes().size(), 2);

    engine.singleSearchStep();
    ASSERT_EQ(engine.getNodes().getState(engine.getNodes().size() - 1), transitions.getVertexState("d1"));
    ASSERT_EQ(engine.getNodes().size(), 3);

    engine.singleSearchStep();
    ASSERT_EQ(engine.getNodes().getState(engine.getNodes().size() - 1), transitions.getVertexState("d2"));
    ASSERT_EQ(engine.getNodes().size(), 3);

    engine.singleSearchStep();
    ASSERT_EQ(engine.getNodes().getState(engine.getNodes().size() - 1), transitions.getVertexState("d3"));
    ASSERT_EQ(engine.getNodes().size(), 3);
    ASSERT_EQ(engine.getNodes().getParentID(engine.getNodes().size() - 1), 1);

    engine.singleSearchStep();
    ASSERT_EQ(engine.getNodes().getState(engine.getNodes().size() - 1), transitions.getVertexState("d4"));
    ASSERT_EQ(engine.getNodes().size(), 3);

    engine.singleSearchStep();
    ASSERT_EQ(engine.getNodes().size(), 1);
    ASSERT_EQ(engine.getNodes().getState(engine.getNodes().size() - 1), transitions.getVertexState("a"));

    ASSERT_TRUE(engine.getActionStack().empty());
    ASSERT_TRUE(engine.getActionIndexStack().empty());
    ASSERT_EQ(engine.getThresholds().size(), 3);
    ASSERT_EQ(engine.getThresholds().back(), 2.0);
    ASSERT_EQ(engine.getNextThreshold(), -1.0);

    engine.singleSearchStep();
    engine.singleSearchStep();
    engine.singleSearchStep();
    ASSERT_EQ(engine.getNodes().getState(engine.getNodes().size() - 1), transitions.getVertexState("b2"));  // Should handle that b1 has no children appropriately
    ASSERT_EQ(engine.getNodes().size(), 3);

    for (unsigned i = 0; i < 8; i++) {
        engine.singleSearchStep();
    }
    ASSERT_FALSE(engine.hasFoundSolution());
    ASSERT_EQ(engine.getNodes().getState(engine.getNodes().size() - 1), transitions.getVertexState("d3"));

    engine.singleSearchStep();
    ASSERT_TRUE(engine.hasFoundSolution());
    ASSERT_EQ(engine.getStatus(), EngineStatus::search_completed);
    ASSERT_EQ(engine.getLastSolutionPlanCost(), 2);

    auto plan = engine.getLastSolutionPlan();
    ASSERT_EQ(plan.size(), 2);
    ASSERT_EQ(plan[0], transitions.getEdgeAction("a", "d"));
    ASSERT_EQ(plan[1], transitions.getEdgeAction("d", "d3"));

    ASSERT_EQ(engine.getNodes().size(), 3);
    ASSERT_EQ(engine.getNodes().getState(0), transitions.getVertexState("a"));
    ASSERT_EQ(engine.getNodes().getState(1), transitions.getVertexState("d"));
    ASSERT_EQ(engine.getNodes().getState(2), transitions.getVertexState("d3"));
}

/**
 * Checks that the searchForPlan function works correctly, as does reset.
 */
TEST_F(IDEngineSimpleGraphTests, searchForPlanAndResetTest) {
    engine.setEvaluator(f_cost_evaluator);
    engine.setTransitionSystem(transitions);
    engine.setGoalTest(goal_test);

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
    ASSERT_TRUE(engine.getNodes().size() == 0);
    ASSERT_TRUE(engine.getActionStack().empty());
    ASSERT_TRUE(engine.getActionIndexStack().empty());
    ASSERT_TRUE(engine.getThresholds().empty());
    ASSERT_EQ(engine.getNextThreshold(), -1.0);
    ASSERT_EQ(engine.getStatus(), EngineStatus::ready);
}

/**
 * Checks that the statistics are being calculated correctly.
 */
TEST_F(IDEngineSimpleGraphTests, getSearchStatisticsTest) {
    engine.setEvaluator(f_cost_evaluator);
    engine.setTransitionSystem(transitions);
    engine.setGoalTest(goal_test);

    engine.searchForPlan(init_state);

    auto stats = engine.getStandardEngineStatistics();

    ASSERT_EQ(stats.m_num_actions_generated, 27);
    ASSERT_EQ(stats.m_num_get_actions_calls, 16);
    ASSERT_EQ(stats.m_num_goal_tests, 17);
    ASSERT_EQ(stats.m_num_states_generated, 26);
    ASSERT_EQ(stats.m_num_evals, 29);
}

/**
 * Checks that resource limits are applied correctly.
 */
TEST_F(IDEngineSimpleGraphTests, resourceLimitsTest) {
    engine.setEvaluator(f_cost_evaluator);
    engine.setTransitionSystem(transitions);
    engine.setGoalTest(goal_test);

    SearchResourceLimits limits;
    limits.m_get_actions_call_limit = 14;
    engine.setResourceLimits(limits);

    engine.searchForPlan(init_state);
    ASSERT_FALSE(engine.hasFoundSolution());
    ASSERT_EQ(engine.getStatus(), EngineStatus::resource_limit_hit);
    ASSERT_EQ(engine.getLastSolutionPlanCost(), -1);
    ASSERT_EQ(engine.getStandardEngineStatistics().m_num_get_actions_calls, 14);

    limits.m_get_actions_call_limit = 0;
    limits.m_goal_test_limit = 12;
    engine.setResourceLimits(limits);

    engine.searchForPlan(init_state);
    ASSERT_FALSE(engine.hasFoundSolution());
    ASSERT_EQ(engine.getStatus(), EngineStatus::resource_limit_hit);
    ASSERT_EQ(engine.getLastSolutionPlanCost(), -1);
    ASSERT_EQ(engine.getStandardEngineStatistics().m_num_goal_tests, 12);

    limits.m_goal_test_limit = 0;
    limits.m_node_eval_limit = 21;
    engine.setResourceLimits(limits);

    engine.searchForPlan(init_state);
    ASSERT_FALSE(engine.hasFoundSolution());
    ASSERT_EQ(engine.getStatus(), EngineStatus::resource_limit_hit);
    ASSERT_EQ(engine.getLastSolutionPlanCost(), -1);
    ASSERT_EQ(engine.getStandardEngineStatistics().m_num_evals, 21);

    limits.m_node_eval_limit = 0;
    limits.m_state_generation_limit = 23;
    engine.setResourceLimits(limits);

    engine.searchForPlan(init_state);
    ASSERT_FALSE(engine.hasFoundSolution());
    ASSERT_EQ(engine.getStatus(), EngineStatus::resource_limit_hit);
    ASSERT_EQ(engine.getLastSolutionPlanCost(), -1);
    ASSERT_EQ(engine.getStandardEngineStatistics().m_num_states_generated, 23);

    limits.m_state_generation_limit = 0;
    limits.m_time_limit_seconds = 0.000001;
    limits.m_timer_check_frequency = 1;
    engine.setResourceLimits(limits);

    engine.searchForPlan(init_state);
    ASSERT_FALSE(engine.hasFoundSolution());
    ASSERT_EQ(engine.getStatus(), EngineStatus::resource_limit_hit);
    ASSERT_EQ(engine.getLastSolutionPlanCost(), -1);
    ASSERT_TRUE(engine.getStandardEngineStatistics().m_search_time_seconds >= limits.m_time_limit_seconds);
}

/**
 * Ensures that the engine handles parent pruning and edge costs correctly.
 */
TEST(IDEngineGraphTests, parentPruningNonUniformEdgeCostTests) {
    std::stringstream csv = std::stringstream("a;b 2;c 7\nb;a 4;d 4");
    Graph graph = getGraphFromCSVAdjacencyList(csv);
    GraphTransitions transitions(graph);

    IDEngineParams params;
    params.m_use_parent_pruning = false;
    IDEngine<GraphState, GraphAction> engine(params);

    SingleStateGoalTest<GraphState> goal_test(transitions.getVertexState("d"));
    ConstantHeuristic<GraphState, GraphAction> zero_heuristic;
    FCostEvaluator<GraphState, GraphAction> f_cost_evaluator(zero_heuristic);
    GraphState init_state = transitions.getVertexState("a");

    engine.setEvaluator(f_cost_evaluator);
    engine.setTransitionSystem(transitions);
    engine.setGoalTest(goal_test);

    engine.initializeSearch(init_state);

    // Start iteration 0 with threshold 0
    engine.singleSearchStep();
    ASSERT_EQ(engine.getNodes().getState(engine.getNodes().size() - 1), transitions.getVertexState("b"));
    ASSERT_EQ(engine.getNodes().getGValue(engine.getNodes().size() - 1), 2);
    ASSERT_EQ(f_cost_evaluator.getCachedEval(engine.getNodes().size() - 1), 2);
    ASSERT_EQ(engine.getNextThreshold(), -1);

    engine.singleSearchStep();
    ASSERT_EQ(engine.getNodes().getState(engine.getNodes().size() - 1), transitions.getVertexState("c"));
    ASSERT_EQ(engine.getNodes().getGValue(engine.getNodes().size() - 1), 7);
    ASSERT_EQ(f_cost_evaluator.getCachedEval(engine.getNodes().size() - 1), 7);
    ASSERT_EQ(engine.getNextThreshold(), 2);

    engine.singleSearchStep();

    // Start iteration 1 with threshold 2
    ASSERT_EQ(engine.getNodes().getState(engine.getNodes().size() - 1), transitions.getVertexState("a"));
    ASSERT_EQ(engine.getThresholds().back(), 2);

    engine.singleSearchStep();
    ASSERT_EQ(engine.getNodes().getState(engine.getNodes().size() - 1), transitions.getVertexState("b"));

    engine.singleSearchStep();
    ASSERT_EQ(engine.getNodes().getState(engine.getNodes().size() - 1), transitions.getVertexState("a"));
    ASSERT_EQ(engine.getNodes().getGValue(engine.getNodes().size() - 1), 6);
    ASSERT_EQ(f_cost_evaluator.getCachedEval(engine.getNodes().size() - 1), 6);

    engine.singleSearchStep();
    ASSERT_EQ(engine.getNodes().getState(engine.getNodes().size() - 1), transitions.getVertexState("d"));
    engine.singleSearchStep();
    ASSERT_EQ(engine.getNodes().getState(engine.getNodes().size() - 1), transitions.getVertexState("c"));
    engine.singleSearchStep();

    // Start iteration 2 with threshold 6
    ASSERT_EQ(engine.getNodes().getState(engine.getNodes().size() - 1), transitions.getVertexState("a"));
    ASSERT_EQ(engine.getThresholds().back(), 6);

    engine.singleSearchStep();
    ASSERT_EQ(engine.getNodes().getState(engine.getNodes().size() - 1), transitions.getVertexState("b"));
    engine.singleSearchStep();
    ASSERT_EQ(engine.getNodes().getState(engine.getNodes().size() - 1), transitions.getVertexState("a"));
    engine.singleSearchStep();
    ASSERT_EQ(engine.getNodes().getState(engine.getNodes().size() - 1), transitions.getVertexState("b"));
    engine.singleSearchStep();
    ASSERT_EQ(engine.getNodes().getState(engine.getNodes().size() - 1), transitions.getVertexState("c"));
    engine.singleSearchStep();
    ASSERT_EQ(engine.getNodes().getState(engine.getNodes().size() - 1), transitions.getVertexState("d"));
    engine.singleSearchStep();

    ASSERT_TRUE(engine.hasFoundSolution());
    ASSERT_EQ(engine.getStatus(), EngineStatus::search_completed);
    ASSERT_EQ(engine.getLastSolutionPlanCost(), 6);

    // Now try with parent pruning
    params.m_use_parent_pruning = true;
    engine.setEngineParams(params);
    engine.reset();

    // Start iteration 0
    engine.initializeSearch(init_state);

    engine.singleSearchStep();
    ASSERT_EQ(engine.getNodes().getState(engine.getNodes().size() - 1), transitions.getVertexState("b"));
    engine.singleSearchStep();

    ASSERT_EQ(engine.getNodes().getState(engine.getNodes().size() - 1), transitions.getVertexState("c"));
    engine.singleSearchStep();

    // Start iteration 1
    ASSERT_EQ(engine.getNodes().getState(engine.getNodes().size() - 1), transitions.getVertexState("a"));
    engine.singleSearchStep();
    ASSERT_EQ(engine.getNodes().getState(engine.getNodes().size() - 1), transitions.getVertexState("b"));
    engine.singleSearchStep();
    ASSERT_EQ(engine.getNodes().getState(engine.getNodes().size() - 1), transitions.getVertexState("d"));
    engine.singleSearchStep();
    ASSERT_EQ(engine.getNodes().getState(engine.getNodes().size() - 1), transitions.getVertexState("c"));
    engine.singleSearchStep();

    // Start iteration 2
    ASSERT_EQ(engine.getNodes().getState(engine.getNodes().size() - 1), transitions.getVertexState("a"));
    engine.singleSearchStep();
    ASSERT_EQ(engine.getNodes().getState(engine.getNodes().size() - 1), transitions.getVertexState("b"));
    engine.singleSearchStep();
    ASSERT_EQ(engine.getNodes().getState(engine.getNodes().size() - 1), transitions.getVertexState("d"));
    engine.singleSearchStep();

    ASSERT_TRUE(engine.hasFoundSolution());
    ASSERT_EQ(engine.getStatus(), EngineStatus::search_completed);
    ASSERT_EQ(engine.getLastSolutionPlanCost(), 6);
}

/**
 * Tests with an actual heuristic to make sure things are calculated correctly.
 */
TEST(IDEngineGraphTests, problemWithHeuristicTest) {
    std::stringstream csv = std::stringstream("a;b;c 2;d\nb;e;f\nc;g;h\ng;goal");
    Graph graph = getGraphFromCSVAdjacencyList(csv);
    GraphTransitions transitions(graph);

    IDEngineParams params;
    IDEngine<GraphState, GraphAction> engine = IDEngine<GraphState, GraphAction>(params);

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

    // Start iteration 0
    engine.initializeSearch(init_state);

    ASSERT_EQ(engine.getNodes().getGValue(0), 0.0);
    ASSERT_EQ(heuristic.getCachedEval(0), 2.0);
    ASSERT_FALSE(f_cost_evaluator.getCachedIsDeadEnd(0));
    ASSERT_EQ(f_cost_evaluator.getCachedEval(0), 2.0);
    ASSERT_EQ(engine.getThresholds().back(), 2.0);

    engine.singleSearchStep();
    ASSERT_EQ(engine.getNodes().getState(engine.getNodes().size() - 1), transitions.getVertexState("b"));
    ASSERT_TRUE(f_cost_evaluator.getCachedIsDeadEnd(engine.getNodes().size() - 1));
    engine.singleSearchStep();
    ASSERT_EQ(engine.getNodes().getState(engine.getNodes().size() - 1), transitions.getVertexState("c"));
    ASSERT_EQ(f_cost_evaluator.getCachedEval(engine.getNodes().size() - 1), 3.0);
    engine.singleSearchStep();
    ASSERT_EQ(engine.getNodes().getState(engine.getNodes().size() - 1), transitions.getVertexState("d"));
    ASSERT_TRUE(f_cost_evaluator.getCachedIsDeadEnd(engine.getNodes().size() - 1));
    engine.singleSearchStep();

    // Start iteration 1
    ASSERT_EQ(engine.getNodes().getState(engine.getNodes().size() - 1), transitions.getVertexState("a"));
    ASSERT_EQ(engine.getThresholds().back(), 3.0);
    engine.singleSearchStep();
    ASSERT_EQ(engine.getNodes().getState(engine.getNodes().size() - 1), transitions.getVertexState("b"));
    engine.singleSearchStep();
    ASSERT_EQ(engine.getNodes().getState(engine.getNodes().size() - 1), transitions.getVertexState("c"));
    engine.singleSearchStep();
    ASSERT_EQ(engine.getNodes().getState(engine.getNodes().size() - 1), transitions.getVertexState("g"));
    engine.singleSearchStep();
    ASSERT_EQ(engine.getNodes().getState(engine.getNodes().size() - 1), transitions.getVertexState("h"));
    ASSERT_FALSE(f_cost_evaluator.getCachedIsDeadEnd(engine.getNodes().size() - 1));
    ASSERT_EQ(heuristic.getCachedEval(engine.getNodes().size() - 1), 2);
    ASSERT_EQ(f_cost_evaluator.getCachedEval(engine.getNodes().size() - 1), 5);
    engine.singleSearchStep();
    ASSERT_EQ(engine.getNodes().getState(engine.getNodes().size() - 1), transitions.getVertexState("d"));
    engine.singleSearchStep();

    // Start iteration 2
    ASSERT_EQ(engine.getNodes().getState(engine.getNodes().size() - 1), transitions.getVertexState("a"));
    ASSERT_EQ(engine.getThresholds().back(), 4.0);
    engine.singleSearchStep();
    ASSERT_EQ(engine.getNodes().getState(engine.getNodes().size() - 1), transitions.getVertexState("b"));
    engine.singleSearchStep();
    ASSERT_EQ(engine.getNodes().getState(engine.getNodes().size() - 1), transitions.getVertexState("c"));
    engine.singleSearchStep();
    ASSERT_EQ(engine.getNodes().getState(engine.getNodes().size() - 1), transitions.getVertexState("g"));
    engine.singleSearchStep();
    ASSERT_EQ(engine.getNodes().getState(engine.getNodes().size() - 1), transitions.getVertexState("goal"));
    engine.singleSearchStep();

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
* Tests that get settings works as intended
*/
TEST(IDEngineGraphTests, getSettingsTest) {
    using namespace evalFunctionTerms;

    std::stringstream csv = std::stringstream("a;b;c 2;d\nb;e;f\nc;g;h\ng;goal");
    Graph graph = getGraphFromCSVAdjacencyList(csv);
    GraphTransitions transitions(graph);

    IDEngineParams params;
    IDEngine<GraphState, GraphAction> engine = IDEngine<GraphState, GraphAction>(params);

    SingleStateGoalTest<GraphState> goal_test(transitions.getVertexState("goal"));

    StateStringHashFunction<GraphState> hasher;
    HashMapHeuristic<GraphState, GraphAction, std::string> heuristic(hasher);

    FCostEvaluator<GraphState, GraphAction> f_cost_evaluator(heuristic);

    engine.setEvaluator(f_cost_evaluator);
    engine.setTransitionSystem(transitions);
    engine.setGoalTest(goal_test);

    auto settings = engine.getAllSettings();
    ASSERT_EQ(settings.m_name, "IDEngine");
    auto& log = settings.m_main_settings;
    ASSERT_EQ(log.size(), 4);
    ASSERT_EQ(log["use_parent_pruning"], "true");
    ASSERT_EQ(log["use_random_op_ordering"], "false");
    ASSERT_EQ(log["use_stored_seed"], "false");
    ASSERT_TRUE(log.find("random_seed") != log.end());

    ASSERT_EQ(settings.m_sub_component_settings.size(), 1);

    auto& evaluator_settings = settings.m_sub_component_settings.at("eval_function");
    ASSERT_EQ(evaluator_settings.m_name, f_cost_evaluator.CLASS_NAME);
    ASSERT_EQ(evaluator_settings.m_main_settings.size(), 0);

    ASSERT_EQ(evaluator_settings.m_sub_component_settings.size(), 1);
    auto& heuristic_settings = evaluator_settings.m_sub_component_settings.at(SETTING_HEURISTIC);
    ASSERT_EQ(heuristic_settings.m_name, heuristic.CLASS_NAME);
}