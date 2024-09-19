#ifndef GRID_NAMES_H_
#define GRID_NAMES_H_

#include <string>

/**
 * Names for grid items
 */
namespace gridNames {
    inline const std::string SETTING_COST_TYPE = "cost_type";  ///< The string for cost type
    inline const std::string COST_STANDARD = "standard";  ///< The string for unit cost
    inline const std::string COST_LIFE = "life";  ///< The string for life cost

    inline const std::string SETTING_GRID_CONNECTION_TYPE = "grid_connection_type";  ///< The string for actions mode
    inline const std::string FOUR_CONNECTED = "4-connected";  ///< The string for 4-connected
    inline const std::string EIGHT_CONNECTED = "8-connected";  ///< The string for 8-connected

    inline const std::string DIRECTION_NORTH = "north";  ///< The string for direction north
    inline const std::string DIRECTION_EAST = "east";  ///< The string for direction east
    inline const std::string DIRECTION_SOUTH = "south";  ///< The string for direction south
    inline const std::string DIRECTION_WEST = "west";  ///< The string for direction west
    inline const std::string DIRECTION_DUMMY = "dummy";  ///< The string for direction dummy
    inline const std::string DIRECTION_NORTHEAST = "northeast";  ///< The string for direction northeast
    inline const std::string DIRECTION_NORTHWEST = "northwest";  ///< The string for direction northwest
    inline const std::string DIRECTION_SOUTHEAST = "southeast";  ///< The string for direction southeast
    inline const std::string DIRECTION_SOUTHWEST = "southwest";  ///< The string for direction southwest

    inline const std::string SETTING_DIAG_COST = "diagonal_cost";  ///< The string for diagonal_cost

    inline const std::string SETTING_GOAL_STATE = "goal_state";  ///< The goal state of the sliding tile puzzle
}  // namespace gridNames

#endif  //GRID_NAMES_H_
