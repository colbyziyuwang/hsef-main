#include <algorithm>

#include "k_ary_tree_names.h"
#include "k_ary_tree_transitions.h"
#include "utils/combinatorics.h"

KAryTreeTransitions::KAryTreeTransitions(int k)
          : m_k(k) {
}

std::vector<KAryTreeAction> KAryTreeTransitions::getActions(const KAryTreeState&) const {
    std::vector<KAryTreeAction> actions(m_k);
    std::iota(actions.begin(), actions.end(), 0);
    return actions;
}

bool KAryTreeTransitions::isApplicable(const KAryTreeState&, const KAryTreeAction& action) const {
    if (action < 0 || action >= m_k) {
        return false;
    }
    return true;
}

double KAryTreeTransitions::getActionCost(const KAryTreeState&, const KAryTreeAction&) const {
    return 1.0;
}

void KAryTreeTransitions::applyAction(KAryTreeState& state, const KAryTreeAction& action) const {
    state.m_actions.push_back(action);
}

std::optional<KAryTreeAction> KAryTreeTransitions::getInverse(const KAryTreeState&, const KAryTreeAction&) const {
    return std::nullopt;
}

bool KAryTreeTransitions::isValidState(const KAryTreeState& state) const {
    for (const KAryTreeAction& action : state.m_actions) {
        if (action < 0 || action > m_k) {
            return false;
        }
    }
    return true;
}

StringMap KAryTreeTransitions::getComponentSettings() const {
    using namespace kAryTreeNames;
    StringMap settings;
    settings.emplace(kAryTreeNames::SETTING_BRANCHING_FACTOR, std::to_string(m_k));
    return settings;
}
