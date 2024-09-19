#include "burnt_pancake_state.h"
#include "utils/string_utils.h"

#include <ostream>
#include <vector>

BurntPancakeState::BurntPancakeState(const std::vector<BurntPancake>& permutation)
          : m_permutation(permutation) {
}

std::ostream& operator<<(std::ostream& out, const BurntPancakeState& state) {
    out << vectorToString<int>(state.m_permutation);
    return out;
}

bool operator==(const BurntPancakeState& state1, const BurntPancakeState& state2) {
    return state1.m_permutation == state2.m_permutation;
}

bool operator!=(const BurntPancakeState& state1, const BurntPancakeState& state2) {
    return !(state1 == state2);
}