#include "burnt_gap_heuristic.h"
#include "burnt_pancake_state.h"
#include "environments/pancake_puzzle/pancake_names.h"
#include "environments/pancake_puzzle/pancake_transitions.h"
#include "environments/pancake_puzzle/pancake_utils.h"
#include "logging/logging_terms.h"
#include "search_basics/node_container.h"
#include "utils/string_utils.h"

#include <algorithm>
#include <cassert>
#include <cstdlib>

BurntGapHeuristic::BurntGapHeuristic(PancakePuzzleCostType cost_type)
          : m_cost_type(cost_type) {
}

void BurntGapHeuristic::doEvaluateAndCache(NodeID to_evaluate) {
    const BurntPancakeState& state = getNodeContainer()->getState(to_evaluate);
    double num_gaps = 0.0;
    double total_cost = 0.0;
    auto num_pancakes = static_cast<unsigned>(state.m_permutation.size());

    for (unsigned i = 0; i < num_pancakes - 1; i++) {
        if (abs(state.m_permutation[i] - state.m_permutation[i + 1]) > 1) {
            if (m_cost_type == PancakePuzzleCostType::heavy) {
                total_cost += std::min(abs(state.m_permutation[i]), abs(state.m_permutation[i + 1]));
            }
            num_gaps++;
        }
    }

    if (abs(state.m_permutation[num_pancakes - 1] - (static_cast<int>(num_pancakes) + 1)) > 1) {
        if (m_cost_type == PancakePuzzleCostType::heavy) {
            total_cost += abs(state.m_permutation[num_pancakes - 1]);
        }
        num_gaps++;
    }

    setCachedDistanceToGoEval(to_evaluate, num_gaps);
    if (m_cost_type == PancakePuzzleCostType::heavy) {
        setCachedValues(to_evaluate, total_cost, false);
    } else {
        setCachedValues(to_evaluate, num_gaps, false);
    }
}

double BurntGapHeuristic::getCachedDistanceToGoEval(NodeID node_id) const {
    assert(node_id < m_distance_to_go_evals.size());
    return m_distance_to_go_evals[node_id];
}

void BurntGapHeuristic::setCachedDistanceToGoEval(NodeID node_id, double eval) {
    if (node_id >= m_distance_to_go_evals.size()) {
        m_distance_to_go_evals.resize(node_id + 1, 0.0);
    }
    m_distance_to_go_evals[node_id] = eval;
}

double BurntGapHeuristic::getLastDistanceToGoEval() const {
    assert(isEvalComputed());

    return m_distance_to_go_evals[getIDofLastEvaluatedNode()];
}

StringMap BurntGapHeuristic::getComponentSettings() const {
    using namespace pancakeNames;

    return {{SETTING_COST_TYPE, streamableToString(m_cost_type)}};
}
