#include <gtest/gtest.h>

#include "engines/best_first_search/a_star_epsilon_params.h"
#include "utils/string_utils.h"

/**
* Tests that getParameterLog contains the correct values
*/
TEST(AStarEpsilonParamsTests, getParameterLogTest) {
    AStarEpsilonParams params;
    StringMap log = params.getParameterLog();

    ASSERT_EQ(log.at("weight"), roundAndToString(params.m_weight, 2));
    ASSERT_EQ(log.at("use_reopened"), boolToString(params.m_use_reopened));
    ASSERT_EQ(log.at("parent_heuristic_updating"), boolToString(params.m_parent_heuristic_updating));
    ASSERT_EQ(log.at("store_expansion_order"), boolToString(params.m_store_expansion_order));

    params.m_weight = 3.5;
    log = params.getParameterLog();
    ASSERT_EQ(log.at("weight"), roundAndToString(params.m_weight, 2));

    params.m_use_reopened = false;
    log = params.getParameterLog();
    ASSERT_EQ(log.at("use_reopened"), boolToString(params.m_use_reopened));

    params.m_parent_heuristic_updating = true;
    log = params.getParameterLog();
    ASSERT_EQ(log.at("parent_heuristic_updating"), boolToString(params.m_parent_heuristic_updating));

    params.m_store_expansion_order = true;
    log = params.getParameterLog();
    ASSERT_EQ(log.at("store_expansion_order"), boolToString(params.m_store_expansion_order));
}