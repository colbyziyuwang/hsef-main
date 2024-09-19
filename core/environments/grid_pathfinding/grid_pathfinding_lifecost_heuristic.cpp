#include "grid_pathfinding_lifecost_heuristic.h"
#include "building_tools/evaluators/node_evaluator_with_cache.h"
#include "grid_location.h"
#include "grid_names.h"
#include "grid_pathfinding_action.h"
#include "logging/logging_terms.h"
#include "search_basics/node_container.h"
#include "utils/string_utils.h"

#include <algorithm>
#include <cassert>
#include <cstdlib>

GridPathfindingLifecostHeuristic::GridPathfindingLifecostHeuristic(GridLocation goal_state)
          : m_goal_state(goal_state) {
}

GridPathfindingLifecostHeuristic::GridPathfindingLifecostHeuristic(int x_coord, int y_coord)
          : GridPathfindingLifecostHeuristic(GridLocation(x_coord, y_coord)) {
}

void GridPathfindingLifecostHeuristic::setGoalState(int x_coord, int y_coord) {
    setGoalState(GridLocation(x_coord, y_coord));
}

void GridPathfindingLifecostHeuristic::doEvaluateAndCache(NodeID to_evaluate) {
    const GridLocation& state = getNodeContainer()->getState(to_evaluate);
    double min_y = std::min(state.m_y_coord, m_goal_state.m_y_coord);
    double max_y = std::max(state.m_y_coord, m_goal_state.m_y_coord);
    double delta_x = abs(state.m_x_coord - m_goal_state.m_x_coord);
    double delta_y = max_y - min_y;
    double num_steps = 0;

    /** sum_between_ys calculates the cost of moving in the vertical direction between min_y and max_y.
     * The final subtraction of the goal state's y coordinate corresponds to the fact that we do not 
     * take further action once it is reached.
     */
    double sum_between_ys = (min_y + max_y) * (delta_y + 1) / 2 - m_goal_state.m_y_coord;
    double right_angle_path = sum_between_ys + min_y * delta_x;

    /** up_horizontal_down is the distance that corresponds to going all the way up to the top row 
     * from the starting state, then coming back down to the goal state.
     */
    double up_horizontal_down = (1 + min_y) * min_y / 2 + (1 + max_y) * max_y / 2 - m_goal_state.m_y_coord;

    if (right_angle_path < up_horizontal_down) {
        num_steps = delta_x + delta_y;
        setCachedDistanceToGoEval(to_evaluate, num_steps);
        setCachedValues(to_evaluate, right_angle_path, false);
    } else {
        num_steps = delta_x + state.m_y_coord + m_goal_state.m_y_coord;
        setCachedDistanceToGoEval(to_evaluate, num_steps);
        setCachedValues(to_evaluate, up_horizontal_down, false);
    }
}

void GridPathfindingLifecostHeuristic::setGoalState(const GridLocation& goal_state) {
    m_goal_state = goal_state;
}

GridLocation GridPathfindingLifecostHeuristic::getGoalState() const {
    return m_goal_state;
}

double GridPathfindingLifecostHeuristic::getCachedDistanceToGoEval(NodeID node_id) const {
    assert(node_id < m_distance_to_go_evals.size());
    return m_distance_to_go_evals[node_id];
}

void GridPathfindingLifecostHeuristic::setCachedDistanceToGoEval(NodeID node_id, double eval) {
    if (node_id >= m_distance_to_go_evals.size()) {
        m_distance_to_go_evals.resize(node_id + 1, 0.0);
    }
    m_distance_to_go_evals[node_id] = eval;
}

double GridPathfindingLifecostHeuristic::getLastDistanceToGoEval() const {
    assert(isEvalComputed());

    return m_distance_to_go_evals[getIDofLastEvaluatedNode()];
}

StringMap GridPathfindingLifecostHeuristic::getComponentSettings() const {
    using namespace gridNames;

    return {{SETTING_GOAL_STATE, streamableToString(m_goal_state)}};
}
