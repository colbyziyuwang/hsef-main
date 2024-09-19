#include <gtest/gtest.h>

#include "environments/grid_pathfinding/grid_location.h"

/**
 * Tests GridLocation where we specify no additional values
 * (ie. create a map location where x = 0, y = 0).
 * Member values x and y should be 0.
 * We also expect that two default map locations with no additional parameters
 * will be equal to each other.
 */
TEST(GridLocationTests, createWithNoParamsTest) {
    GridLocation grid_location_a;

    ASSERT_EQ(grid_location_a.m_x_coord, -1);
    ASSERT_EQ(grid_location_a.m_y_coord, -1);

    std::ostringstream state_string;
    state_string << grid_location_a;
    ASSERT_EQ(state_string.str(), "(-1, -1)");

    GridLocation grid_location_b;
    ASSERT_TRUE(grid_location_a == grid_location_b);
    ASSERT_FALSE(grid_location_a != grid_location_b);
}

/**
 * Tests GridLocation where we specify additional values
 * (ie. create a map location where x = 17, y = 38).
 * Member values x and y should be 17 and 38 respectively. 
 */
TEST(GridLocationTests, createWithParamsTest) {
    GridLocation grid_location_a(17, 38);

    ASSERT_EQ(grid_location_a.m_x_coord, 17);
    ASSERT_EQ(grid_location_a.m_y_coord, 38);

    std::ostringstream state_string;
    state_string << grid_location_a;
    ASSERT_EQ(state_string.str(), "(17, 38)");

    GridLocation grid_location_b(0, 0);
    ASSERT_FALSE(grid_location_a == grid_location_b);
    ASSERT_TRUE(grid_location_a != grid_location_b);

    GridLocation grid_location_c(420, 69);
    ASSERT_FALSE(grid_location_a == grid_location_c);
    ASSERT_TRUE(grid_location_a != grid_location_c);

    GridLocation grid_location_d(17, 38);
    ASSERT_TRUE(grid_location_a == grid_location_d);
    ASSERT_FALSE(grid_location_a != grid_location_d);
}
