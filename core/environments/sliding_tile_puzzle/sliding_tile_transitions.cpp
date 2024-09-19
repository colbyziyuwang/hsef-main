#include <cassert>
#include <optional>
#include <string>

#include "logging/logging_terms.h"
#include "sliding_tile_action.h"
#include "sliding_tile_names.h"
#include "sliding_tile_state.h"
#include "sliding_tile_transitions.h"
#include "sliding_tile_utils.h"
#include "utils/combinatorics.h"
#include "utils/string_utils.h"

using std::vector;

SlidingTileTransitions::SlidingTileTransitions(int rows, int cols, SlidingTileCostType cost_type) {
    assert(rows > 0 && cols > 0);
    setPuzzleDimensions(rows, cols);
    setCostType(cost_type);
}

bool SlidingTileTransitions::isApplicable(const SlidingTileState& state, const BlankSlide& action) const {
    return isApplicableInLocation(action, state.m_blank_loc);
}

double SlidingTileTransitions::getActionCost(const SlidingTileState& state, const BlankSlide& action) const {
    return m_tile_move_costs[getMovingTile(state, action)];
}

void SlidingTileTransitions::applyAction(SlidingTileState& state, const BlankSlide& action) const {
    if (action == BlankSlide::up) {
        state.m_permutation[state.m_blank_loc] = state.m_permutation[state.m_blank_loc - m_num_cols];
        state.m_blank_loc -= m_num_cols;
    } else if (action == BlankSlide::right) {
        state.m_permutation[state.m_blank_loc] = state.m_permutation[state.m_blank_loc + 1];
        state.m_blank_loc++;
    } else if (action == BlankSlide::down) {
        state.m_permutation[state.m_blank_loc] = state.m_permutation[state.m_blank_loc + m_num_cols];
        state.m_blank_loc += m_num_cols;
    } else if (action == BlankSlide::left) {
        state.m_permutation[state.m_blank_loc] = state.m_permutation[state.m_blank_loc - 1];
        state.m_blank_loc--;
    }

    state.m_permutation[state.m_blank_loc] = 0;
}

vector<BlankSlide> SlidingTileTransitions::getActions(const SlidingTileState& state) const {
    return m_loc_actions[state.m_blank_loc];
}

bool SlidingTileTransitions::isApplicableInLocation(const BlankSlide& action, int blank_loc) const {
    if (action == BlankSlide::up) {
        if (blank_loc >= m_num_cols) {
            return true;
        }
    } else if (action == BlankSlide::right) {
        if (blank_loc % m_num_cols < m_num_cols - 1) {
            return true;
        }
    } else if (action == BlankSlide::down) {
        if (blank_loc < (m_num_rows - 1) * m_num_cols) {
            return true;
        }
    } else if (action == BlankSlide::left) {
        if (blank_loc % m_num_cols > 0) {
            return true;
        }
    }
    return false;
}

void SlidingTileTransitions::setCostType(SlidingTileCostType cost_type) {
    m_cost_type = cost_type;
    m_tile_move_costs = getTileMoveCosts(m_num_rows * m_num_cols, m_cost_type);
}

void SlidingTileTransitions::setActionList() {
    m_ops_in_order.push_back(BlankSlide::up);
    m_ops_in_order.push_back(BlankSlide::right);
    m_ops_in_order.push_back(BlankSlide::down);
    m_ops_in_order.push_back(BlankSlide::left);

    m_loc_actions.clear();

    vector<BlankSlide> blank_actions(4);
    for (int i = 0; i < m_num_rows * m_num_cols; i++) {
        blank_actions.clear();
        for (auto& action : m_ops_in_order) {
            if (isApplicableInLocation(action, i)) {
                blank_actions.push_back(action);
            }
        }
        m_loc_actions.push_back(blank_actions);
    }
}

double SlidingTileTransitions::getTileMoveCost(Tile tile_num) const {
    return m_tile_move_costs[tile_num];
}

Tile SlidingTileTransitions::getMovingTile(const SlidingTileState& state, const BlankSlide& action) const {
    if (action == BlankSlide::up) {
        return state.m_permutation[state.m_blank_loc - m_num_cols];
    } else if (action == BlankSlide::right) {
        return state.m_permutation[state.m_blank_loc + 1];
    } else if (action == BlankSlide::down) {
        return state.m_permutation[state.m_blank_loc + m_num_cols];
    } else if (action == BlankSlide::left) {
        return state.m_permutation[state.m_blank_loc - 1];
    }
    return 0;
}

std::optional<BlankSlide> SlidingTileTransitions::getInverse(const SlidingTileState&, const BlankSlide& action) const {
    if (action == BlankSlide::up) {
        return BlankSlide::down;
    } else if (action == BlankSlide::right) {
        return BlankSlide::left;
    } else if (action == BlankSlide::down) {
        return BlankSlide::up;
    } else {
        return BlankSlide::right;
    }
}

bool SlidingTileTransitions::isValidState(const SlidingTileState& state) const {
    return (state.m_num_cols == m_num_cols && state.m_num_rows == m_num_rows) &&
           isValidPermutation(state.m_permutation);
}
void SlidingTileTransitions::setPuzzleDimensions(int rows, int cols) {
    assert(rows > 0 && cols > 0);

    m_num_rows = rows;
    m_num_cols = cols;
    setActionList();
}

StringMap SlidingTileTransitions::getComponentSettings() const {
    using namespace slidingTileNames;

    return {{SETTING_COST_TYPE, streamableToString(m_cost_type)},
              {SETTING_NUM_ROWS, std::to_string(m_num_rows)},
              {SETTING_NUM_COLS, std::to_string(m_num_cols)}};
}
