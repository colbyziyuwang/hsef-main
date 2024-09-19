#ifndef GRID_LOCATION_H_
#define GRID_LOCATION_H_

#include <iostream>

/**
 * Defines a state for 2D grid pathfinding. Each state is a location in the grid given as a set of coordinates.
 *
 * @class GridLocation
 */
class GridLocation {
public:
    /**
     * Default constructor for a grid location. Sets the coordinates to (-1,-1).
     */
    GridLocation() = default;

    /**
     * Constructor for the grid location that sets the coordinates to the given values.
     * @param x_coord The x coordinate for the state.
     * @param y_coord The y coordinate for the state.
     */
    GridLocation(int x_coord, int y_coord);

    int m_x_coord = -1;  ///< The x coordinate of the state.
    int m_y_coord = -1;  ///< The y coordinate of the state.
};

/**
 * Outputs a string representation of a grid location to the given output stream.
 *
 * @param out The output stream.
 * @param loc The grid location to output.
 * @return The output stream.
 */
std::ostream& operator<<(std::ostream& out, const GridLocation& loc);

/**
 * Defines equality of two grid locations.
 *
 * @param loc1 The first location to test.
 * @param loc2 The second location to test.
 * @return If the locations are equal or not.
 */
bool operator==(const GridLocation& loc1, const GridLocation& loc2);

/**
 * Defines inequality of two grid locations.
 *
 * @param loc1 The first location to test.
 * @param loc2 The second location to test.
 * @return If the locations are not equal.
 */
bool operator!=(const GridLocation& loc1, const GridLocation& loc2);

#endif /* GRID_LOCATION_H_ */
