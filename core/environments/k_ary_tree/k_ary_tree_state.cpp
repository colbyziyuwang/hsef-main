#include "k_ary_tree_state.h"

KAryTreeState::KAryTreeState(const std::vector<KAryTreeAction>& actions)
          : m_actions(actions) {
}

std::ostream& operator<<(std::ostream& out, const KAryTreeState& state) {
    // Output the actions in the state
    out << "[";
    for (const auto& action : state.m_actions) {
        out << action << " ";
    }
    out << "]";
    return out;
}

bool operator==(const KAryTreeState& state1, const KAryTreeState& state2) {
    // Compare the actions in the states for equality
    return state1.m_actions == state2.m_actions;
}

bool operator!=(const KAryTreeState& state1, const KAryTreeState& state2) {
    // Compare the actions in the states for inequality
    return !(state1 == state2);
}