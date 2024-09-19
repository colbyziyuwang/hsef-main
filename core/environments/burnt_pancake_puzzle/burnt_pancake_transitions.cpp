#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <numeric>
#include <optional>
#include <string>
#include <vector>

#include "burnt_pancake_state.h"
#include "burnt_pancake_transitions.h"
#include "environments/pancake_puzzle/pancake_action.h"
#include "environments/pancake_puzzle/pancake_names.h"
#include "environments/pancake_puzzle/pancake_transitions.h"
#include "environments/pancake_puzzle/pancake_utils.h"
#include "logging/logging_terms.h"
#include "utils/combinatorics.h"
#include "utils/string_utils.h"

BurntPancakeTransitions::BurntPancakeTransitions(int size, PancakePuzzleCostType cost_type)
          : m_stack_size(size), m_cost_type(cost_type) {
    assert(size > 0);
}

bool BurntPancakeTransitions::isApplicable(const BurntPancakeState&, const NumToFlip& action) const {
    return (action >= 1 && action <= m_stack_size);
}

double BurntPancakeTransitions::getActionCost(const BurntPancakeState& state, const NumToFlip& action) const {
    if (m_cost_type == PancakePuzzleCostType::heavy) {
        return std::max(abs(state.m_permutation[0]), abs(state.m_permutation[action - 1]));
    }
    return 1.0;
}

void BurntPancakeTransitions::applyAction(BurntPancakeState& state, const NumToFlip& action) const {
    std::reverse(state.m_permutation.begin(), state.m_permutation.begin() + action);
    auto iter = state.m_permutation.begin();
    while (iter != state.m_permutation.begin() + action) {
        *iter++ *= -1;
    }
}

std::vector<int> BurntPancakeTransitions::getActions(const BurntPancakeState&) const {
    std::vector<int> new_actions(m_stack_size);
    std::iota(new_actions.begin(), new_actions.end(), 1);
    return new_actions;
}

std::optional<NumToFlip> BurntPancakeTransitions::getInverse(const BurntPancakeState&, const NumToFlip& action) const {
    return action;
}

bool BurntPancakeTransitions::isValidState(const BurntPancakeState& state) const {
    return m_stack_size == static_cast<int>(state.m_permutation.size()) &&
           isValidSignedPermutation(state.m_permutation);
}

void BurntPancakeTransitions::setTileCostType(const PancakePuzzleCostType cost_type) {
    m_cost_type = cost_type;
}

StringMap BurntPancakeTransitions::getComponentSettings() const {
    using namespace pancakeNames;

    return {
              {SETTING_STACK_SIZE, std::to_string(m_stack_size)},
              {SETTING_COST_TYPE, streamableToString(m_cost_type)}};
}
