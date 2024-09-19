#ifndef A_STAR_EPSILON_PARAMS_H_
#define A_STAR_EPSILON_PARAMS_H_

#include "logging/logging_terms.h"
#include "utils/string_utils.h"
#include <unordered_map>

/**
 * The parameters for an A Star Epsilon engine
 */
struct AStarEpsilonParams {
    /**
     * Returns a map containing the log paramater that use in A Star Epsilon
     * @return A map to stand for the Log of paramas
     */
    StringMap getParameterLog() const;

    double m_weight = 1;  ///< The weight
    bool m_use_reopened = true;  ///< Whether we are reopening closed nodes
    bool m_parent_heuristic_updating = false;  ///< Whether or not to use pathmax
    bool m_store_expansion_order = false;  ///< Whether we want to store the order of node expansions
};

#endif  //A_STAR_EPSILON_PARAMS_H_