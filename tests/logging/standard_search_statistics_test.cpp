#include <gtest/gtest.h>

#include "logging/standard_search_statistics.h"

/**
 * Checks that reset works properly.
 */
TEST(SearchStatisticsTests, resetTest) {
    StandardSearchStatistics stats;

    stats.m_num_evals = 105;
    stats.m_num_get_actions_calls = 904;
    stats.m_num_goal_tests = 3450;
    stats.m_num_actions_generated = 87501;
    stats.m_num_states_generated = 345601;
    stats.m_search_time_seconds = 75.678;

    stats.reset();

    ASSERT_EQ(stats.m_num_evals, 0);
    ASSERT_EQ(stats.m_num_get_actions_calls, 0);
    ASSERT_EQ(stats.m_num_goal_tests, 0);
    ASSERT_EQ(stats.m_num_actions_generated, 0);
    ASSERT_EQ(stats.m_num_states_generated, 0);
    ASSERT_DOUBLE_EQ(stats.m_search_time_seconds, 0.0);
}
/**
* Checks that StatsLog works properly
*/
TEST(SearchStatisticsTests, logTest) {
    using namespace standardSearchStatisticsTerms;
    StandardSearchStatistics stats;

    stats.m_num_evals = 105;
    stats.m_num_get_actions_calls = 904;
    stats.m_num_goal_tests = 3450;
    stats.m_num_actions_generated = 87501;
    stats.m_num_states_generated = 345601;
    stats.m_search_time_seconds = 75.678572352;

    StringMap log = stats.getStatsLog();

    ASSERT_EQ(log.at(STAT_NUM_EVALS), "105");
    ASSERT_EQ(log.at(STAT_NUM_GET_ACTION_CALLS), "904");
    ASSERT_EQ(log.at(STAT_NUM_GOAL_TESTS), "3450");
    ASSERT_EQ(log.at(STAT_NUM_ACTIONS_GENERATED), "87501");
    ASSERT_EQ(log.at(STAT_NUM_STATES_GENERATED), "345601");
    ASSERT_EQ(log.at(STAT_SEARCH_TIME_SECONDS), "75.6786");
}