#include <gtest/gtest.h>
#include <string>
#include <vector>

#include "building_tools/evaluators/hash_map_heuristic.h"
#include "building_tools/evaluators/set_aggregate_evaluator.h"
#include "building_tools/evaluators/set_aggregate_evaluator_factory.h"
#include "building_tools/hashing/state_string_hash_function.h"
#include "environments/grid_pathfinding/grid_location.h"
#include "environments/grid_pathfinding/grid_pathfinding_action.h"
#include "environments/sliding_tile_puzzle/sliding_tile_action.h"
#include "environments/sliding_tile_puzzle/sliding_tile_state.h"
#include "test_helpers.h"

/**
 * Tests the getMaxEvaluator function on map pathfinding states
 */
TEST(EvaluatorFactoryTests, mapPathfindingMaxTest) {
    GridLocation state1(123, 839);
    GridLocation state2(69, 420);
    GridLocation state3(901, 2048);

    StateStringHashFunction<GridLocation> hash_function;
    HashMapHeuristic<GridLocation, GridDirection, std::string> heuristic1(hash_function, 32.0);
    heuristic1.addHeuristicValue(state1, 55.0, false);
    heuristic1.addHeuristicValue(state2, 11.0, true);

    HashMapHeuristic<GridLocation, GridDirection, std::string> heuristic2(hash_function, 32.0);
    heuristic2.addHeuristicValue(state1, 69.0, false);
    heuristic2.addHeuristicValue(state2, 5.0, true);

    HashMapHeuristic<GridLocation, GridDirection, std::string> heuristic3(hash_function, 32.0);
    heuristic3.addHeuristicValue(state1, 45.0, false);
    heuristic3.addHeuristicValue(state2, 5.0, true);
    heuristic3.addHeuristicValue(state3, 777.0, true);

    SetAggregateEvaluator<GridLocation, GridDirection> max_heuristic = getMaxEvaluator<GridLocation, GridDirection>({&heuristic1, &heuristic2, &heuristic3});

    ASSERT_TRUE(checkStateEvaluation(max_heuristic, state1, 69.0, false));
    ASSERT_TRUE(checkStateEvaluation(max_heuristic, state2, 11.0, true));
    ASSERT_TRUE(checkStateEvaluation(max_heuristic, state3, 777.0, true));
}

/**
 * Tests the getMaxEvaluator function on sliding tile states
 */
TEST(EvaluatorFactoryTests, slidingTileMaxTest) {
    std::vector<int> perm1{0, 1, 2, 3, 4, 5};
    std::vector<int> perm2{1, 4, 2, 3, 0, 5};
    std::vector<int> perm3{5, 3, 2, 1, 0, 4};

    SlidingTileState state1(perm1, 2, 3);
    SlidingTileState state2(perm2, 2, 3);
    SlidingTileState state3(perm3, 2, 3);

    StateStringHashFunction<SlidingTileState> hash_function;
    HashMapHeuristic<SlidingTileState, BlankSlide, std::string> heuristic1(hash_function, 32.0);
    heuristic1.addHeuristicValue(state1, 69.0, false);
    heuristic1.addHeuristicValue(state2, 2.0, true);

    HashMapHeuristic<SlidingTileState, BlankSlide, std::string> heuristic2(hash_function, 32.0);
    heuristic2.addHeuristicValue(state1, 55.0, false);
    heuristic2.addHeuristicValue(state2, 11.0, true);
    heuristic2.addHeuristicValue(state3, 11.0, true);

    HashMapHeuristic<SlidingTileState, BlankSlide, std::string> heuristic3(hash_function, 32.0);
    heuristic3.addHeuristicValue(state1, 55.0, false);
    heuristic3.addHeuristicValue(state2, 1.0, true);

    SetAggregateEvaluator<SlidingTileState, BlankSlide> max_heuristic = getMaxEvaluator<SlidingTileState, BlankSlide>({&heuristic1, &heuristic2, &heuristic3});

    ASSERT_TRUE(checkStateEvaluation(max_heuristic, state1, 69.0, false));
    ASSERT_TRUE(checkStateEvaluation(max_heuristic, state2, 11.0, true));
    ASSERT_TRUE(checkStateEvaluation(max_heuristic, state3, 32.0, true));
}

/**
 * Tests the getSumEvaluator function on sliding tile states
 */
TEST(EvaluatorFactoryTests, slidingTileSumTest) {
    std::vector<int> perm1{0, 1, 2, 3, 4, 5};
    std::vector<int> perm2{1, 4, 2, 3, 0, 5};
    std::vector<int> perm3{5, 3, 2, 1, 0, 4};

    SlidingTileState state1(perm1, 2, 3);
    SlidingTileState state2(perm2, 2, 3);
    SlidingTileState state3(perm3, 2, 3);

    StateStringHashFunction<SlidingTileState> hash_function;
    HashMapHeuristic<SlidingTileState, BlankSlide, std::string> heuristic1(hash_function, 32.0);
    heuristic1.addHeuristicValue(state1, 69.0, false);
    heuristic1.addHeuristicValue(state2, 2.0, true);

    HashMapHeuristic<SlidingTileState, BlankSlide, std::string> heuristic2(hash_function, 32.0);
    heuristic2.addHeuristicValue(state1, 55.0, false);
    heuristic2.addHeuristicValue(state2, 11.0, true);
    heuristic2.addHeuristicValue(state3, 11.0, true);

    HashMapHeuristic<SlidingTileState, BlankSlide, std::string> heuristic3(hash_function, 32.0);
    heuristic3.addHeuristicValue(state1, 55.0, false);
    heuristic3.addHeuristicValue(state2, 1.0, true);

    SetAggregateEvaluator<SlidingTileState, BlankSlide> sum_heuristic = getSumEvaluator<SlidingTileState, BlankSlide>({&heuristic1, &heuristic2, &heuristic3});

    ASSERT_TRUE(checkStateEvaluation(sum_heuristic, state1, 179.0, false));
    ASSERT_TRUE(checkStateEvaluation(sum_heuristic, state2, 14.0, true));
    ASSERT_TRUE(checkStateEvaluation(sum_heuristic, state3, 75.0, true));
}