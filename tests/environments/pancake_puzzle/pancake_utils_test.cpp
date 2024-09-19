#include <gtest/gtest.h>

#include "environments/pancake_puzzle/pancake_names.h"
#include "environments/pancake_puzzle/pancake_utils.h"
#include "utils/string_utils.h"

/**
 * Checks that cost type to string works.
 */
TEST(PancakeUtilsTest, pancakeCostTypeToStringTest) {
    ASSERT_EQ(streamableToString(PancakePuzzleCostType::unit), pancakeNames::COST_UNIT);
    ASSERT_EQ(streamableToString(PancakePuzzleCostType::heavy), pancakeNames::COST_HEAVY);
}