#include "building_tools/evaluators/hash_map_heuristic.h"
#include "building_tools/goal_tests/single_state_goal_test.h"
#include "building_tools/hashing/permutation_hash_function.h"
#include "building_tools/hashing/state_string_hash_function.h"
#include "engines/best_first_search/a_star_epsilon.h"
#include "environments/graph/graph_transitions.h"
#include "environments/graph/graph_utils.h"
#include "environments/graph/vertex_hash_function.h"
#include "environments/sliding_tile_puzzle/sliding_tile_manhattan_heuristic.h"
#include "environments/sliding_tile_puzzle/sliding_tile_transitions.h"
#include "gtest/gtest.h"

/** 
* Creates a fixture for A Star Epsilon tests. Uses sliding tile environment and manhattan distance heuristic.
*/
class AStarEpsilonSlidingTileTest : public ::testing::Test {
public:
    AStarEpsilonParams params;

    std::vector<Tile> init_perm{3, 5, 1, 4, 0, 2};
    SlidingTileState init_state = SlidingTileState(init_perm, 2, 3);

    std::vector<Tile> goal_perm{0, 1, 2, 3, 4, 5};
    SlidingTileState goal_state = SlidingTileState(goal_perm, 2, 3);
    SingleStateGoalTest<SlidingTileState> goal_test = SingleStateGoalTest<SlidingTileState>(goal_state);

    SlidingTileTransitions transitions = SlidingTileTransitions(2, 3, SlidingTileCostType::unit);
    SlidingTileManhattanHeuristic heuristic = SlidingTileManhattanHeuristic(goal_state, SlidingTileCostType::unit);
    PermutationHashFunction<SlidingTileState> hash_function;

    /**
    * Method to test the contents of the focal list. Ensures the node at the top of the heap is the best node.
    */
    bool checkFocalList(AStarEpsilon<SlidingTileState, BlankSlide, uint64_t>* current_engine) {

        double fmin = current_engine->getMaxEval();
        if (current_engine->getFocalList().isEmpty()) {
            return true;
        }
        const auto& focal = current_engine->getFocalList();
        NodeID best_node_id = focal.getIDOfBestNode();
        const FCostEvaluator<SlidingTileState, BlankSlide>& f_cost_evaluator = current_engine->getFCostEvaluator();

        for (int i = 0; i < static_cast<int>(focal.getSize()); i++) {
            NodeID node_id = focal.getHeapEntry(i);
            if (f_cost_evaluator.getCachedEval(node_id) > fmin) {
                return false;
            }
            if (heuristic.getCachedEval(best_node_id) > heuristic.getCachedEval(node_id)) {
                return false;
            }
            if (heuristic.getCachedEval(best_node_id) == heuristic.getCachedEval(node_id) &&
                      f_cost_evaluator.getCachedEval(best_node_id) > f_cost_evaluator.getCachedEval(node_id)) {
                return false;
            }
        }
        return true;
    }
};

/**
 * Tests that the constructor sets everything as expected.
 */
TEST_F(AStarEpsilonSlidingTileTest, constructorTest) {
    AStarEpsilon<SlidingTileState, BlankSlide, uint64_t> engine(params);

    ASSERT_FALSE(engine.canRunSearch());
    ASSERT_EQ(engine.getMaxEval(), 0);
    ASSERT_TRUE(engine.getFocalList().isEmpty());
    ASSERT_TRUE(engine.getNotInFocalList().isEmpty());
    ASSERT_TRUE(engine.getOpenList().isEmpty());
    ASSERT_EQ(engine.getStatus(), EngineStatus::not_ready);

    StandardSearchStatistics stats = engine.getStandardEngineStatistics();
    ASSERT_EQ(stats.m_num_goal_tests, 0);

    ASSERT_FALSE(engine.hasFoundSolution());
    ASSERT_TRUE(engine.getLastSolutionPlan().empty());
    ASSERT_EQ(engine.getLastSolutionPlanCost(), -1.0);
}

/** 
* Checks that the engine can run properly once all necessary parts have been set
*/
TEST_F(AStarEpsilonSlidingTileTest, setAndCanRunTest) {
    AStarEpsilon<SlidingTileState, BlankSlide, uint64_t> engine(params);

    engine.setHeuristic(heuristic);
    ASSERT_FALSE(engine.canRunSearch());
    ASSERT_EQ(engine.getStatus(), EngineStatus::not_ready);

    engine.setTransitionSystem(transitions);
    ASSERT_FALSE(engine.canRunSearch());
    ASSERT_EQ(engine.getStatus(), EngineStatus::not_ready);

    engine.setGoalTest(goal_test);
    ASSERT_FALSE(engine.canRunSearch());
    ASSERT_EQ(engine.getStatus(), EngineStatus::not_ready);

    engine.setHashFunction(hash_function);
    ASSERT_TRUE(engine.canRunSearch());
    ASSERT_EQ(engine.getStatus(), EngineStatus::ready);
}

/** 
* Checks that initialize search sets things up correctly.
*/
TEST_F(AStarEpsilonSlidingTileTest, initializeSearchTest) {
    AStarEpsilon<SlidingTileState, BlankSlide, uint64_t> engine(params);

    engine.setHeuristic(heuristic);
    engine.setTransitionSystem(transitions);
    engine.setGoalTest(goal_test);
    engine.setHashFunction(hash_function);

    engine.initializeSearch(init_state);
    ASSERT_EQ(engine.getStatus(), EngineStatus::active);
    ASSERT_EQ(engine.getMaxEval(), 6.0);
    ASSERT_EQ(engine.getOpenListSize(), 1);

    const auto& nodes = engine.getNodes();
    ASSERT_EQ(nodes.getState(0), init_state);
    ASSERT_EQ(nodes.getLastAction(0), std::nullopt);
    ASSERT_EQ(nodes.getGValue(0), 0.0);
    ASSERT_EQ(heuristic.getCachedEval(0), 6.0);
    ASSERT_FALSE(heuristic.getCachedIsDeadEnd(0));
    ASSERT_EQ(engine.getFCostEvaluator().getCachedEval(0), 6.0);
}

/**
*  Runs step by step through a search to make sure heaps contains the correct items after each step.
*/
TEST_F(AStarEpsilonSlidingTileTest, singleSearchStepTest) {
    AStarEpsilon<SlidingTileState, BlankSlide, uint64_t> engine(params);

    engine.setHeuristic(heuristic);
    engine.setTransitionSystem(transitions);
    engine.setGoalTest(goal_test);
    engine.setHashFunction(hash_function);
    params.m_store_expansion_order = true;
    engine.setEngineParams(params);

    engine.initializeSearch(init_state);
    ASSERT_EQ(engine.getMaxEval(), 6.0);
    ASSERT_EQ(engine.getFocalList().getSize(), 0);
    ASSERT_TRUE(checkFocalList(&engine));
    ASSERT_EQ(engine.getNotInFocalList().getSize(), 1);
    ASSERT_EQ(engine.getOpenListSize(), 1);
    ASSERT_EQ(engine.getClosedListSize(), 0);
    ASSERT_EQ(engine.getExpansionOrder().size(), 0);

    EngineStatus status = engine.singleSearchStep();
    ASSERT_EQ(engine.getMaxEval(), 6.0);
    ASSERT_EQ(engine.getFocalList().getSize(), 0);
    ASSERT_TRUE(checkFocalList(&engine));
    ASSERT_EQ(engine.getNotInFocalList().getSize(), 3);
    ASSERT_EQ(engine.getOpenListSize(), 3);
    ASSERT_EQ(engine.getClosedListSize(), 1);
    ASSERT_EQ(engine.getExpansionOrder().size(), 1);
    ASSERT_EQ(engine.getLastExpandedNodeID(), 0);
    ASSERT_EQ(status, EngineStatus::active);
    ASSERT_EQ(engine.getExpansionOrder().size(), 1);

    status = engine.singleSearchStep();
    ASSERT_EQ(engine.getMaxEval(), 6.0);
    ASSERT_EQ(engine.getFocalList().getSize(), 1);
    ASSERT_TRUE(checkFocalList(&engine));
    ASSERT_EQ(engine.getNotInFocalList().getSize(), 3);
    ASSERT_EQ(engine.getOpenListSize(), 4);
    ASSERT_EQ(engine.getClosedListSize(), 2);
    ASSERT_EQ(engine.getExpansionOrder().size(), 2);
    ASSERT_EQ(engine.getLastExpandedNodeID(), 1);
    ASSERT_EQ(status, EngineStatus::active);
    ASSERT_EQ(engine.getExpansionOrder().size(), 2);

    status = engine.singleSearchStep();
    ASSERT_EQ(engine.getMaxEval(), 6.0);
    ASSERT_EQ(engine.getFocalList().getSize(), 1);
    ASSERT_TRUE(checkFocalList(&engine));
    ASSERT_EQ(engine.getNotInFocalList().getSize(), 3);
    ASSERT_EQ(engine.getOpenListSize(), 4);
    ASSERT_EQ(engine.getClosedListSize(), 3);
    ASSERT_EQ(engine.getExpansionOrder().size(), 3);
    ASSERT_EQ(engine.getLastExpandedNodeID(), 4);
    ASSERT_EQ(status, EngineStatus::active);
    ASSERT_EQ(engine.getExpansionOrder().size(), 3);

    status = engine.singleSearchStep();
    ASSERT_EQ(engine.getMaxEval(), 6.0);
    ASSERT_EQ(engine.getFocalList().getSize(), 1);
    ASSERT_TRUE(checkFocalList(&engine));
    ASSERT_EQ(engine.getNotInFocalList().getSize(), 3);
    ASSERT_EQ(engine.getOpenListSize(), 4);
    ASSERT_EQ(engine.getClosedListSize(), 4);
    ASSERT_EQ(engine.getExpansionOrder().size(), 4);
    ASSERT_EQ(engine.getLastExpandedNodeID(), 6);
    ASSERT_EQ(status, EngineStatus::active);
    ASSERT_EQ(engine.getExpansionOrder().size(), 4);

    status = engine.singleSearchStep();
    ASSERT_EQ(engine.getMaxEval(), 6.0);
    ASSERT_EQ(engine.getFocalList().getSize(), 1);
    ASSERT_TRUE(checkFocalList(&engine));
    ASSERT_EQ(engine.getNotInFocalList().getSize(), 4);
    ASSERT_EQ(engine.getOpenListSize(), 5);
    ASSERT_EQ(engine.getClosedListSize(), 5);
    ASSERT_EQ(engine.getExpansionOrder().size(), 5);
    ASSERT_EQ(engine.getLastExpandedNodeID(), 7);
    ASSERT_EQ(status, EngineStatus::active);
    ASSERT_EQ(engine.getExpansionOrder().size(), 5);

    status = engine.singleSearchStep();
    ASSERT_EQ(engine.getMaxEval(), 6.0);
    ASSERT_EQ(engine.getFocalList().getSize(), 1);
    ASSERT_TRUE(checkFocalList(&engine));
    ASSERT_EQ(engine.getNotInFocalList().getSize(), 4);
    ASSERT_EQ(engine.getOpenListSize(), 5);
    ASSERT_EQ(engine.getClosedListSize(), 6);
    ASSERT_EQ(engine.getExpansionOrder().size(), 6);
    ASSERT_EQ(engine.getLastExpandedNodeID(), 9);
    ASSERT_EQ(status, EngineStatus::active);
    ASSERT_EQ(engine.getExpansionOrder().size(), 6);

    status = engine.singleSearchStep();
    ASSERT_EQ(engine.getMaxEval(), 6.0);
    ASSERT_EQ(engine.getFocalList().getSize(), 2);
    ASSERT_TRUE(checkFocalList(&engine));
    ASSERT_EQ(engine.getNotInFocalList().getSize(), 3);
    ASSERT_EQ(engine.getOpenListSize(), 5);
    ASSERT_EQ(engine.getClosedListSize(), 6);
    ASSERT_EQ(engine.getExpansionOrder().size(), 7);
    ASSERT_EQ(engine.getLastExpandedNodeID(), 10);
    ASSERT_EQ(status, EngineStatus::search_completed);
    ASSERT_EQ(engine.getExpansionOrder().size(), 7);

    ASSERT_TRUE(engine.hasFoundSolution());
    ASSERT_EQ(engine.getLastSolutionPlanCost(), 6);
    auto plan = engine.getLastSolutionPlan();
    ASSERT_EQ(vectorToString(plan), "[up right down left left up]");
}

/**
 * Checks that the statistics are being calculated correctly.
 */
TEST_F(AStarEpsilonSlidingTileTest, getSearchStatisticsTest) {
    AStarEpsilon<SlidingTileState, BlankSlide, uint64_t> engine(params);

    engine.setHeuristic(heuristic);
    engine.setTransitionSystem(transitions);
    engine.setGoalTest(goal_test);
    engine.setHashFunction(hash_function);

    engine.searchForPlan(init_state);

    auto stats = engine.getStandardEngineStatistics();

    ASSERT_EQ(stats.m_num_actions_generated, 15);
    ASSERT_EQ(stats.m_num_get_actions_calls, 6);
    ASSERT_EQ(stats.m_num_goal_tests, 7);
    ASSERT_EQ(stats.m_num_states_generated, 15);
    ASSERT_EQ(stats.m_num_evals, 11);
}

/**
 * Checks that resource limits are applied correctly.
 */
TEST_F(AStarEpsilonSlidingTileTest, resourceLimitsTest) {
    AStarEpsilon<SlidingTileState, BlankSlide, uint64_t> engine(params);

    engine.setHeuristic(heuristic);
    engine.setTransitionSystem(transitions);
    engine.setGoalTest(goal_test);
    engine.setHashFunction(hash_function);

    SearchResourceLimits limits;
    limits.m_get_actions_call_limit = 3;
    engine.setResourceLimits(limits);

    engine.searchForPlan(init_state);
    ASSERT_FALSE(engine.hasFoundSolution());
    ASSERT_EQ(engine.getStatus(), EngineStatus::resource_limit_hit);
    ASSERT_EQ(engine.getLastSolutionPlanCost(), -1);
    ASSERT_EQ(engine.getStandardEngineStatistics().m_num_get_actions_calls, 3);

    limits.m_get_actions_call_limit = 0;
    limits.m_goal_test_limit = 5;
    engine.setResourceLimits(limits);

    engine.searchForPlan(init_state);
    ASSERT_FALSE(engine.hasFoundSolution());
    ASSERT_EQ(engine.getStatus(), EngineStatus::resource_limit_hit);
    ASSERT_EQ(engine.getLastSolutionPlanCost(), -1);
    ASSERT_EQ(engine.getStandardEngineStatistics().m_num_goal_tests, 5);

    limits.m_goal_test_limit = 0;
    limits.m_node_eval_limit = 5;
    engine.setResourceLimits(limits);

    engine.searchForPlan(init_state);
    ASSERT_FALSE(engine.hasFoundSolution());
    ASSERT_EQ(engine.getStatus(), EngineStatus::resource_limit_hit);
    ASSERT_EQ(engine.getLastSolutionPlanCost(), -1);
    ASSERT_EQ(engine.getStandardEngineStatistics().m_num_evals, 5);

    limits.m_node_eval_limit = 0;
    limits.m_state_generation_limit = 5;
    engine.setResourceLimits(limits);

    engine.searchForPlan(init_state);
    ASSERT_FALSE(engine.hasFoundSolution());
    ASSERT_EQ(engine.getStatus(), EngineStatus::resource_limit_hit);
    ASSERT_EQ(engine.getLastSolutionPlanCost(), -1);
    ASSERT_EQ(engine.getStandardEngineStatistics().m_num_states_generated, 5);

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
* Runs step by step through a search to make sure heaps contains the correct items after each step.
* Checks using a weight that increases f_min by a factor of 2.
*/
TEST_F(AStarEpsilonSlidingTileTest, weightedSingleSearchStepTest) {
    params.m_weight = 2;
    AStarEpsilon<SlidingTileState, BlankSlide, uint64_t> engine(params);

    engine.setHeuristic(heuristic);
    engine.setTransitionSystem(transitions);
    engine.setGoalTest(goal_test);
    engine.setHashFunction(hash_function);

    engine.initializeSearch(init_state);
    ASSERT_EQ(engine.getMaxEval(), 12.0);
    ASSERT_EQ(engine.getFocalList().getSize(), 0);
    ASSERT_TRUE(checkFocalList(&engine));
    ASSERT_EQ(engine.getNotInFocalList().getSize(), 1);
    ASSERT_EQ(engine.getOpenListSize(), 1);
    ASSERT_EQ(engine.getClosedListSize(), 0);

    EngineStatus status = engine.singleSearchStep();
    ASSERT_EQ(engine.getMaxEval(), 12.0);
    ASSERT_EQ(engine.getFocalList().getSize(), 0);
    ASSERT_TRUE(checkFocalList(&engine));
    ASSERT_EQ(engine.getNotInFocalList().getSize(), 3);
    ASSERT_EQ(engine.getOpenListSize(), 3);
    ASSERT_EQ(engine.getClosedListSize(), 1);
    ASSERT_EQ(status, EngineStatus::active);

    status = engine.singleSearchStep();
    ASSERT_EQ(engine.getMaxEval(), 12.0);
    ASSERT_EQ(engine.getFocalList().getSize(), 2);
    ASSERT_TRUE(checkFocalList(&engine));
    ASSERT_EQ(engine.getNotInFocalList().getSize(), 2);
    ASSERT_EQ(engine.getOpenListSize(), 4);
    ASSERT_EQ(engine.getClosedListSize(), 2);
    ASSERT_EQ(status, EngineStatus::active);

    status = engine.singleSearchStep();
    ASSERT_EQ(engine.getMaxEval(), 12.0);
    ASSERT_EQ(engine.getFocalList().getSize(), 3);
    ASSERT_TRUE(checkFocalList(&engine));
    ASSERT_EQ(engine.getNotInFocalList().getSize(), 1);
    ASSERT_EQ(engine.getOpenListSize(), 4);
    ASSERT_EQ(engine.getClosedListSize(), 3);
    ASSERT_EQ(status, EngineStatus::active);

    status = engine.singleSearchStep();
    ASSERT_EQ(engine.getMaxEval(), 12.0);
    ASSERT_EQ(engine.getFocalList().getSize(), 3);
    ASSERT_TRUE(checkFocalList(&engine));
    ASSERT_EQ(engine.getNotInFocalList().getSize(), 1);
    ASSERT_EQ(engine.getOpenListSize(), 4);
    ASSERT_EQ(engine.getClosedListSize(), 4);
    ASSERT_EQ(status, EngineStatus::active);

    status = engine.singleSearchStep();
    ASSERT_EQ(engine.getMaxEval(), 12.0);
    ASSERT_EQ(engine.getFocalList().getSize(), 3);
    ASSERT_TRUE(checkFocalList(&engine));
    ASSERT_EQ(engine.getNotInFocalList().getSize(), 2);
    ASSERT_EQ(engine.getOpenListSize(), 5);
    ASSERT_EQ(engine.getClosedListSize(), 5);
    ASSERT_EQ(status, EngineStatus::active);

    status = engine.singleSearchStep();
    ASSERT_EQ(engine.getMaxEval(), 12.0);
    ASSERT_EQ(engine.getFocalList().getSize(), 4);
    ASSERT_TRUE(checkFocalList(&engine));
    ASSERT_EQ(engine.getNotInFocalList().getSize(), 1);
    ASSERT_EQ(engine.getOpenListSize(), 5);
    ASSERT_EQ(engine.getClosedListSize(), 6);
    ASSERT_EQ(status, EngineStatus::active);

    status = engine.singleSearchStep();
    ASSERT_EQ(engine.getMaxEval(), 12.0);
    ASSERT_EQ(engine.getFocalList().getSize(), 5);
    ASSERT_TRUE(checkFocalList(&engine));
    ASSERT_EQ(engine.getNotInFocalList().getSize(), 0);
    ASSERT_EQ(engine.getOpenListSize(), 5);
    ASSERT_EQ(engine.getClosedListSize(), 6);
    ASSERT_EQ(status, EngineStatus::search_completed);

    ASSERT_TRUE(engine.hasFoundSolution());
    ASSERT_EQ(engine.getLastSolutionPlanCost(), 6);
    auto plan = engine.getLastSolutionPlan();
    ASSERT_EQ(vectorToString(plan), "[up right down left left up]");
}

/** 
* Test behavior when there is no solution
*/
TEST(AStarEpsilonAlgorithmTest, noSolutionTest) {
    AStarEpsilonParams params;
    AStarEpsilon<SlidingTileState, BlankSlide, uint64_t> engine(params);

    std::vector<Tile> init_perm{0, 1, 3, 2};
    SlidingTileState init_state = SlidingTileState(init_perm, 2, 2);

    std::vector<Tile> goal_perm{0, 1, 2, 3};
    SlidingTileState goal_state = SlidingTileState(goal_perm, 2, 2);
    SingleStateGoalTest<SlidingTileState> goal_test = SingleStateGoalTest<SlidingTileState>(goal_state);

    SlidingTileTransitions transitions = SlidingTileTransitions(2, 2, SlidingTileCostType::unit);
    SlidingTileManhattanHeuristic heuristic = SlidingTileManhattanHeuristic(goal_state, SlidingTileCostType::unit);
    PermutationHashFunction<SlidingTileState> hash_function;

    engine.setHeuristic(heuristic);
    engine.setTransitionSystem(transitions);
    engine.setGoalTest(goal_test);
    engine.setHashFunction(hash_function);

    engine.searchForPlan(init_state);
    ASSERT_FALSE(engine.hasFoundSolution());
    ASSERT_EQ(engine.getStatus(), EngineStatus::search_completed);
    ASSERT_EQ(engine.getLastSolutionPlanCost(), -1);
}

/**
* Checks that the algorithm can handle pre-existing nodes and can reopen them.
*/
TEST(AStarEpsilonGraphTest, nodeReopenedTest) {
    std::stringstream csv = std::stringstream("a;b 5;c 1\nb;d 1\nc;b 1\nd;goal 4");
    Graph graph = getGraphFromCSVAdjacencyList(csv);
    GraphTransitions transitions(graph);

    AStarEpsilonParams params;
    AStarEpsilon<GraphState, GraphAction, std::string> engine(params);

    SingleStateGoalTest<GraphState> goal_test(transitions.getVertexState("goal"));
    StateStringHashFunction<GraphState> hasher;

    HashMapHeuristic<GraphState, GraphAction, std::string> heuristic(hasher);
    heuristic.addHeuristicValue(transitions.getVertexState("a"), 7, false);
    heuristic.addHeuristicValue(transitions.getVertexState("b"), 1, false);
    heuristic.addHeuristicValue(transitions.getVertexState("c"), 6, false);
    heuristic.addHeuristicValue(transitions.getVertexState("d"), 4, false);
    heuristic.addHeuristicValue(transitions.getVertexState("goal"), 0, false);

    GraphState init_state = transitions.getVertexState("a");

    engine.setHeuristic(heuristic);
    engine.setTransitionSystem(transitions);
    engine.setGoalTest(goal_test);
    engine.setHashFunction(hasher);

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
    ASSERT_EQ(stats.m_num_get_actions_calls, 5);

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
    ASSERT_EQ(stats.m_num_get_actions_calls, 4);
}

// TODO Re-enable parent updating tests
///**
//* Checks that parent updating works properly
//*/
//TEST(AStarEpsilonGraphTest, parentUpdatingTest) {
//    std::stringstream csv = std::stringstream("a;b 2;c 2;d 2\nc;goal 5");
//    Graph graph = getGraphFromCSVAdjacencyList(csv);
//    GraphTransitions transitions(graph);
//
//    AStarEpsilonParams params;
//    params.m_parent_heuristic_updating = false;
//    AStarEpsilon<GraphState, GraphAction, std::string> engine(params);
//
//    SingleGoalTest<GraphState> goal_test(transitions.getVertexState("goal"));
//    StateStringHashFunction<GraphState> hasher;
//
//    HashMapHeuristic<GraphState, GraphAction, std::string> heuristic(&hasher);
//    heuristic.addHeuristicValue(transitions.getVertexState("a"), 1, false);
//    heuristic.addHeuristicValue(transitions.getVertexState("b"), 4, false);
//    heuristic.addHeuristicValue(transitions.getVertexState("c"), 5, false);
//    heuristic.addHeuristicValue(transitions.getVertexState("d"), 6, false);
//    heuristic.addHeuristicValue(transitions.getVertexState("goal"), 0, false);
//
//    GraphState init_state = transitions.getVertexState("a");
//
//    engine.setHeuristic(heuristic);
//    engine.setTransitionSystem(transitions);
//    engine.setGoalTest(goal_test);
//    engine.setHashFunction(&hasher);
//    engine.searchForPlan(init_state);
//    ASSERT_EQ(engine.getNodeTable().getNode(0).m_evals[0], 1);
//
//    params.m_parent_heuristic_updating = true;
//    engine.setEngineParams(params);
//    engine.searchForPlan(init_state);
//    ASSERT_EQ(engine.getNodeTable().getNode(0).m_evals[0], 6);
//
//    HashMapHeuristic<GraphState, GraphAction, std::string> second_heuristic(&hasher);
//    second_heuristic.addHeuristicValue(transitions.getVertexState("a"), 8, false);
//    second_heuristic.addHeuristicValue(transitions.getVertexState("b"), 4, false);
//    second_heuristic.addHeuristicValue(transitions.getVertexState("c"), 5, false);
//    second_heuristic.addHeuristicValue(transitions.getVertexState("d"), 6, false);
//    second_heuristic.addHeuristicValue(transitions.getVertexState("goal"), 0, false);
//
//    engine.setHeuristic(second_heuristic);
//    params.m_parent_heuristic_updating = false;
//    engine.setEngineParams(params);
//    engine.searchForPlan(init_state);
//    ASSERT_EQ(engine.getNodeTable().getNode(0).m_evals[0], 8);
//
//    params.m_parent_heuristic_updating = true;
//    engine.setEngineParams(params);
//    engine.searchForPlan(init_state);
//    ASSERT_EQ(engine.getNodeTable().getNode(0).m_evals[0], 8);
//}

/**
* Tests that get settings works as intended
*/
TEST(AStarEpsilonGraphTest, getSettingsTest) {
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

    auto settings = engine.getAllSettings();
    ASSERT_EQ(settings.m_name, "AStarEpsilon");
    auto& log = settings.m_main_settings;
    ASSERT_EQ(log.size(), 6);
    ASSERT_EQ(log["use_stored_seed"], "false");
    ASSERT_TRUE(log.find("random_seed") != log.end());
    ASSERT_EQ(log["weight"], "1.0");
    ASSERT_EQ(log["use_reopened"], "true");
    ASSERT_EQ(log["parent_heuristic_updating"], "false");
    ASSERT_EQ(log["store_expansion_order"], "false");

    auto& subcomponent_settings = settings.m_sub_component_settings;
    ASSERT_EQ(subcomponent_settings.size(), 2);

    ASSERT_EQ(subcomponent_settings.at("heuristic").m_name, "HashMapHeuristic");
    ASSERT_EQ(subcomponent_settings.at("hash_function").m_name, "StateStringHashFunction");
}
