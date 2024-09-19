#include "pancake_transitions.h"
#include "logging/logging_terms.h"
#include "pancake_action.h"
#include "pancake_names.h"
#include "pancake_state.h"
#include "pancake_utils.h"
#include "utils/combinatorics.h"
#include "utils/string_utils.h"

#include <algorithm>
#include <cassert>
#include <numeric>
#include <optional>
#include <string>
#include <vector>

PancakeTransitions::PancakeTransitions(int size, PancakePuzzleCostType cost_type)
          : m_stack_size(size), m_cost_type(cost_type) {
    assert(size > 0);
}

bool PancakeTransitions::isApplicable(const PancakeState&, const NumToFlip& action) const {
    return (action >= 2 && action <= m_stack_size);
}

double PancakeTransitions::getActionCost(const PancakeState& state, const NumToFlip& action) const {
    if (m_cost_type == PancakePuzzleCostType::heavy) {
        return 1.0 + std::max(state.m_permutation[0], state.m_permutation[action - 1]);
    }
    return 1.0;
}

void PancakeTransitions::applyAction(PancakeState& state, const NumToFlip& action) const {
    std::reverse(state.m_permutation.begin(), state.m_permutation.begin() + action);
}

std::vector<NumToFlip> PancakeTransitions::getActions(const PancakeState&) const {
    std::vector<NumToFlip> new_actions(m_stack_size - 1);
    std::iota(new_actions.begin(), new_actions.end(), 2);
    return new_actions;
}

std::optional<NumToFlip> PancakeTransitions::getInverse(const PancakeState&, const NumToFlip& action) const {
    return action;
}

bool PancakeTransitions::isValidState(const PancakeState& state) const {
    return m_stack_size == static_cast<int>(state.m_permutation.size()) && isValidPermutation(state.m_permutation);
}

void PancakeTransitions::setTileCostType(const PancakePuzzleCostType cost_type) {
    m_cost_type = cost_type;
}

StringMap PancakeTransitions::getComponentSettings() const {
    using namespace pancakeNames;

    return {{SETTING_STACK_SIZE, std::to_string(m_stack_size)},
              {SETTING_COST_TYPE, streamableToString(m_cost_type)}};
}
