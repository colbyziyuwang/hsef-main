#include <gtest/gtest.h>

#include "environments/burnt_pancake_puzzle/burnt_gap_heuristic.h"
#include "environments/burnt_pancake_puzzle/burnt_pancake_state.h"
#include "environments/pancake_puzzle/pancake_names.h"
#include "test_helpers.h"

/**
 * Tests heuristic computation in BurntGapHeuristic for some permutations. Expected values calculated by hand.
 */
TEST(BurntGapHeuristicTests, unitGapValueTest) {
    std::vector<BurntPancake> perm{1, -2, 3, 4, 5};
    BurntPancakeState state1(perm);

    BurntGapHeuristic heuristic;
    ASSERT_TRUE(checkDistanceToGoStateEvaluation(heuristic, state1, 2.0, false, 2.0));

    perm = {-5, -4, -3, -2, -1};
    BurntPancakeState state2(perm);
    ASSERT_TRUE(checkDistanceToGoStateEvaluation(heuristic, state2, 1.0, false, 1.0));

    perm = {-5, 3, -1, -4, 2};
    BurntPancakeState state3(perm);
    ASSERT_TRUE(checkDistanceToGoStateEvaluation(heuristic, state3, 5.0, false, 5.0));

    perm = {4, -3, -1, 5, 2};
    BurntPancakeState state4(perm);
    ASSERT_TRUE(checkDistanceToGoStateEvaluation(heuristic, state4, 5.0, false, 5.0));

    perm = {4, 3, 1, 2, -5};
    BurntPancakeState state5(perm);
    ASSERT_TRUE(checkDistanceToGoStateEvaluation(heuristic, state5, 3.0, false, 3.0));

    perm = {-5, 3, 1, 2, 4};
    BurntPancakeState state6(perm);
    ASSERT_TRUE(checkDistanceToGoStateEvaluation(heuristic, state6, 4.0, false, 4.0));
}

TEST(BurntGapHeuristicTests, heavyGapValueTest) {
    std::vector<BurntPancake> perm{1, -2, 3, 4, 5};
    BurntPancakeState state1(perm);

    BurntGapHeuristic heuristic(PancakePuzzleCostType::heavy);
    ASSERT_TRUE(checkDistanceToGoStateEvaluation(heuristic, state1, 3.0, false, 2.0));

    perm = {-5, -4, -3, -2, -1};
    BurntPancakeState state2(perm);
    ASSERT_TRUE(checkDistanceToGoStateEvaluation(heuristic, state2, 1.0, false, 1.0));

    perm = {-5, 3, -1, -4, 2};
    BurntPancakeState state3(perm);
    ASSERT_TRUE(checkDistanceToGoStateEvaluation(heuristic, state3, 9.0, false, 5.0));

    perm = {4, -3, -1, 5, 2};
    BurntPancakeState state4(perm);
    ASSERT_TRUE(checkDistanceToGoStateEvaluation(heuristic, state4, 9.0, false, 5.0));

    perm = {4, 3, 1, 2, -5};
    BurntPancakeState state5(perm);
    ASSERT_TRUE(checkDistanceToGoStateEvaluation(heuristic, state5, 8.0, false, 3.0));

    perm = {-5, 3, 1, 2, 4};
    BurntPancakeState state6(perm);
    ASSERT_TRUE(checkDistanceToGoStateEvaluation(heuristic, state6, 10.0, false, 4.0));
}

/**
 * Checks that getAllSettings is works.
 */
TEST(BurntGapHeuristicTests, getSettingsTest) {
    using namespace pancakeNames;

    std::vector<Pancake> perm{-5, 3, 1, 2, 4};
    PancakeState state1(perm);

    BurntGapHeuristic heuristic(PancakePuzzleCostType::heavy);

    auto settings = heuristic.getAllSettings();
    ASSERT_EQ(settings.m_name, BurntGapHeuristic::CLASS_NAME);
    ASSERT_EQ(settings.m_main_settings.size(), 1);
    ASSERT_EQ(settings.m_main_settings[SETTING_COST_TYPE], COST_HEAVY);
    ASSERT_EQ(settings.m_sub_component_settings.size(), 0);
}