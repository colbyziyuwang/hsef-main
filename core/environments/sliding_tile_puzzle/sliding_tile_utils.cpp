#include "sliding_tile_utils.h"
#include "sliding_tile_names.h"
#include "sliding_tile_state.h"
#include "sliding_tile_transitions.h"
#include "utils/combinatorics.h"
#include "utils/io_utils.h"

#include <ostream>
#include <string>
#include <vector>

std::vector<double> getTileMoveCosts(int puzzle_size, SlidingTileCostType cost_type) {
    std::vector<double> tile_costs(puzzle_size);

    tile_costs[0] = 0;
    for (int i = 1; i < puzzle_size; i++) {
        if (cost_type == SlidingTileCostType::unit) {
            tile_costs[i] = 1.0;
        } else if (cost_type == SlidingTileCostType::heavy) {
            tile_costs[i] = i;
        } else if (cost_type == SlidingTileCostType::inverse) {
            tile_costs[i] = 1.0 / (static_cast<double>(i));
        }
    }
    return tile_costs;
}

std::ostream& operator<<(std::ostream& out, const SlidingTileCostType& cost_type) {
    switch (cost_type) {
        case SlidingTileCostType::unit:
            out << slidingTileNames::COST_UNIT;
            break;
        case SlidingTileCostType::heavy:
            out << slidingTileNames::COST_HEAVY;
            break;
        case SlidingTileCostType::inverse:
            out << slidingTileNames::COST_INVERSE;
            break;
    }
    return out;
}

std::vector<SlidingTileState> readSlidingTileStatesFromFile(const std::string& filename, int num_rows, int num_cols) {
    auto perm_stream = loadFileIntoStringSteam(filename);
    auto permutations = readPermutations(perm_stream, false);

    std::vector<SlidingTileState> start_states;
    start_states.reserve(permutations.size());

    for (const auto& perm : permutations) {
        start_states.emplace_back(perm, num_rows, num_cols);
    }

    return start_states;
}
