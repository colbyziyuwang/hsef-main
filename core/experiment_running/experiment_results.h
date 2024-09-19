#ifndef EXPERIMENT_RESULTS_H_
#define EXPERIMENT_RESULTS_H_

#include "logging/logging_terms.h"
#include "logging/search_component_settings.h"
#include "logging/standard_search_statistics.h"

#include <vector>

/**
 * A log of the result of a experiment.
 *
 * @tparam Action_t The type for actions.
 */
template<class Action_t>
struct ExperimentResults {
    StandardSearchStatistics m_standard_stats;  ///< The standard search statistics
    StringMap m_engine_specific_stats;  ///< The engine specific stats
    std::vector<Action_t> m_plan;  ///< The plan found
    double m_plan_cost = -1;  ///< The cost of the plan found
    bool m_has_found_plan = false;  ///< Whether a plan was found or not
    SearchComponentSettings m_engine_settings;  ///< The engine settings
    SearchComponentSettings m_transitions_settings;  ///< The transition system settings
    SearchComponentSettings m_goal_test_settings;  ///< The goal test settings
};
#endif  // !EXPERIMENT_OUTPUT_H_
