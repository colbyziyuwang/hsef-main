#include <algorithm>
#include <gtest/gtest.h>
#include <iterator>
#include <vector>

#include "building_tools/evaluators/distance_to_go_wrapper_evaluator.h"
#include "building_tools/evaluators/set_aggregate_evaluator.h"
#include "building_tools/evaluators/set_aggregate_evaluator_factory.h"
#include "engines/engine_components/eval_functions/f_cost_evaluator.h"
#include "environments/grid_pathfinding/grid_location.h"
#include "environments/grid_pathfinding/grid_pathfinding_action.h"
#include "environments/grid_pathfinding/grid_pathfinding_euclidean_heuristic.h"
#include "environments/grid_pathfinding/grid_pathfinding_lifecost_heuristic.h"
#include "environments/grid_pathfinding/grid_pathfinding_manhattan_heuristic.h"
#include "environments/grid_pathfinding/grid_pathfinding_octile_heuristic.h"
#include "environments/sliding_tile_puzzle/sliding_tile_action.h"
#include "environments/sliding_tile_puzzle/sliding_tile_manhattan_heuristic.h"
#include "environments/sliding_tile_puzzle/sliding_tile_state.h"
#include "environments/sliding_tile_puzzle/sliding_tile_transitions.h"
#include "utils/evaluator_utils.h"

class EvaluatorUtilsTests : public ::testing::Test {
protected:
    void SetUp() override {
    }

public:
    GridPathfindingLifecostHeuristic life_cost;
    DistanceToGoWrapperEvaluator<GridLocation, GridDirection> life_cost_wrapper = DistanceToGoWrapperEvaluator(life_cost);

    GridPathfindingOctileHeuristic octile;
    DistanceToGoWrapperEvaluator<GridLocation, GridDirection> octile_wrapper = DistanceToGoWrapperEvaluator(octile);

    GridPathfindingEuclideanHeuristic euclid;
    GridPathfindingManhattanHeuristic manhattan;

    SetAggregateEvaluator<GridLocation, GridDirection> sum_lc_o_euclid = getSumEvaluator<GridLocation, GridDirection>({&life_cost, &octile, &euclid});
    SetAggregateEvaluator<GridLocation, GridDirection> max_lc_o = getMaxEvaluator<GridLocation, GridDirection>({&life_cost, &octile});
    SetAggregateEvaluator<GridLocation, GridDirection> max_man_euclid = getMaxEvaluator<GridLocation, GridDirection>({&manhattan, &euclid});
    SetAggregateEvaluator<GridLocation, GridDirection> max_wrapper = getMaxEvaluator<GridLocation, GridDirection>({&life_cost_wrapper, &octile_wrapper});

    SetAggregateEvaluator<GridLocation, GridDirection> max_everything = getMaxEvaluator<GridLocation, GridDirection>({&max_wrapper, &max_lc_o, &sum_lc_o_euclid, &max_man_euclid});
};

/**
 * Helper method for tests below that returns the index of a pointer in a vector of pointers, and -1 if it isn't there.
 *
 * @param vec The vector to check
 * @param eval The pointer to check for
 * @return The index of the pointer in the vector
 */
int getVectorIndex(const std::vector<NodeEvaluator<GridLocation, GridDirection>*>& vec, const NodeEvaluator<GridLocation, GridDirection>* eval) {
    auto result = std::find(vec.begin(), vec.end(), eval);
    if (result == vec.end()) {
        return -1;
    }
    return static_cast<int>(std::distance(vec.begin(), result));
}

/**
 * Tets that an empty vector is returned when an empty vector of evaluators is given
 */
TEST_F(EvaluatorUtilsTests, emptyVectorTest) {
    std::vector<NodeEvaluator<GridLocation, GridDirection>*> evals;
    auto evaluators = getAllEvaluators(evals);

    ASSERT_EQ(evaluators.size(), 0);
}

/**
 * Tests that a single evaluator is handled correctly
 */
TEST_F(EvaluatorUtilsTests, singleEvaluatorTest) {
    std::vector<NodeEvaluator<GridLocation, GridDirection>*> evals;
    evals.push_back(&life_cost);

    auto evaluators = getAllEvaluators(evals);

    ASSERT_EQ(evaluators.size(), 1);
    ASSERT_EQ(evaluators[0], &life_cost);
}

/**
 * Tests that a single evaluator with a single sub-evaluator works correctly when the sub-evaluator is first in the
 * list.
 */
TEST_F(EvaluatorUtilsTests, singleSubHeuristicTest) {
    std::vector<NodeEvaluator<GridLocation, GridDirection>*> evals;
    evals.push_back(&life_cost);
    evals.push_back(&life_cost_wrapper);

    auto evaluators = getAllEvaluators(evals);

    ASSERT_EQ(evaluators.size(), 2);
    ASSERT_EQ(evaluators[0], &life_cost);
    ASSERT_EQ(evaluators[1], &life_cost_wrapper);
}

/**
 * Tests that a single evaluator with a single sub-evaluator works correctly when the sub-evaluator is second in
 * the given list
 */
TEST_F(EvaluatorUtilsTests, singleSubHeuristicBackwardsTest) {
    std::vector<NodeEvaluator<GridLocation, GridDirection>*> evals;

    evals.push_back(&life_cost_wrapper);
    evals.push_back(&life_cost);

    auto evaluators = getAllEvaluators(evals);

    ASSERT_EQ(evaluators.size(), 2);
    ASSERT_EQ(evaluators[0], &life_cost);
    ASSERT_EQ(evaluators[1], &life_cost_wrapper);
}

/**
 * Ensures that duplicates in the given list are handled correctly
 */
TEST_F(EvaluatorUtilsTests, singleSumWithDuplicatesTest) {

    std::vector<NodeEvaluator<GridLocation, GridDirection>*> evals;
    evals.push_back(&life_cost);
    evals.push_back(&euclid);
    evals.push_back(&life_cost);
    evals.push_back(&octile);
    evals.push_back(&sum_lc_o_euclid);
    evals.push_back(&euclid);
    evals.push_back(&sum_lc_o_euclid);

    auto evaluators = getAllEvaluators(evals);
    ASSERT_EQ(evaluators.size(), 4);
    // Check all evaluators are there
    ASSERT_TRUE(getVectorIndex(evaluators, &life_cost) >= 0);
    ASSERT_TRUE(getVectorIndex(evaluators, &euclid) >= 0);
    ASSERT_TRUE(getVectorIndex(evaluators, &octile) >= 0);
    ASSERT_EQ(evaluators[3], &sum_lc_o_euclid);
    // Check the sub-evaluators occur before the sum
    ASSERT_TRUE(getVectorIndex(evaluators, &life_cost) < 3);
    ASSERT_TRUE(getVectorIndex(evaluators, &euclid) < 3);
    ASSERT_TRUE(getVectorIndex(evaluators, &octile) < 3);
}

TEST_F(EvaluatorUtilsTests, twoIndependentComponentsTest) {

    std::vector<NodeEvaluator<GridLocation, GridDirection>*> evals;
    evals.push_back(&max_lc_o);
    evals.push_back(&max_man_euclid);

    auto evaluators = getAllEvaluators(evals);
    ASSERT_EQ(evaluators.size(), 6);
    // Check all evaluators are there
    ASSERT_TRUE(getVectorIndex(evaluators, &life_cost) >= 0);
    ASSERT_TRUE(getVectorIndex(evaluators, &octile) >= 0);
    ASSERT_TRUE(getVectorIndex(evaluators, &max_lc_o) >= 0);
    ASSERT_TRUE(getVectorIndex(evaluators, &manhattan) >= 0);
    ASSERT_TRUE(getVectorIndex(evaluators, &euclid) >= 0);
    ASSERT_TRUE(getVectorIndex(evaluators, &max_man_euclid) >= 0);

    // Check the sub-evaluators occur before the aggregators
    ASSERT_TRUE(getVectorIndex(evaluators, &life_cost) < getVectorIndex(evaluators, &max_lc_o));
    ASSERT_TRUE(getVectorIndex(evaluators, &octile) < getVectorIndex(evaluators, &max_lc_o));
    ASSERT_TRUE(getVectorIndex(evaluators, &manhattan) < getVectorIndex(evaluators, &max_man_euclid));
    ASSERT_TRUE(getVectorIndex(evaluators, &euclid) < getVectorIndex(evaluators, &max_man_euclid));
}

/**
 * Tests on a more complex evaluator DAG with multiple intersecting dependcies
 */
TEST_F(EvaluatorUtilsTests, complexDagTest) {
    std::vector<NodeEvaluator<GridLocation, GridDirection>*> evals;
    evals.push_back(&max_everything);

    auto evaluators = getAllEvaluators(evals);
    ASSERT_EQ(evaluators.size(), 11);

    // Check all evaluators are there
    ASSERT_TRUE(getVectorIndex(evaluators, &life_cost) >= 0);
    ASSERT_TRUE(getVectorIndex(evaluators, &octile) >= 0);
    ASSERT_TRUE(getVectorIndex(evaluators, &euclid) >= 0);
    ASSERT_TRUE(getVectorIndex(evaluators, &manhattan) >= 0);

    ASSERT_TRUE(getVectorIndex(evaluators, &life_cost_wrapper) >= 0);
    ASSERT_TRUE(getVectorIndex(evaluators, &octile_wrapper) >= 0);

    ASSERT_TRUE(getVectorIndex(evaluators, &max_lc_o) >= 0);
    ASSERT_TRUE(getVectorIndex(evaluators, &max_man_euclid) >= 0);
    ASSERT_TRUE(getVectorIndex(evaluators, &sum_lc_o_euclid) >= 0);

    ASSERT_TRUE(getVectorIndex(evaluators, &max_wrapper) >= 0);
    ASSERT_TRUE(getVectorIndex(evaluators, &max_everything) >= 0);

    // Check dependency ordering is satisfied
    ASSERT_TRUE(getVectorIndex(evaluators, &life_cost) < getVectorIndex(evaluators, &life_cost_wrapper));
    ASSERT_TRUE(getVectorIndex(evaluators, &octile) < getVectorIndex(evaluators, &octile_wrapper));

    ASSERT_TRUE(getVectorIndex(evaluators, &life_cost) < getVectorIndex(evaluators, &max_lc_o));
    ASSERT_TRUE(getVectorIndex(evaluators, &octile) < getVectorIndex(evaluators, &max_lc_o));

    ASSERT_TRUE(getVectorIndex(evaluators, &manhattan) < getVectorIndex(evaluators, &max_man_euclid));
    ASSERT_TRUE(getVectorIndex(evaluators, &euclid) < getVectorIndex(evaluators, &max_man_euclid));

    ASSERT_TRUE(getVectorIndex(evaluators, &life_cost) < getVectorIndex(evaluators, &sum_lc_o_euclid));
    ASSERT_TRUE(getVectorIndex(evaluators, &octile) < getVectorIndex(evaluators, &sum_lc_o_euclid));
    ASSERT_TRUE(getVectorIndex(evaluators, &euclid) < getVectorIndex(evaluators, &sum_lc_o_euclid));

    ASSERT_TRUE(getVectorIndex(evaluators, &life_cost_wrapper) < getVectorIndex(evaluators, &max_wrapper));
    ASSERT_TRUE(getVectorIndex(evaluators, &octile_wrapper) < getVectorIndex(evaluators, &max_wrapper));

    ASSERT_TRUE(getVectorIndex(evaluators, &max_wrapper) < getVectorIndex(evaluators, &max_everything));
    ASSERT_TRUE(getVectorIndex(evaluators, &max_lc_o) < getVectorIndex(evaluators, &max_everything));
    ASSERT_TRUE(getVectorIndex(evaluators, &max_man_euclid) < getVectorIndex(evaluators, &max_everything));
    ASSERT_TRUE(getVectorIndex(evaluators, &sum_lc_o_euclid) < getVectorIndex(evaluators, &max_everything));
}

/**
 * Tests that updateEvaluatorGoalTests works as expected.
 */
TEST_F(EvaluatorUtilsTests, updateEvaluatorGoalStateTest) {
    SlidingTileState goal_state1 = SlidingTileState({0, 1, 2, 3, 4, 5}, 2, 3);
    SlidingTileManhattanHeuristic unit_h(goal_state1, SlidingTileCostType::unit);
    SlidingTileManhattanHeuristic inverse_h(goal_state1, SlidingTileCostType::inverse);
    FCostEvaluator<SlidingTileState, BlankSlide> f_cost(inverse_h);

    std::vector<NodeEvaluator<SlidingTileState, BlankSlide>*> evals = {&inverse_h, &f_cost, &unit_h};

    SlidingTileState goal_state2 = SlidingTileState({1, 2, 3, 4, 5, 0}, 2, 3);
    updateEvaluatorGoalState(evals, goal_state2);

    ASSERT_EQ(unit_h.getGoalState(), goal_state2);
    ASSERT_EQ(inverse_h.getGoalState(), goal_state2);
}