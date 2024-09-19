#include "grid_pathfinding_octile_heuristic.h"
#include "grid_location.h"
#include "grid_names.h"
#include "logging/logging_terms.h"
#include "utils/floating_point_utils.h"
#include "utils/string_utils.h"

#include <cassert>
#include <cstdlib>
#include <iostream>

GridPathfindingOctileHeuristic::GridPathfindingOctileHeuristic(GridLocation goal_state, double diag_cost)
          : m_goal_state(goal_state) {
    setDiagonalCost(diag_cost);
}

GridPathfindingOctileHeuristic::GridPathfindingOctileHeuristic(int x_coord, int y_coord, double diag_cost)
          : GridPathfindingOctileHeuristic(GridLocation(x_coord, y_coord), diag_cost) {
}

void GridPathfindingOctileHeuristic::setGoalState(int x_coord, int y_coord) {
    setGoalState(GridLocation(x_coord, y_coord));
}

bool GridPathfindingOctileHeuristic::setDiagonalCost(double diag_cost) {
    if (!fpGreater(diag_cost, 0.0)) {
        std::cerr << "GridPathfindingOctileHeuristic::setDiagonalCost error: "
                  << "Given cost is not greater than 0.0\n";
        return false;
    }
    m_diag_cost = diag_cost;
    return true;
}

void GridPathfindingOctileHeuristic::doEvaluateAndCache(NodeID to_evaluate) {
    const GridLocation& state = getNodeContainer()->getState(to_evaluate);
    double h_value = 0.0;
    double num_steps = 0.0;

    if (!fpLess(m_diag_cost, 2.0)) {
        h_value = abs(m_goal_state.m_x_coord - state.m_x_coord) + abs(m_goal_state.m_y_coord - state.m_y_coord);
    } else {
        double delta_x = abs(m_goal_state.m_x_coord - state.m_x_coord);
        double delta_y = abs(m_goal_state.m_y_coord - state.m_y_coord);

        if (fpLess(delta_x, delta_y)) {
            h_value = delta_x * m_diag_cost + delta_y - delta_x;
            num_steps = delta_y;
        } else {
            h_value = delta_y * m_diag_cost + delta_x - delta_y;
            num_steps = delta_x;
        }
    }
    setCachedDistanceToGoEval(to_evaluate, num_steps);
    setCachedValues(to_evaluate, h_value, false);
}

void GridPathfindingOctileHeuristic::setGoalState(const GridLocation& goal_state) {
    m_goal_state = goal_state;
}

GridLocation GridPathfindingOctileHeuristic::getGoalState() const {
    return m_goal_state;
}

double GridPathfindingOctileHeuristic::getCachedDistanceToGoEval(NodeID node_id) const {
    assert(node_id < m_distance_to_go_evals.size());
    return m_distance_to_go_evals[node_id];
}

void GridPathfindingOctileHeuristic::setCachedDistanceToGoEval(NodeID node_id, double eval) {
    if (node_id >= m_distance_to_go_evals.size()) {
        m_distance_to_go_evals.resize(node_id + 1, 0.0);
    }
    m_distance_to_go_evals[node_id] = eval;
}

double GridPathfindingOctileHeuristic::getLastDistanceToGoEval() const {
    assert(isEvalComputed());

    return m_distance_to_go_evals[getIDofLastEvaluatedNode()];
}

StringMap GridPathfindingOctileHeuristic::getComponentSettings() const {
    using namespace gridNames;

    return {{SETTING_GOAL_STATE, streamableToString(m_goal_state)},
              {SETTING_DIAG_COST, roundAndToString(m_diag_cost, 6)}};
}
