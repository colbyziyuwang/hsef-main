#include <gtest/gtest.h>

#include "environments/pancake_puzzle/gap_heuristic.h"
#include "environments/pancake_puzzle/pancake_names.h"
#include "environments/pancake_puzzle/pancake_state.h"
#include "test_helpers.h"

/**
 * Tests heuristic computation in GapHeuristic for some permutations. Expected values calculated by hand.
 */
TEST(GapHeuristicTests, unitCostgapValuesTest) {
    std::vector<Pancake> perm{0, 1, 2, 3, 4};
    PancakeState state1(perm);

    GapHeuristic heuristic;
    ASSERT_TRUE(checkDistanceToGoStateEvaluation(heuristic, state1, 0.0, false, 0.0));

    perm = {4, 3, 2, 1, 0};
    PancakeState state2(perm);
    ASSERT_TRUE(checkDistanceToGoStateEvaluation(heuristic, state2, 1.0, false, 1.0));

    perm = {4, 2, 0, 3, 1};
    PancakeState state3(perm);
    ASSERT_TRUE(checkDistanceToGoStateEvaluation(heuristic, state3, 5.0, false, 5.0));

    perm = {3, 2, 0, 4, 1};
    PancakeState state4(perm);
    ASSERT_TRUE(checkDistanceToGoStateEvaluation(heuristic, state4, 4.0, false, 4.0));

    perm = {3, 2, 0, 1, 4};
    PancakeState state5(perm);
    ASSERT_TRUE(checkDistanceToGoStateEvaluation(heuristic, state5, 2.0, false, 2.0));

    perm = {4, 2, 0, 1, 3};
    PancakeState state6(perm);
    ASSERT_TRUE(checkDistanceToGoStateEvaluation(heuristic, state6, 4.0, false, 4.0));
}

/**
 * Tests heuristic computation in heavy gap heuristic for some permutations. Expected values calculated by hand.
 */
TEST(GapHeuristicTests, heavyCostgapValuesTest) {
    std::vector<Pancake> perm{0, 1, 2, 3, 4};
    PancakeState state1(perm);

    GapHeuristic heuristic(PancakePuzzleCostType::heavy);
    ASSERT_TRUE(checkDistanceToGoStateEvaluation(heuristic, state1, 0.0, false, 0.0));

    perm = {4, 3, 2, 1, 0};
    PancakeState state2(perm);
    ASSERT_TRUE(checkDistanceToGoStateEvaluation(heuristic, state2, 1.0, false, 1.0));

    perm = {4, 2, 0, 3, 1};
    PancakeState state3(perm);
    ASSERT_TRUE(checkDistanceToGoStateEvaluation(heuristic, state3, 9.0, false, 5.0));

    perm = {3, 2, 0, 4, 1};
    PancakeState state4(perm);
    ASSERT_TRUE(checkDistanceToGoStateEvaluation(heuristic, state4, 6.0, false, 4.0));

    perm = {3, 2, 0, 1, 4};
    PancakeState state5(perm);
    ASSERT_TRUE(checkDistanceToGoStateEvaluation(heuristic, state5, 3.0, false, 2.0));

    perm = {4, 2, 0, 1, 3};
    PancakeState state6(perm);
    ASSERT_TRUE(checkDistanceToGoStateEvaluation(heuristic, state6, 10.0, false, 4.0));
}

/**
 * Checks that getAllSettings is works.
 */
TEST(GapHeuristicTests, getSettingsTest) {
    using namespace pancakeNames;

    std::vector<Pancake> perm{0, 1, 2, 3, 4};
    PancakeState state1(perm);

    GapHeuristic heuristic(PancakePuzzleCostType::heavy);

    auto settings = heuristic.getAllSettings();
    ASSERT_EQ(settings.m_name, GapHeuristic::CLASS_NAME);
    ASSERT_EQ(settings.m_main_settings.size(), 1);
    ASSERT_EQ(settings.m_main_settings[SETTING_COST_TYPE], COST_HEAVY);
    ASSERT_EQ(settings.m_sub_component_settings.size(), 0);
}