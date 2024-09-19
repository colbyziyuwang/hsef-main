#include <gtest/gtest.h>

#include "environments/grid_pathfinding/grid_location.h"
#include "environments/grid_pathfinding/grid_names.h"
#include "environments/grid_pathfinding/grid_pathfinding_utils.h"
#include "utils/string_utils.h"

/**
 * Tests readInGridPathfindingProblems where we specify an empty input file
 */
TEST(GridPathfindingUtilsTests, emptyFileTest) {
    std::stringstream empty_file("");
    std::vector<GridLocation> starts, goals;

    ASSERT_TRUE(readInGridPathfindingProblems(empty_file, starts, goals));
    ASSERT_EQ(starts.size(), 0);
    ASSERT_EQ(goals.size(), 0);
}

/**
 * Tests readInGridPathfindingProblems where we specify a file with some valid input
 */
TEST(GridPathfindingUtilsTests, validFileTest) {
    std::stringstream valid_file("0 2 3 4\n5 6 7 8\n9 10 11 12");
    std::vector<GridLocation> starts, goals;
    std::vector<GridLocation> expected_starts, expected_goals;

    ASSERT_TRUE(readInGridPathfindingProblems(valid_file, starts, goals));

    expected_starts = {GridLocation(0, 2), GridLocation(5, 6), GridLocation(9, 10)};
    expected_goals = {GridLocation(3, 4), GridLocation(7, 8), GridLocation(11, 12)};
    ASSERT_EQ(starts, expected_starts);
    ASSERT_EQ(goals, expected_goals);
}

/**
 * Tests readInGridPathfindingProblems where we specify a file with some valid input, but the line s
 * have more strings than 4 numbers
 */
TEST(GridPathfindingUtilsTests, invalidFileExtraNumsInLineTest) {
    std::stringstream invalid_file("1 2 3 4 0 10\n5 6 7 8 3 3 3 3\n9 10 11 12 69 100 78 test");
    std::vector<GridLocation> starts, goals;

    ASSERT_FALSE(readInGridPathfindingProblems(invalid_file, starts, goals));

    // Input vector sizes are expected not to change in size
    ASSERT_EQ(starts.size(), 0);
    ASSERT_EQ(goals.size(), 0);
}

/**
 * Tests readInGridPathfindingProblems where we specify a file with technically invalid input,
 * as one of the goal state's x coordinate is a string "test".
 */
TEST(GridPathfindingUtilsTests, invalidFileStringsTest) {
    std::stringstream invalid_file("1 2 test 4\n5 6 7 8\n9 10 11 12");
    std::vector<GridLocation> starts, goals;

    ASSERT_FALSE(readInGridPathfindingProblems(invalid_file, starts, goals));

    // Input vector sizes are expected not to change in size
    ASSERT_EQ(starts.size(), 0);
    ASSERT_EQ(goals.size(), 0);
}

/**
 * Checks that output worls for cost type
 */
TEST(GridPathfindingUtilsTests, outputCostTypeToStringTest) {
    ASSERT_EQ(streamableToString(GridPathfindingCostType::standard), gridNames::COST_STANDARD);
    ASSERT_EQ(streamableToString(GridPathfindingCostType::life), gridNames::COST_LIFE);
}
