#include "sliding_tile_manhattan_heuristic.h"
#include "logging/logging_terms.h"
#include "search_basics/node_evaluator.h"
#include "sliding_tile_names.h"
#include "sliding_tile_state.h"
#include "sliding_tile_transitions.h"
#include "sliding_tile_utils.h"
#include "utils/string_utils.h"

#include <cassert>
#include <cmath>

using std::abs;
using std::vector;

SlidingTileManhattanHeuristic::SlidingTileManhattanHeuristic(
          const SlidingTileState& goal_state, SlidingTileCostType cost_type)
          : m_goal_state(goal_state), m_num_rows(goal_state.m_num_rows), m_num_cols(goal_state.m_num_cols),
            m_puzzle_size(goal_state.m_num_rows * goal_state.m_num_cols), m_cost_type(cost_type) {

    updateHeuristicCache();
}

void SlidingTileManhattanHeuristic::updateHeuristicCache() {
    m_tile_h_value.resize(m_puzzle_size, vector<double>(m_puzzle_size, 0.0));
    m_tile_distance_to_go.resize(m_puzzle_size, vector<double>(m_puzzle_size, 0.0));

    vector<double> tile_move_cost = getTileMoveCosts(m_puzzle_size, m_cost_type);

    for (int goal_pos = 0; goal_pos < m_puzzle_size; goal_pos++) {
        Tile tile_num = m_goal_state.m_permutation[goal_pos];

        if (tile_num == 0) {
            continue;
        }

        for (int pos = 0; pos < m_puzzle_size; pos++) {
            m_tile_distance_to_go[tile_num][pos] = abs((goal_pos % m_num_cols) - (pos % m_num_cols));  // column difference
            m_tile_distance_to_go[tile_num][pos] += abs(goal_pos / m_num_cols - pos / m_num_cols);  // row difference
            m_tile_h_value[tile_num][pos] = m_tile_distance_to_go[tile_num][pos] * tile_move_cost[tile_num];  // weight by the tile move cost
        }
    }
}


void SlidingTileManhattanHeuristic::doEvaluateAndCache(NodeID to_evaluate) {
    assert(isValidState(getNodeContainer()->getState(to_evaluate)));

    const auto& permutation = getNodeContainer()->getState(to_evaluate).m_permutation;
    double h_value = 0.0;
    double m_total_distance_to_go = 0.0;

    for (unsigned pos = 0; pos < permutation.size(); pos++) {
        Tile tile_num = permutation[pos];
        h_value += m_tile_h_value[tile_num][pos];
        m_total_distance_to_go += m_tile_distance_to_go[tile_num][pos];
    }
    setCachedDistanceToGoEval(to_evaluate, m_total_distance_to_go);
    setCachedValues(to_evaluate, h_value, false);
}

bool SlidingTileManhattanHeuristic::isValidState(const SlidingTileState& state) const {
    return (m_num_rows == state.m_num_rows && m_num_cols == state.m_num_cols);
}

void SlidingTileManhattanHeuristic::setCostType(SlidingTileCostType cost_type) {
    m_cost_type = cost_type;
    updateHeuristicCache();
}

void SlidingTileManhattanHeuristic::setGoalState(const SlidingTileState& goal_state) {
    m_goal_state = goal_state;
    updateHeuristicCache();
}

SlidingTileState SlidingTileManhattanHeuristic::getGoalState() const {
    return m_goal_state;
}

double SlidingTileManhattanHeuristic::getCachedDistanceToGoEval(NodeID node_id) const {
    assert(node_id < m_distance_to_go_evals.size());
    return m_distance_to_go_evals[node_id];
}

void SlidingTileManhattanHeuristic::setCachedDistanceToGoEval(NodeID node_id, double eval) {
    if (node_id >= m_distance_to_go_evals.size()) {
        m_distance_to_go_evals.resize(node_id + 1, 0.0);
    }
    m_distance_to_go_evals[node_id] = eval;
}

double SlidingTileManhattanHeuristic::getLastDistanceToGoEval() const {
    assert(isEvalComputed());

    return m_distance_to_go_evals[getIDofLastEvaluatedNode()];
}

StringMap SlidingTileManhattanHeuristic::getComponentSettings() const {
    using namespace slidingTileNames;

    return {{SETTING_COST_TYPE, streamableToString(m_cost_type)},
              {SETTING_GOAL_STATE, streamableToString(m_goal_state)}};
}