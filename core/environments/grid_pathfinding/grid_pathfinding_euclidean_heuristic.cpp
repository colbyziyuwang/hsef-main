#include "grid_pathfinding_euclidean_heuristic.h"
#include "grid_location.h"
#include "grid_names.h"
#include "grid_pathfinding_action.h"
#include "logging/logging_terms.h"
#include "search_basics/node_container.h"
#include "utils/string_utils.h"

#include <cassert>
#include <cmath>

GridPathfindingEuclideanHeuristic::GridPathfindingEuclideanHeuristic(GridLocation goal_state)
          : m_goal_state(goal_state) {
}

GridPathfindingEuclideanHeuristic::GridPathfindingEuclideanHeuristic(int x_coord, int y_coord)
          : GridPathfindingEuclideanHeuristic(GridLocation(x_coord, y_coord)) {
}

void GridPathfindingEuclideanHeuristic::setGoalState(int x_coord, int y_coord) {
    setGoalState(GridLocation(x_coord, y_coord));
}

void GridPathfindingEuclideanHeuristic::doEvaluateAndCache(NodeID to_evaluate) {
    const GridLocation& state = getNodeContainer()->getState(to_evaluate);
    double h_value = sqrt(pow(abs(m_goal_state.m_x_coord - state.m_x_coord), 2.0) +
                          pow(abs(m_goal_state.m_y_coord - state.m_y_coord), 2.0));
    setCachedValues(to_evaluate, h_value, false);
}

void GridPathfindingEuclideanHeuristic::setGoalState(const GridLocation& goal_state) {
    m_goal_state = goal_state;
}

GridLocation GridPathfindingEuclideanHeuristic::getGoalState() const {
    return m_goal_state;
}

StringMap GridPathfindingEuclideanHeuristic::getComponentSettings() const {
    using namespace gridNames;
    return {{SETTING_GOAL_STATE, streamableToString(m_goal_state)}};
}