#include <algorithm>
#include <cassert>
#include <iostream>
#include <iterator>
#include <numeric>
#include <ostream>
#include <vector>

#include "sliding_tile_state.h"

using std::cerr;
using std::find;
using std::iota;
using std::vector;


SlidingTileState::SlidingTileState(int rows, int cols)
          : m_permutation(rows * cols), m_num_rows(rows), m_num_cols(cols) {
    assert(rows > 0 && cols > 0);
    iota(m_permutation.begin(), m_permutation.end(), 0);
}

SlidingTileState::SlidingTileState(const std::vector<Tile>& permutation, int rows, int cols)
          : m_permutation(permutation), m_num_rows(rows), m_num_cols(cols) {
    if (m_permutation.size() != static_cast<unsigned>(m_num_rows * m_num_cols)) {
        cerr << "Error in SlidingTileState constructor.";
        cerr << "Permutation isn't the right size for given parameters.\n";
    }

    auto loc = find(m_permutation.begin(), m_permutation.end(), 0);
    assert(loc != m_permutation.end());
    m_blank_loc = static_cast<int>(std::distance(m_permutation.begin(), loc));
}

std::ostream& operator<<(std::ostream& out, const SlidingTileState& state) {
    out << "(" << state.m_num_rows << "x" << state.m_num_cols << ")-[";
    for (unsigned i = 0; i < state.m_permutation.size(); i++) {
        out << state.m_permutation[i];
        if (i > 1 && i < state.m_permutation.size() - 1 && (i + 1) % state.m_num_cols == 0) {
            out << ",";
        }
        if (i < state.m_permutation.size() - 1) {
            out << " ";
        }
    }
    out << "]";
    return out;
}

bool operator==(const SlidingTileState& state1, const SlidingTileState& state2) {
    return state1.m_permutation == state2.m_permutation;
}

bool operator!=(const SlidingTileState& state1, const SlidingTileState& state2) {
    return !(state1 == state2);
}
