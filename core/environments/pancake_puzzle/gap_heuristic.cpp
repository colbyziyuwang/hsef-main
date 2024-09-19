#include "gap_heuristic.h"
#include "building_tools/evaluators/node_evaluator_with_cache.h"
#include "logging/logging_terms.h"
#include "pancake_names.h"
#include "pancake_state.h"
#include "pancake_transitions.h"
#include "pancake_utils.h"
#include "search_basics/node_container.h"
#include "utils/string_utils.h"

#include <algorithm>
#include <cassert>
#include <cstdlib>


GapHeuristic::GapHeuristic(PancakePuzzleCostType cost_type)
          : m_cost_type(cost_type) {
}

void GapHeuristic::doEvaluateAndCache(NodeID to_evaluate) {
    const PancakeState& state = getNodeContainer()->getState(to_evaluate);
    double num_gaps = 0.0;
    double extra_weight = 0.0;  // added action costs due to pancake weighting

    auto num_pancakes = static_cast<unsigned>(state.m_permutation.size());

    for (unsigned i = 0; i < num_pancakes - 1; i++) {
        if (abs(state.m_permutation[i] - state.m_permutation[i + 1]) > 1) {
            num_gaps += 1.0;

            if (m_cost_type == PancakePuzzleCostType::heavy) {  // add the heavy part if it is necessary
                extra_weight += std::min(state.m_permutation[i], state.m_permutation[i + 1]);
            }
        }
    }

    if (abs(state.m_permutation[num_pancakes - 1] - static_cast<int>(num_pancakes)) > 1) {
        num_gaps += 1.0;
        if (m_cost_type == PancakePuzzleCostType::heavy) {  // add the heavy part if it is necessary
            extra_weight += state.m_permutation[num_pancakes - 1];
        }
    }
    setCachedDistanceToGoEval(to_evaluate, num_gaps);
    setCachedValues(to_evaluate, num_gaps + extra_weight, false);
}

double GapHeuristic::getCachedDistanceToGoEval(NodeID node_id) const {
    assert(node_id < m_distance_to_go_evals.size());
    return m_distance_to_go_evals[node_id];
}

void GapHeuristic::setCachedDistanceToGoEval(NodeID node_id, double eval) {
    if (node_id >= m_distance_to_go_evals.size()) {
        m_distance_to_go_evals.resize(node_id + 1, 0.0);
    }
    m_distance_to_go_evals[node_id] = eval;
}

double GapHeuristic::getLastDistanceToGoEval() const {
    using NE = NodeEvaluatorWithCache<PancakeState, NumToFlip>;
    assert(NE::isEvalComputed());

    return m_distance_to_go_evals[NE::getIDofLastEvaluatedNode()];
}

StringMap GapHeuristic::getComponentSettings() const {
    using namespace pancakeNames;

    return {{SETTING_COST_TYPE, streamableToString(m_cost_type)}};
}
