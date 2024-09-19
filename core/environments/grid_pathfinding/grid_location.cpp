#include "grid_location.h"

#include <cassert>
#include <iostream>

GridLocation::GridLocation(int x_coord, int y_coord)
          : m_x_coord(x_coord), m_y_coord(y_coord) {
    assert(x_coord >= 0 && y_coord >= 0);
}

std::ostream& operator<<(std::ostream& out, const GridLocation& loc) {
    out << "(" << loc.m_x_coord << ", " << loc.m_y_coord << ")";
    return out;
}

bool operator==(const GridLocation& loc1, const GridLocation& loc2) {
    return (loc1.m_x_coord == loc2.m_x_coord) && (loc1.m_y_coord == loc2.m_y_coord);
}

bool operator!=(const GridLocation& loc1, const GridLocation& loc2) {
    return !(loc1 == loc2);
}
