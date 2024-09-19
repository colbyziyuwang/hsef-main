#include "grid_pathfinding_action.h"
#include "grid_names.h"

#include <ostream>

std::ostream& operator<<(std::ostream& out, const GridDirection& action) {
    using namespace gridNames;

    switch (action) {
        case GridDirection::north:
            out << DIRECTION_NORTH;
            break;
        case GridDirection::northeast:
            out << DIRECTION_NORTHEAST;
            break;
        case GridDirection::east:
            out << DIRECTION_EAST;
            break;
        case GridDirection::southeast:
            out << DIRECTION_SOUTHEAST;
            break;
        case GridDirection::south:
            out << DIRECTION_SOUTH;
            break;
        case GridDirection::southwest:
            out << DIRECTION_SOUTHWEST;
            break;
        case GridDirection::west:
            out << DIRECTION_WEST;
            break;
        case GridDirection::northwest:
            out << DIRECTION_NORTHWEST;
            break;
        default:
            out << DIRECTION_DUMMY;
            break;
    }
    return out;
}