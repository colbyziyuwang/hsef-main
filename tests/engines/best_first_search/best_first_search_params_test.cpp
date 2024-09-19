#include <gtest/gtest.h>

#include "engines/best_first_search/best_first_search_params.h"
#include "utils/string_utils.h"

/**
* Tests that getParameterLog contains the correct values
*/
TEST(BestFirstSearchParamsTests, getParameterLogTest) {
    BestFirstSearchParams params;
    StringMap log = params.getParameterLog();

    ASSERT_EQ(log.at("use_reopened"), boolToString(params.m_use_reopened));
    ASSERT_EQ(log.at("store_expansion_order"), boolToString(params.m_store_expansion_order));

    log = params.getParameterLog();

    params.m_use_reopened = false;
    log = params.getParameterLog();
    ASSERT_EQ(log.at("use_reopened"), boolToString(params.m_use_reopened));

    params.m_store_expansion_order = true;
    log = params.getParameterLog();
    ASSERT_EQ(log.at("store_expansion_order"), boolToString(params.m_store_expansion_order));
}