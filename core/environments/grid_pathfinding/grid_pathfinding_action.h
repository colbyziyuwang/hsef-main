#ifndef GRID_PATHFINDING_ACTION_H_
#define GRID_PATHFINDING_ACTION_H_

#include <cstdint>
#include <iostream>

/**
 * Defines the actions possible in the 2D grid pathfinding domain.
 *
 * @class GridDirection
 */
enum class GridDirection : std::uint8_t {
    north,
    northeast,
    east,
    southeast,
    south,
    southwest,
    west,
    northwest
};

/**
 * Defines an output representation 2D grid map pathfinding actions.
 *
 * @param out The output stream.
 * @param action The action to output.
 * @return A string representation of the given action.
 */
std::ostream& operator<<(std::ostream& out, const GridDirection& action);

#endif  //GRID_PATHFINDING_ACTION_H_
