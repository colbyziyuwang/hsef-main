#include <gtest/gtest.h>

#include "environments/grid_pathfinding/grid_location_hash_function.h"

/**
 * Tests hash value in case that no width is set
 */
TEST(GridLocationHashFunctionTests, getHashValueDefaultTest) {
    GridLocationHashFunction hasher;

    ASSERT_EQ(hasher.getHashValue(GridLocation(3, 8)), 524291u);
    ASSERT_EQ(hasher.getHashValue(GridLocation(7, 0)), 7u);
    ASSERT_EQ(hasher.getHashValue(GridLocation(36, 99)), 6488100u);
    ASSERT_EQ(hasher.getHashValue(GridLocation(0, 31)), 2031616u);
    ASSERT_EQ(hasher.getHashValue(GridLocation(65535, 0)), 65535u);
    ASSERT_EQ(hasher.getHashValue(GridLocation(65535, 5)), 393215u);
}

/**
 * Tests hash value in the case a width is set.
 */
TEST(GridLocationHashFunctionTests, getHashValueSetWidthTest) {
    GridLocationHashFunction hasher;
    hasher.setMapWidth(256);

    ASSERT_EQ(hasher.getHashValue(GridLocation(3, 8)), 2051u);
    ASSERT_EQ(hasher.getHashValue(GridLocation(7, 0)), 7u);
    ASSERT_EQ(hasher.getHashValue(GridLocation(36, 99)), 25380u);
    ASSERT_EQ(hasher.getHashValue(GridLocation(0, 31)), 7936u);
    ASSERT_EQ(hasher.getHashValue(GridLocation(255, 0)), 255u);
    ASSERT_EQ(hasher.getHashValue(GridLocation(255, 5)), 1535u);

    ASSERT_DEBUG_DEATH(hasher.setMapWidth(-2), "");
}

/**
 * Tests that it works when given a transitions object.
 */
TEST(GridLocationHashFunctionTests, getHashValueSetWidthWithTransitionsTest) {
    std::stringstream map_4x3("height 3\nwidth 4\nmap\nGST@\n.W.O\n....");
    GridMap grid(map_4x3);
    GridPathfindingTransitions transitions(&grid);

    GridLocationHashFunction hasher;
    hasher.setMapWidth(transitions);

    ASSERT_EQ(hasher.getHashValue(GridLocation(0, 0)), 0u);
    ASSERT_EQ(hasher.getHashValue(GridLocation(3, 0)), 3u);
    ASSERT_EQ(hasher.getHashValue(GridLocation(2, 1)), 6u);
    ASSERT_EQ(hasher.getHashValue(GridLocation(0, 3)), 12u);
    ASSERT_EQ(hasher.getHashValue(GridLocation(3, 3)), 15u);
}

/**
 * Tests that isPerfectHashFunction always returns true.
 */
TEST(GridLocationHashFunctionTests, isPerfectHashFunctionTest) {
    GridLocationHashFunction hasher;
    ASSERT_TRUE(hasher.isPerfectHashFunction());
}

/**
 * Checks that getAllSettings returns the correct values.
 */
TEST(GridLocationHashFunctionTests, getSettingsTest) {
    GridLocationHashFunction hasher;

    auto settings = hasher.getAllSettings();
    ASSERT_EQ(settings.m_name, GridLocationHashFunction::CLASS_NAME);
    ASSERT_EQ(settings.m_main_settings.size(), 1);
    ASSERT_EQ(settings.m_main_settings["first_dimension_size"], "65536");
    ASSERT_EQ(settings.m_sub_component_settings.size(), 0);
}