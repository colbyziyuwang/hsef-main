#include <gtest/gtest.h>

#include "environments/sliding_tile_puzzle/sliding_tile_names.h"
#include "environments/sliding_tile_puzzle/sliding_tile_utils.h"
#include "utils/string_utils.h"

/**
 * Tests that the right vector is returned in the unit cost case.
 */
TEST(SlidingTileUtilsTests, getTileMoveCostsUnitTest) {
    std::vector<double> size1 = getTileMoveCosts(1, SlidingTileCostType::unit);
    ASSERT_EQ(size1.size(), 1);
    ASSERT_EQ(size1[0], 0);

    std::vector<double> size5 = getTileMoveCosts(5, SlidingTileCostType::unit);
    ASSERT_EQ(size5.size(), 5);

    std::vector<double> size5_expected = {0, 1, 1, 1, 1};
    ASSERT_EQ(size5, size5_expected);
}

/**
 * Tests that the right vector is returned in the heavy cost case.
 */
TEST(SlidingTileUtilsTests, getTileMoveCostsUnitHeavyTest) {
    std::vector<double> size1 = getTileMoveCosts(1, SlidingTileCostType::heavy);
    ASSERT_EQ(size1.size(), 1);
    ASSERT_EQ(size1[0], 0);

    std::vector<double> size5 = getTileMoveCosts(5, SlidingTileCostType::heavy);
    ASSERT_EQ(size5.size(), 5);

    std::vector<double> size5_expected = {0, 1, 2, 3, 4};
    ASSERT_EQ(size5, size5_expected);
}

/**
 * Tests that the right vector is returned in the inverse cost case.
 */
TEST(SlidingTileUtilsTests, getTileMoveCostsUnitInverseTest) {
    std::vector<double> size1 = getTileMoveCosts(1, SlidingTileCostType::inverse);
    ASSERT_EQ(size1.size(), 1);
    ASSERT_EQ(size1[0], 0);

    std::vector<double> size5 = getTileMoveCosts(5, SlidingTileCostType::inverse);
    ASSERT_EQ(size5.size(), 5);

    std::vector<double> size5_expected = {0, 1, 0.5, 1.0 / 3.0, 0.25};
    ASSERT_EQ(size5, size5_expected);
}

/**
 * Tests that outputing cost type works.
 */
TEST(SlidingTileUtilsTests, outputCostTypeTest) {
    ASSERT_EQ(streamableToString(SlidingTileCostType::unit), slidingTileNames::COST_UNIT);
    ASSERT_EQ(streamableToString(SlidingTileCostType::heavy), slidingTileNames::COST_HEAVY);
    ASSERT_EQ(streamableToString(SlidingTileCostType::inverse), slidingTileNames::COST_INVERSE);
}