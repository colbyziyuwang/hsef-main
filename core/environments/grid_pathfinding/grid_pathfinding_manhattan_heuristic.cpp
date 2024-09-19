#include "grid_pathfinding_manhattan_heuristic.h"
#include "building_tools/evaluators/node_evaluator_with_cache.h"
#include "grid_location.h"
#include "grid_names.h"
#include "grid_pathfinding_action.h"
#include "logging/logging_terms.h"
#include "search_basics/node_container.h"
#include "utils/string_utils.h"

#include <cstdlib>

GridPathfindingManhattanHeuristic::GridPathfindingManhattanHeuristic(GridLocation goal_state)
          : m_goal_state(goal_state) {
}

GridPathfindingManhattanHeuristic::GridPathfindingManhattanHeuristic(int x_coord, int y_coord)
          : GridPathfindingManhattanHeuristic(GridLocation(x_coord, y_coord)) {
}

void GridPathfindingManhattanHeuristic::setGoalState(int x_coord, int y_coord) {
    setGoalState(GridLocation(x_coord, y_coord));
}

void GridPathfindingManhattanHeuristic::doEvaluateAndCache(NodeID to_evaluate) {
    const GridLocation& state = getNodeContainer()->getState(to_evaluate);
    auto h_value = static_cast<double>(
              abs(m_goal_state.m_x_coord - state.m_x_coord) +
              abs(m_goal_state.m_y_coord - state.m_y_coord));
    setCachedValues(to_evaluate, h_value, false);
}

void GridPathfindingManhattanHeuristic::setGoalState(const GridLocation& goal_state) {
    m_goal_state = goal_state;
}

GridLocation GridPathfindingManhattanHeuristic::getGoalState() const {
    return m_goal_state;
}

StringMap GridPathfindingManhattanHeuristic::getComponentSettings() const {
    using namespace gridNames;

    return {{SETTING_GOAL_STATE, streamableToString(m_goal_state)}};
}