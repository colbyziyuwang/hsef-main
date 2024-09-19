#include <gtest/gtest.h>

#include "engines/iterative_deepening/id_engine_params.h"
#include "utils/string_utils.h"

/**
* Tests that getParameterLog contains the correct values
*/
TEST(IDEngineParamsTests, getParameterLogTest) {
    IDEngineParams params;
    StringMap log = params.getParameterLog();

    ASSERT_EQ(log.at("use_parent_pruning"), boolToString(params.m_use_parent_pruning));
    ASSERT_EQ(log.at("use_random_op_ordering"), boolToString(params.m_use_random_op_ordering));
}