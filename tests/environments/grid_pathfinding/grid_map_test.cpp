#include <gtest/gtest.h>

#include "environments/grid_pathfinding/grid_map.h"

/**
 * Checks that if we create a map with no locations, the expected behaviour occurs
 */
TEST(GridMapTests, emptyMapConstructorAllZeroesTests) {
    GridMap grid(0, 0);

    ASSERT_EQ(grid.getWidth(), 0);
    ASSERT_EQ(grid.getHeight(), 0);
    ASSERT_EQ(grid.getLocationType(0, 0), GridLocationType::outside_grid);
    ASSERT_EQ(grid.getLocationType(5, 10), GridLocationType::outside_grid);
}

/**
 * Tests that a map is correctly created in the empty map case.
 */
TEST(GridMapTests, emptyMapConstructorTests) {
    GridMap grid(5, 8);

    ASSERT_EQ(grid.getWidth(), 5);
    ASSERT_EQ(grid.getHeight(), 8);

    for (int x_coord = 0; x_coord < 5; x_coord++) {
        for (int y_coord = 0; y_coord < 8; y_coord++) {
            ASSERT_EQ(grid.getLocationType(x_coord, y_coord), GridLocationType::passable);
        }
    }

    ASSERT_EQ(grid.getLocationType(5, 0), GridLocationType::outside_grid);
    ASSERT_EQ(grid.getLocationType(2, 10), GridLocationType::outside_grid);
    ASSERT_EQ(grid.getLocationType(5, 8), GridLocationType::outside_grid);
    ASSERT_EQ(grid.getLocationType(-5, 0), GridLocationType::outside_grid);
}

/**
 * Tests map loading with loadMap works with a regular map
 */
TEST(GridMapTests, loadMapTest) {
    std::stringstream map_3x4("height 4\nwidth 3\nmap\nGST\n@.W\n.O.\n...");

    GridMap grid(map_3x4);

    ASSERT_EQ(grid.getWidth(), 3);
    ASSERT_EQ(grid.getHeight(), 4);

    ASSERT_EQ(grid.getLocationType(0, 0), GridLocationType::passable);
    ASSERT_EQ(grid.getLocationType(1, 0), GridLocationType::swamp);
    ASSERT_EQ(grid.getLocationType(2, 0), GridLocationType::tree);
    ASSERT_EQ(grid.getLocationType(0, 1), GridLocationType::obstacle);
    ASSERT_EQ(grid.getLocationType(1, 1), GridLocationType::passable);
    ASSERT_EQ(grid.getLocationType(2, 1), GridLocationType::water);
    ASSERT_EQ(grid.getLocationType(0, 2), GridLocationType::passable);
    ASSERT_EQ(grid.getLocationType(1, 2), GridLocationType::obstacle);
    ASSERT_EQ(grid.getLocationType(2, 2), GridLocationType::passable);
    ASSERT_EQ(grid.getLocationType(0, 3), GridLocationType::passable);
    ASSERT_EQ(grid.getLocationType(1, 3), GridLocationType::passable);
    ASSERT_EQ(grid.getLocationType(2, 3), GridLocationType::passable);

    ASSERT_EQ(grid.getLocationType(4, 3), GridLocationType::outside_grid);
    ASSERT_EQ(grid.getLocationType(2, 5), GridLocationType::outside_grid);
}

/**
 * Tests that GridMap correctly handles header reading.
 */
TEST(GridMapTests, loadMapHeaderTest) {
    // Tests can handle flipped dimensions
    std::stringstream map_4x3("width 4\nheight 3\nmap\nGST@\n.W.O\n....");

    GridMap grid(map_4x3);

    ASSERT_EQ(grid.getWidth(), 4);
    ASSERT_EQ(grid.getHeight(), 3);

    ASSERT_EQ(grid.getLocationType(3, 2), GridLocationType::passable);

    // Tests fails when duplicate dimensions
    std::stringstream multi_width("width 4\nheight 3\nwidth 3\nmap\nGST@\n.W.O\n....");
    ASSERT_DEBUG_DEATH(GridMap fails(multi_width), "");

    std::stringstream multi_height("width 4\nheight 3\nheight 3\nmap\nGST@\n.W.O\n....");
    ASSERT_DEBUG_DEATH(GridMap fails(multi_height), "");

    // Tests with missing dimensions
    std::stringstream no_width("height 3\nmap\nGST@\n.W.O\n....");
    ASSERT_DEBUG_DEATH(GridMap fails(no_width), "");

    std::stringstream no_height("width 3\nmap\nGST@\n.W.O\n....");
    ASSERT_DEBUG_DEATH(GridMap fails(no_height), "");

    // Tests with missing map token
    std::stringstream no_map("width 4\nheight 3\nGST@\n.W.O\n....");
    ASSERT_DEBUG_DEATH(GridMap fails(no_map), "");

    // Tests with extra useless line
    std::stringstream extra_line("width 4\nheight 3\nblah\nmap\nGST@\n.W.O\n....");
    ASSERT_DEBUG_DEATH(GridMap fails(extra_line), "");
}

/**
 * Tests that load map handles incorrect maps.
 */
TEST(GridMapTests, loadMapBadMapTest) {
// Tests fails with bad entry
#ifndef NDEBUG
    std::stringstream invalid_symbol("width 4\nheight 3\nmap\nGST@\n.W.O\n.Q..");
    ASSERT_DEBUG_DEATH(GridMap fails(invalid_symbol), "");

    // Tests with one invalid line width
    std::stringstream bad_width("width 4\nheight 3\nmap\nGST@\n.W.O.\n....");
    ASSERT_DEBUG_DEATH(GridMap fails(bad_width), "");

    // Tests with too many rows
    std::stringstream too_many_rows("width 4\nheight 3\nmap\nGST@\n.W.O\n....\n....");
    ASSERT_DEBUG_DEATH(GridMap fails(too_many_rows), "");

    // Tests with too few rows
    std::stringstream too_few_rows("width 4\nheight 3\nmap\nGST@\n.W.O");
    ASSERT_DEBUG_DEATH(GridMap fails(too_few_rows), "");
#endif
}

/**
 * Checks that can occupy correctly classifies different locations.
 */
TEST(GridMapTests, canOccupyTest) {
    std::stringstream map_4x3("width 4\nheight 3\nmap\n..WW\n..@S\n.SSS");

    GridMap grid(map_4x3);

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 3; j++) {
            if (i == 2 && j == 1) {
                ASSERT_FALSE(grid.canOccupyLocation(i, j));
            } else {
                ASSERT_TRUE(grid.canOccupyLocation(i, j));
            }
        }
    }
}

/**
 * Checks that canMove currently captures the different cases
 */
TEST(GridMapTests, canMoveTest) {
    std::stringstream map_4x3("width 5\nheight 4\nmap\n..W..\n..WS.\n.SSS.\n..@..");

    GridMap grid(map_4x3);

    // Checks top and bottom row for moves that leave grid
    for (int i = 0; i < 5; i++) {
        ASSERT_FALSE(grid.canMoveNorth(i, 0));
        ASSERT_FALSE(grid.canMoveNorthEast(i, 0, true));
        ASSERT_FALSE(grid.canMoveNorthEast(i, 0, false));
        ASSERT_FALSE(grid.canMoveNorthWest(i, 0, true));
        ASSERT_FALSE(grid.canMoveNorthWest(i, 0, false));

        if (i != 2) {  // skip the obstacle
            ASSERT_FALSE(grid.canMoveSouth(i, 3));
            ASSERT_FALSE(grid.canMoveSouthEast(i, 3, true));
            ASSERT_FALSE(grid.canMoveSouthEast(i, 3, false));
            ASSERT_FALSE(grid.canMoveSouthWest(i, 3, true));
            ASSERT_FALSE(grid.canMoveSouthWest(i, 3, false));
        }
    }

    // Checks the leftmost and rightmost column for moves that leave grid
    for (int j = 0; j < 4; j++) {
        ASSERT_FALSE(grid.canMoveWest(0, j));
        ASSERT_FALSE(grid.canMoveNorthWest(0, j, true));
        ASSERT_FALSE(grid.canMoveNorthWest(0, j, false));
        ASSERT_FALSE(grid.canMoveSouthWest(0, j, true));
        ASSERT_FALSE(grid.canMoveSouthWest(0, j, false));

        ASSERT_FALSE(grid.canMoveEast(4, j));
        ASSERT_FALSE(grid.canMoveNorthEast(4, j, true));
        ASSERT_FALSE(grid.canMoveNorthEast(4, j, false));
        ASSERT_FALSE(grid.canMoveSouthEast(4, j, true));
        ASSERT_FALSE(grid.canMoveSouthEast(4, j, false));
    }

    // Check switching of terrain works
    ASSERT_FALSE(grid.canMoveEast(1, 0));  // Check cannot move from terrain to water
    ASSERT_FALSE(grid.canMoveWest(3, 0));  // Check cannot move from terrain to water

    ASSERT_FALSE(grid.canMoveEast(2, 0));  // Check cannot move from water to terrain
    ASSERT_FALSE(grid.canMoveWest(2, 0));  // Check cannot move from water to terrain

    ASSERT_FALSE(grid.canMoveWest(3, 1));  // Check cannot move from swamp to water
    ASSERT_FALSE(grid.canMoveEast(2, 1));  // Check cannot move from water to swamp

    ASSERT_TRUE(grid.canMoveEast(0, 2));  // Can move from terrain to swamp
    ASSERT_TRUE(grid.canMoveWest(1, 2));  // Can move from swamp to terrain

    ASSERT_TRUE(grid.canMoveSouth(2, 0));  // Can move from water to water

    // Check all 8 directions for one location
    ASSERT_TRUE(grid.canMoveNorth(1, 2));
    ASSERT_FALSE(grid.canMoveNorthEast(1, 2, true));  // runs into water
    ASSERT_FALSE(grid.canMoveNorthEast(1, 2, false));  // runs into water
    ASSERT_TRUE(grid.canMoveEast(1, 2));
    ASSERT_FALSE(grid.canMoveSouthEast(1, 2, true));  // runs into obstacle
    ASSERT_FALSE(grid.canMoveSouthEast(1, 2, false));  // runs into obstacle
    ASSERT_TRUE(grid.canMoveSouth(1, 2));
    ASSERT_TRUE(grid.canMoveSouthWest(1, 2, true));
    ASSERT_TRUE(grid.canMoveSouthWest(1, 2, false));
    ASSERT_TRUE(grid.canMoveWest(1, 2));
    ASSERT_TRUE(grid.canMoveNorthWest(1, 2, true));
    ASSERT_TRUE(grid.canMoveNorthWest(1, 2, false));

    // Check all 8 directions for one location where obstacles interfere with diagonal
    ASSERT_FALSE(grid.canMoveNorth(2, 2));
    ASSERT_FALSE(grid.canMoveNorthEast(2, 2, true));  // Blocked by water to north
    ASSERT_TRUE(grid.canMoveNorthEast(2, 2, false));  // Check that ignore works correctly
    ASSERT_TRUE(grid.canMoveEast(2, 2));
    ASSERT_FALSE(grid.canMoveSouthEast(2, 2, true));  // Blocked by obstacle to south
    ASSERT_TRUE(grid.canMoveSouthEast(2, 2, false));  // Check that ignore works correctly
    ASSERT_FALSE(grid.canMoveSouth(2, 2));
    ASSERT_FALSE(grid.canMoveSouthWest(2, 2, true));  // Blocked by obstacle to south
    ASSERT_TRUE(grid.canMoveSouthWest(2, 2, false));  // Check that ignore works correctly
    ASSERT_TRUE(grid.canMoveWest(2, 2));
    ASSERT_FALSE(grid.canMoveNorthWest(2, 2, true));  // blocked by obstacle to north
    ASSERT_TRUE(grid.canMoveNorthWest(2, 2, false));  // Check that ignore works correctly
}