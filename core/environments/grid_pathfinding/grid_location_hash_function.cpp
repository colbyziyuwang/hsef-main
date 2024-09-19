#include "grid_location_hash_function.h"
#include "grid_location.h"
#include "grid_pathfinding_transitions.h"
#include "logging/logging_terms.h"

#include <cassert>
#include <cstdint>
#include <string>

uint32_t GridLocationHashFunction::getHashValue(const GridLocation& state) const {
    assert(static_cast<uint32_t>(state.m_x_coord) < m_first_dimension_size);
    return state.m_x_coord + state.m_y_coord * m_first_dimension_size;
}

void GridLocationHashFunction::setMapWidth(uint32_t map_width) {
    assert(map_width <= 65536 && map_width > 0);
    m_first_dimension_size = map_width;
}

void GridLocationHashFunction::setMapWidth(const GridPathfindingTransitions& transitions) {
    setMapWidth(transitions.getMapWidth());
}

StringMap GridLocationHashFunction::getComponentSettings() const {
    return {{"first_dimension_size", std::to_string(m_first_dimension_size)}};
}
