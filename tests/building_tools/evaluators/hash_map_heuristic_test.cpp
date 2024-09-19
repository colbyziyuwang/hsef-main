#include <gtest/gtest.h>

#include "building_tools/evaluators/hash_map_heuristic.h"
#include "building_tools/hashing/permutation_hash_function.h"
#include "building_tools/hashing/state_string_hash_function.h"
#include "environments/grid_pathfinding/grid_location.h"
#include "environments/grid_pathfinding/grid_location_hash_function.h"
#include "environments/grid_pathfinding/grid_pathfinding_transitions.h"
#include "environments/sliding_tile_puzzle/sliding_tile_state.h"
#include "environments/sliding_tile_puzzle/sliding_tile_transitions.h"
#include "test_helpers.h"


/**
 * Tests that the default value of 0.0 is returned when no states are added. Map pathfinding is used for the test.
 *
 * TODO Update to use map location hash function when implemented
 */
TEST(HashMapHeuristicTests, mapPathfindingNoHValuesAddedTest) {
    GridLocation state(123, 839);
    GridLocationHashFunction hash_function;
    HashMapHeuristic<GridLocation, GridDirection, uint32_t> heuristic(hash_function);

    ASSERT_TRUE(checkStateEvaluation(heuristic, state, 0.0, false));
}

/**
 * Tests that the correct values and default values are used after states are added. Map pathfinding is used for the
 * test.
 *
 */
TEST(HashMapHeuristicTests, mapPathfindingAddingHValuesTest) {
    GridLocation state1(123, 839);
    GridLocation state2(69, 420);
    GridLocation state3(901, 2048);

    GridLocationHashFunction hash_function;
    HashMapHeuristic<GridLocation, GridDirection, uint32_t> heuristic(hash_function, 32.0);

    heuristic.addHeuristicValue(state1, 55.0, false);
    heuristic.addHeuristicValue(state2, 11.0, true);

    ASSERT_TRUE(checkStateEvaluation(heuristic, state1, 55.0, false));
    ASSERT_TRUE(checkStateEvaluation(heuristic, state2, 11.0, true));
    ASSERT_TRUE(checkStateEvaluation(heuristic, state3, 32.0, false));
}

/**
 * Tests that the correct values and default values are used after states are added. Sliding tile puzzle is used for the
 * test.
 *
 */
TEST(HashMapHeuristicTests, slidingTileAddingHValueTest) {
    std::vector<int> perm1{0, 1, 2, 3, 4, 5};
    std::vector<int> perm2{1, 4, 2, 3, 0, 5};
    std::vector<int> perm3{5, 3, 2, 1, 0, 4};

    SlidingTileState state1(perm1, 2, 3);
    SlidingTileState state2(perm2, 2, 3);
    SlidingTileState state3(perm3, 2, 3);

    // StateStringHashFunction<SlidingTileState> hash_function;
    PermutationHashFunction<SlidingTileState> hash_function;
    HashMapHeuristic<SlidingTileState, BlankSlide, uint64_t> heuristic(hash_function, 32.0);

    heuristic.addHeuristicValue(state1, 55.0, false);
    heuristic.addHeuristicValue(state2, 11.0, true);

    ASSERT_TRUE(checkStateEvaluation(heuristic, state1, 55.0, false));
    ASSERT_TRUE(checkStateEvaluation(heuristic, state2, 11.0, true));
    ASSERT_TRUE(checkStateEvaluation(heuristic, state3, 32.0, false));
}

/**
 * Tests getAllSettings functionality
 */
TEST(HashMapHeuristicTests, getSettingsTest) {
    GridLocationHashFunction hash_function;

    HashMapHeuristic<GridLocation, GridDirection, uint32_t> heuristic(hash_function, 2.0);

    auto settings = heuristic.getAllSettings();
    ASSERT_EQ(settings.m_name, heuristic.CLASS_NAME);
    ASSERT_EQ(settings.m_main_settings.size(), 1);
    ASSERT_EQ(settings.m_main_settings["default_h_value"], "2.0");
    ASSERT_EQ(settings.m_sub_component_settings.size(), 1);

    auto& add_hash = settings.m_sub_component_settings.at("hash_function");
    ASSERT_EQ(add_hash.m_name, "GridLocationHashFunction");
    ASSERT_EQ(add_hash.m_main_settings.size(), 1);
    ASSERT_EQ(add_hash.m_main_settings.at("first_dimension_size"), "65536");
    ASSERT_EQ(add_hash.m_sub_component_settings.size(), 0);
}