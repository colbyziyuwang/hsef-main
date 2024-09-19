#include <gtest/gtest.h>

#include <algorithm>
#include <sstream>

#include "environments/grid_pathfinding/grid_location.h"
#include "environments/grid_pathfinding/grid_names.h"
#include "environments/grid_pathfinding/grid_pathfinding_transitions.h"
#include "environments/grid_pathfinding/grid_pathfinding_utils.h"
#include "utils/floating_point_utils.h"
#include "utils/string_utils.h"

/**
 * Tests grid map loading with setGridMap
 */
TEST(GridPathfindingTransitionsTests, setGridMapTest) {
    std::stringstream map_3x4("height 4\nwidth 3\nmap\nGST\n@.W\n.O.\n...");
    GridMap grid_3x4(map_3x4);

    GridPathfindingTransitions map(&grid_3x4);
    GridLocation map_loc(0, 0);

    ASSERT_EQ(map.getMapWidth(), 3);
    ASSERT_EQ(map.getMapHeight(), 4);

    ASSERT_TRUE(map.isValidState(map_loc));
    map_loc = {2, 0};  // Set location to an obstacle location
    ASSERT_FALSE(map.isValidState(map_loc));
    map_loc = {4, 3};  // Set to an out of bounds location
    ASSERT_FALSE(map.isValidState(map_loc));
    map_loc = {10000, 19091};  // Set to an out of bounds location
    ASSERT_FALSE(map.isValidState(map_loc));

    std::stringstream map_4x3("height 3\nwidth 4\nmap\nGST@\n.W.O\n....");
    GridMap grid_4x3(map_4x3);
    map.setGridMap(&grid_4x3);

    ASSERT_EQ(map.getMapWidth(), 4);
    ASSERT_EQ(map.getMapHeight(), 3);

    map_loc = {3, 4};  // Set to an out of bounds location
    ASSERT_FALSE(map.isValidState(map_loc));
}

/**
 * Tests GridPathfindingTransitions 4-connected on a 3x4 grid with obstacles
 */
TEST(GridPathfindingTransitionsTests, fourConnectedTest) {
    std::stringstream map_3x4("height 4\nwidth 3\nmap\nGST\n@.W\n.O.\n...");
    GridMap grid_3x4(map_3x4);
    GridPathfindingTransitions map_3x4_trans(&grid_3x4);

    ASSERT_EQ(map_3x4_trans.getMapWidth(), 3);
    ASSERT_EQ(map_3x4_trans.getMapHeight(), 4);

    GridLocation map_3x4_loc(1, 1);

    std::vector<GridDirection> expected_actions{GridDirection::north};
    std::vector<GridDirection> current_actions = map_3x4_trans.getActions(map_3x4_loc);

    ASSERT_EQ(current_actions, expected_actions);
    for (auto action : GRID_PATHFINDING_ALL_ACTIONS) {
        if (std::find(current_actions.begin(), current_actions.end(), action) != current_actions.end()) {
            ASSERT_TRUE(map_3x4_trans.isApplicable(map_3x4_loc, action));
        } else {
            ASSERT_FALSE(map_3x4_trans.isApplicable(map_3x4_loc, action));
        }
    }

    map_3x4_trans.applyAction(map_3x4_loc, GridDirection::north);
    ASSERT_EQ(map_3x4_loc.m_x_coord, 1);  // Check to see if we got the correct next state
    ASSERT_EQ(map_3x4_loc.m_y_coord, 0);

    expected_actions = {GridDirection::south, GridDirection::west};
    current_actions = map_3x4_trans.getActions(map_3x4_loc);
    ASSERT_EQ(current_actions, expected_actions);

    map_3x4_trans.applyAction(map_3x4_loc, GridDirection::west);
    ASSERT_EQ(map_3x4_loc.m_x_coord, 0);
    ASSERT_EQ(map_3x4_loc.m_y_coord, 0);
}

/**
 * Tests GridPathfindingTransitions 8-connected on a 3x4 grid map with obstacles
 */
TEST(GridPathfindingTransitionsTests, eightConnectedTest) {
    std::stringstream map_3x4_8("height 4\nwidth 3\nmap\n...\n..W\n.O.\n...");
    GridMap grid_3x4_8(map_3x4_8);

    GridPathfindingTransitions map_3x4(&grid_3x4_8, GridConnectionType::four);

    ASSERT_EQ(map_3x4.getConnectionType(), GridConnectionType::four);
    map_3x4.setConnectionType(GridConnectionType::eight);
    ASSERT_EQ(map_3x4.getConnectionType(), GridConnectionType::eight);

    ASSERT_EQ(map_3x4.getMapWidth(), 3);
    ASSERT_EQ(map_3x4.getMapHeight(), 4);

    GridLocation map_3x4_loc(1, 1);

    std::vector<GridDirection> expected_actions{GridDirection::north, GridDirection::west, GridDirection::northwest};
    std::vector<GridDirection> current_actions = map_3x4.getActions(map_3x4_loc);
    ASSERT_EQ(current_actions, expected_actions);
    for (auto action : GRID_PATHFINDING_ALL_ACTIONS) {
        if (std::find(current_actions.begin(), current_actions.end(), action) != current_actions.end()) {
            ASSERT_TRUE(map_3x4.isApplicable(map_3x4_loc, action));
        } else {
            ASSERT_FALSE(map_3x4.isApplicable(map_3x4_loc, action));
        }
    }

    map_3x4.applyAction(map_3x4_loc, GridDirection::northwest);
    ASSERT_EQ(map_3x4_loc.m_x_coord, 0);  // Check to see if we got the correct next state
    ASSERT_EQ(map_3x4_loc.m_y_coord, 0);

    expected_actions = {GridDirection::east, GridDirection::southeast, GridDirection::south};
    current_actions = map_3x4.getActions(map_3x4_loc);
    ASSERT_EQ(current_actions, expected_actions);

    // Check bottom right corner case (2, 3)
    GridLocation map_3x4_bottom_right(2, 3);
    expected_actions = {GridDirection::north, GridDirection::west};
    current_actions = map_3x4.getActions(map_3x4_bottom_right);
    ASSERT_EQ(current_actions, expected_actions);
}

/**
 * Tests getActionCost, setDiagonalCost from GridPathfindingTransitions.
 * Uses a 3x4 empty grid (no obstacles)
 */
TEST(GridPathfindingTransitionsTests, getActionCostTest) {
    GridMap grid_3x4(4, 3);
    GridPathfindingTransitions map_transitions(&grid_3x4, GridConnectionType::four);

    GridLocation map_loc(1, 1);

    for (auto action : GRID_PATHFINDING_CARDINAL_ACTIONS) {
        ASSERT_DOUBLE_EQ(map_transitions.getActionCost(map_loc, action), 1.0);
    }

    map_transitions.setConnectionType(GridConnectionType::eight);

    for (auto action : GRID_PATHFINDING_DIAGONAL_ACTIONS) {
        ASSERT_DOUBLE_EQ(map_transitions.getActionCost(map_loc, action), ROOT_TWO);
    }

    const double pi_value = 3.14;
    ASSERT_DOUBLE_EQ(map_transitions.setDiagonalCost(pi_value), true);

    for (auto action : GRID_PATHFINDING_DIAGONAL_ACTIONS) {
        ASSERT_DOUBLE_EQ(map_transitions.getActionCost(map_loc, action), pi_value);
    }
}

/**
 * Tests getInverse to see if it returns the correct inverse action.
 */
TEST(GridPathfindingTransitionsTests, getInverseTest) {
    GridMap grid_3x4(4, 3);
    GridPathfindingTransitions map_transition(&grid_3x4, GridConnectionType::eight);

    GridLocation map_loc(1, 1);

    ASSERT_EQ(map_transition.getInverse(map_loc, GridDirection::north), GridDirection::south);
    ASSERT_EQ(map_transition.getInverse(map_loc, GridDirection::northeast), GridDirection::southwest);
    ASSERT_EQ(map_transition.getInverse(map_loc, GridDirection::east), GridDirection::west);
    ASSERT_EQ(map_transition.getInverse(map_loc, GridDirection::southeast), GridDirection::northwest);
    ASSERT_EQ(map_transition.getInverse(map_loc, GridDirection::south), GridDirection::north);
    ASSERT_EQ(map_transition.getInverse(map_loc, GridDirection::southwest), GridDirection::northeast);
    ASSERT_EQ(map_transition.getInverse(map_loc, GridDirection::west), GridDirection::east);
    ASSERT_EQ(map_transition.getInverse(map_loc, GridDirection::northwest), GridDirection::southeast);
}


/**
 * Tests setting cost type to life on an empty 3x4 grid
 */
TEST(GridPathfindingTransitionsTests, lifeCostTest) {
    GridMap grid_3x4(4, 3);
    GridPathfindingTransitions map_transitions(&grid_3x4, GridConnectionType::four, GridPathfindingCostType::life);

    GridLocation map_loc(1, 1);

    for (auto action : GRID_PATHFINDING_CARDINAL_ACTIONS) {
        ASSERT_DOUBLE_EQ(map_transitions.getActionCost(map_loc, action), 1.0);
    }

    GridLocation map_loc_2(1, 0);
    for (auto action : GRID_PATHFINDING_CARDINAL_ACTIONS) {
        ASSERT_DOUBLE_EQ(map_transitions.getActionCost(map_loc_2, action), 0.0);
    }

    GridLocation map_loc_3(2, 3);
    for (auto action : GRID_PATHFINDING_CARDINAL_ACTIONS) {
        ASSERT_DOUBLE_EQ(map_transitions.getActionCost(map_loc_3, action), 3.0);
    }
}

/**
 * Checks that getAllSettings is works.
 */
TEST(GridPathfindingTransitionsTests, getSettingsTest) {
    using namespace gridNames;

    GridMap map(4, 3);
    GridPathfindingTransitions transitions(&map, GridConnectionType::four, GridPathfindingCostType::life);
    transitions.setDiagonalCost(3.456);

    auto settings = transitions.getAllSettings();
    ASSERT_EQ(settings.m_name, GridPathfindingTransitions::CLASS_NAME);
    ASSERT_EQ(settings.m_main_settings.size(), 3);
    ASSERT_EQ(settings.m_main_settings[SETTING_COST_TYPE], COST_LIFE);
    ASSERT_EQ(settings.m_main_settings[SETTING_GRID_CONNECTION_TYPE], FOUR_CONNECTED);
    ASSERT_EQ(settings.m_main_settings[SETTING_DIAG_COST], "3.456");
    ASSERT_EQ(settings.m_sub_component_settings.size(), 0);
}