#include "logging/standard_search_statistics.h"
#include "utils/string_utils.h"

void StandardSearchStatistics::reset() {
    m_num_evals = 0;
    m_num_get_actions_calls = 0;
    m_num_goal_tests = 0;
    m_num_actions_generated = 0;
    m_num_states_generated = 0;
    m_search_time_seconds = 0;
}

StringMap StandardSearchStatistics::getStatsLog() const {
    using namespace standardSearchStatisticsTerms;

    StringMap stats;
    stats[STAT_NUM_EVALS] = std::to_string(m_num_evals);
    stats[STAT_NUM_GET_ACTION_CALLS] = std::to_string(m_num_get_actions_calls);
    stats[STAT_NUM_GOAL_TESTS] = std::to_string(m_num_goal_tests);
    stats[STAT_NUM_ACTIONS_GENERATED] = std::to_string(m_num_actions_generated);
    stats[STAT_NUM_STATES_GENERATED] = std::to_string(m_num_states_generated);
    stats[STAT_SEARCH_TIME_SECONDS] = roundAndToString(m_search_time_seconds, 4);
    return stats;
}