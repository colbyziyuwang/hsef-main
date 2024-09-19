#ifndef STANDARD_SEARCH_STATISTICS_H_
#define STANDARD_SEARCH_STATISTICS_H_

#include <cstdint>
#include <string>

#include "logging/logging_terms.h"

/**
 * Stores the standard search statistics related to the run of a search engine.
 */
struct StandardSearchStatistics {

    /**
     * Constructor initializes all values to 0.
     */
    StandardSearchStatistics() = default;

    /**
     * Gets a log of the statistics for the current search. This is a map, where the stat name is the key and the value of each
     * stat as a string is the value.
     *
     * @return The log of the statistics for the search.
     */
    StringMap getStatsLog() const;

    int64_t m_num_evals = 0;  ///< The number of node evaluations performed
    int64_t m_num_get_actions_calls = 0;  ///< The number of get action calls made to the transition function
    int64_t m_num_goal_tests = 0;  ///< The number of goal tests performed
    int64_t m_num_actions_generated = 0;  ///< The number of actions generated
    int64_t m_num_states_generated = 0;  ///< The number of states generated
    double m_search_time_seconds = 0;  ///< The search time seconds

    /**
     * Resets all values to 0.
     */
    void reset();
};

/**
 * Names for the search statistics
 */
namespace standardSearchStatisticsTerms {
    inline const std::string STAT_NUM_EVALS = "num_evals";  ///< The string for number of evaluations
    inline const std::string STAT_NUM_GET_ACTION_CALLS = "num_get_actions_calls";  ///< The string for number of getAction calls
    inline const std::string STAT_NUM_GOAL_TESTS = "num_goal_tests";  ///< The string for number of goal tests
    inline const std::string STAT_NUM_ACTIONS_GENERATED = "num_actions_generated";  ///< The string for number of actions generated
    inline const std::string STAT_NUM_STATES_GENERATED = "num_states_generated";  ///< The string for number of states generated
    inline const std::string STAT_SEARCH_TIME_SECONDS = "search_time_seconds";  ///< The string for the time in seconds
}  // namespace standardSearchStatisticsTerms

#endif  //STANDARD_SEARCH_STATISTICS_H_
