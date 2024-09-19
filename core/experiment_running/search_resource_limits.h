#ifndef SEARCH_RESOURCE_LIMITS_H_
#define SEARCH_RESOURCE_LIMITS_H_

#include "logging/logging_terms.h"
#include "logging/search_component_settings.h"
#include "logging/settings_logger.h"
#include "logging/standard_search_statistics.h"
#include "utils/timer.h"

#include <cstdint>
#include <string>

/**
 * The resource limits on search. Limits of 0 (or negative) mean there is no limit.
 */
struct SearchResourceLimits : public SettingsLogger {

    inline static const std::string CLASS_NAME = "SearchResourceLimits";  ///< The name of this class

    /**
     * Constructs the limit struct. All limits are set
     */
    SearchResourceLimits() = default;

    /**
     * Checks if the current storage reaches or exceeds the current storage limit.
     *
     * @param current_storage The current number of states in memory
     * @return Whether the storage limit has been hit or not
     */
    bool hasHitStorageLimit(int64_t current_storage) const;

    /**
     * Checks if the limit on the number of node evaluations have been reached.
     *
     * @param stats The statistics for the current search run
     * @return Whether the node evaluation call limit has been reached
     */
    bool hasHitNumEvalLimit(const StandardSearchStatistics& stats) const;

    /**
     * Checks if the limit on the number of transition function calls of getActions have been reached.
     *
     * @param stats The statistics for the current search run
     * @return Whether the transition function call limit has been reached
     */
    bool hasHitGetActionsCallLimit(const StandardSearchStatistics& stats) const;

    /**
     * Checks if the limit on the number of goal tests has been reached.
     *
     * @param stats The statistics for the current search run
     * @return Whether the goal test limit has been reached
     */
    bool hasHitGoalTestLimit(const StandardSearchStatistics& stats) const;

    /**
     * Checks if the limit on the number of states generated has been reached
     *
     * @param stats The statistics for the current search run
     * @return Whether the state generation limit has been reached
     */
    bool hasHitStateGenerationLimit(const StandardSearchStatistics& stats) const;

    /**
     * Checks if the time limit has been reached.
     *
     * @param timer The timer being used to track search time
     * @return Whether the time limit has been reached
     */
    bool hasHitTimeLimit(const Timer& timer) const;

    int64_t m_state_storage_limit = 0;  ///< The limit on the number of states stored at any time
    int64_t m_node_eval_limit = 0;  ///< The limit on the number of evaluations made (does not count sub-heuristic calls)
    int64_t m_get_actions_call_limit = 0;  ///< The limit on the number of times the transition function getActions function is called
    int64_t m_goal_test_limit = 0;  ///< The limit on the number of goal tests that can be performed
    int64_t m_state_generation_limit = 0;  ///< The limit on the number of states that can be generated
    double m_time_limit_seconds = 0;  ///< The limit on the search time
    int64_t m_timer_check_frequency = 10000;  ///< Frequency of check time

    // Overriden public SettingsLogger method
    std::string getName() const override { return CLASS_NAME; }

protected:
    // Override protected SettingsLogger methods
    StringMap getComponentSettings() const override;
    SearchSettingsMap getSubComponentSettings() const override { return {}; }
};

/**
 * Names for the search resource limits
 */
namespace searchResourceLimitsNames {
    inline const std::string RL_STATE_STORAGE_LIMIT = "state_storage_limit";  ///< The string for the limit on state storage
    inline const std::string RL_M_NODE_EVAL_LIMIT = "node_eval_limit";  ///< The string for the limit of evaluations made
    inline const std::string RL_M_GET_ACTIONS_CALL_LIMIT = "get_actions_call_limit";  ///< The string for the limit of calls to the getActions function
    inline const std::string RL_M_GOAL_TEST_LIMIT = "goal_test_limit";  ///< The string for the limit of goal tests
    inline const std::string RL_M_STATE_GENERATION_LIMIT = "state_generation_limit";  ///< The string for the limit on states generated
    inline const std::string RL_TIME_LIMIT_SECONDS = "time_limit_seconds";  ///< The string for the limit on search time
    inline const std::string RL_M_TIMER_CHECK_FREQUENCY = "timer_check_frequency";  ///< The string for the check time frequency
}  // namespace searchResourceLimitsNames

#endif  //SEARCH_RESOURCE_LIMITS_H_
