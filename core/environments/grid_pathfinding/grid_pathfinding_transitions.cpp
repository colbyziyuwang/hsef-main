#include <iostream>
#include <optional>

#include <cassert>
#include <vector>

#include "grid_location.h"
#include "grid_map.h"
#include "grid_names.h"
#include "grid_pathfinding_action.h"
#include "grid_pathfinding_transitions.h"
#include "grid_pathfinding_utils.h"
#include "logging/logging_terms.h"
#include "utils/floating_point_utils.h"
#include "utils/string_utils.h"


GridPathfindingTransitions::GridPathfindingTransitions(const GridMap* grid_map, GridConnectionType connection_type,
          GridPathfindingCostType cost_type)
          : m_grid_map(grid_map), m_cost_type(cost_type), m_connection_type(connection_type) {
    assert(cost_type == GridPathfindingCostType::standard ||
              (cost_type == GridPathfindingCostType::life && connection_type == GridConnectionType::four));
}

int GridPathfindingTransitions::getMapWidth() const {
    return m_grid_map->getWidth();
}

int GridPathfindingTransitions::getMapHeight() const {
    return m_grid_map->getHeight();
}

void GridPathfindingTransitions::setConnectionType(GridConnectionType connection_type) {
    m_connection_type = connection_type;
}

bool GridPathfindingTransitions::setDiagonalCost(double diag_cost) {
    if (fpLess(diag_cost, 0.0)) {
        std::cerr << "Diagonal cost of " << diag_cost << " is less than 0.0 and is thus invalid.\n";
        return false;
    }
    m_diag_cost = diag_cost;
    return true;
}


GridConnectionType GridPathfindingTransitions::getConnectionType() const {
    return m_connection_type;
}

bool GridPathfindingTransitions::isApplicable(const GridLocation& state, const GridDirection& action) const {
    switch (action) {
        case GridDirection::north:
            return m_grid_map->canMoveNorth(state.m_x_coord, state.m_y_coord);
        case GridDirection::east:
            return m_grid_map->canMoveEast(state.m_x_coord, state.m_y_coord);
        case GridDirection::south:
            return m_grid_map->canMoveSouth(state.m_x_coord, state.m_y_coord);
        case GridDirection::west:
            return m_grid_map->canMoveWest(state.m_x_coord, state.m_y_coord);
        default:
            break;
    }
    if (m_connection_type == GridConnectionType::eight) {
        switch (action) {
            case GridDirection::northeast:
                return m_grid_map->canMoveNorthEast(state.m_x_coord, state.m_y_coord);
            case GridDirection::southeast:
                return m_grid_map->canMoveSouthEast(state.m_x_coord, state.m_y_coord);
            case GridDirection::southwest:
                return m_grid_map->canMoveSouthWest(state.m_x_coord, state.m_y_coord);
            case GridDirection::northwest:
                return m_grid_map->canMoveNorthWest(state.m_x_coord, state.m_y_coord);
            default:
                break;
        }
    }
    return false;  // includes dummy case
}

double GridPathfindingTransitions::getActionCost(const GridLocation& state, const GridDirection& action) const {
    if (action == GridDirection::north || action == GridDirection::east || action == GridDirection::south ||
              action == GridDirection::west) {
        if (m_cost_type == GridPathfindingCostType::standard) {
            return 1.0;
        } else if (m_cost_type == GridPathfindingCostType::life) {
            return state.m_y_coord;
        }
    }
    assert(m_connection_type == GridConnectionType::eight);
    return m_diag_cost;
}

void GridPathfindingTransitions::applyAction(GridLocation& state, const GridDirection& action) const {
    switch (action) {
        case GridDirection::north:
            state.m_y_coord--;
            break;
        case GridDirection::northeast:
            state.m_x_coord++;
            state.m_y_coord--;
            break;
        case GridDirection::east:
            state.m_x_coord++;
            break;
        case GridDirection::southeast:
            state.m_x_coord++;
            state.m_y_coord++;
            break;
        case GridDirection::south:
            state.m_y_coord++;
            break;
        case GridDirection::southwest:
            state.m_x_coord--;
            state.m_y_coord++;
            break;
        case GridDirection::west:
            state.m_x_coord--;
            break;
        case GridDirection::northwest:
            state.m_x_coord--;
            state.m_y_coord--;
            break;
        default:
            break;
    }
}

// TODO Cache applicable actions at each location to make this faster
std::vector<GridDirection> GridPathfindingTransitions::getActions(const GridLocation& state) const {
    std::vector<GridDirection> actions;

    bool north = m_grid_map->canMoveNorth(state.m_x_coord, state.m_y_coord);
    bool east = m_grid_map->canMoveEast(state.m_x_coord, state.m_y_coord);
    bool south = m_grid_map->canMoveSouth(state.m_x_coord, state.m_y_coord);
    bool west = m_grid_map->canMoveWest(state.m_x_coord, state.m_y_coord);

    if (north) {
        actions.push_back(GridDirection::north);
    }
    if (m_connection_type == GridConnectionType::eight && north && east &&
              m_grid_map->canMoveNorthEast(state.m_x_coord, state.m_y_coord, false)) {
        actions.push_back(GridDirection::northeast);
    }
    if (east) {
        actions.push_back(GridDirection::east);
    }
    if (m_connection_type == GridConnectionType::eight && east && south &&
              m_grid_map->canMoveSouthEast(state.m_x_coord, state.m_y_coord, false)) {
        actions.push_back(GridDirection::southeast);
    }
    if (south) {
        actions.push_back(GridDirection::south);
    }
    if (m_connection_type == GridConnectionType::eight && south && west &&
              m_grid_map->canMoveSouthWest(state.m_x_coord, state.m_y_coord, false)) {
        actions.push_back(GridDirection::southwest);
    }
    if (west) {
        actions.push_back(GridDirection::west);
    }
    if (m_connection_type == GridConnectionType::eight && north && west &&
              m_grid_map->canMoveNorthWest(state.m_x_coord, state.m_y_coord, false)) {
        actions.push_back(GridDirection::northwest);
    }

    return actions;
}

std::optional<GridDirection> GridPathfindingTransitions::getInverse(const GridLocation&, const GridDirection& action) const {
    switch (action) {
        case GridDirection::north:
            return GridDirection::south;
        case GridDirection::northeast:
            return GridDirection::southwest;
        case GridDirection::east:
            return GridDirection::west;
        case GridDirection::southeast:
            return GridDirection::northwest;
        case GridDirection::south:
            return GridDirection::north;
        case GridDirection::southwest:
            return GridDirection::northeast;
        case GridDirection::west:
            return GridDirection::east;
        case GridDirection::northwest:
            return GridDirection::southeast;
        default:
            break;
    }
    return std::nullopt;
}

bool GridPathfindingTransitions::isValidState(const GridLocation& state) const {
    return m_grid_map->canOccupyLocation(state.m_x_coord, state.m_y_coord);
}

void GridPathfindingTransitions::setCostType(GridPathfindingCostType cost_type) {
    assert(cost_type == GridPathfindingCostType::standard ||
              (cost_type == GridPathfindingCostType::life && m_connection_type == GridConnectionType::four));
    m_cost_type = cost_type;
}

void GridPathfindingTransitions::setGridMap(const GridMap* grid_map) {
    m_grid_map = grid_map;
}

StringMap GridPathfindingTransitions::getComponentSettings() const {
    using namespace gridNames;

    return {{SETTING_COST_TYPE, streamableToString(m_cost_type)},
              {SETTING_GRID_CONNECTION_TYPE, streamableToString(m_connection_type)},
              {SETTING_DIAG_COST, roundAndToString(m_diag_cost, 6)}};
}
