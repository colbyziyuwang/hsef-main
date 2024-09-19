#include "a_star_epsilon_params.h"

StringMap AStarEpsilonParams::getParameterLog() const {
    StringMap params;

    params["weight"] = roundAndToString(m_weight, 2);
    params["use_reopened"] = boolToString(m_use_reopened);
    params["parent_heuristic_updating"] = boolToString(m_parent_heuristic_updating);
    params["store_expansion_order"] = boolToString(m_store_expansion_order);
    return params;
}