#include "pancake_state.h"
#include "utils/string_utils.h"

#include <ostream>
#include <vector>

PancakeState::PancakeState(const std::vector<Pancake>& permutation)
          : m_permutation(permutation) {
}

std::ostream& operator<<(std::ostream& out, const PancakeState& state) {
    out << vectorToString<int>(state.m_permutation);
    return out;
}

bool operator==(const PancakeState& state1, const PancakeState& state2) {
    return state1.m_permutation == state2.m_permutation;
}

bool operator!=(const PancakeState& state1, const PancakeState& state2) {
    return !(state1 == state2);
}