#include <chrono>
#include <gtest/gtest.h>
#include <thread>

#include "experiment_running/search_resource_limits.h"
#include "utils/timer.h"


/**
 * Tests that when the limits are all 0, the limits are not set as hit.
 */
TEST(SearchResourceLimitsTests, hitLimitTestsNoLimitTest) {
    SearchResourceLimits limits;

    ASSERT_FALSE(limits.hasHitStorageLimit(750));

    StandardSearchStatistics stats;
    stats.m_num_evals = 75;
    stats.m_num_get_actions_calls = 62;
    stats.m_num_goal_tests = 124;
    stats.m_num_actions_generated = 89;
    stats.m_num_states_generated = 56;

    ASSERT_FALSE(limits.hasHitNumEvalLimit(stats));
    ASSERT_FALSE(limits.hasHitGetActionsCallLimit(stats));
    ASSERT_FALSE(limits.hasHitGoalTestLimit(stats));

    ASSERT_FALSE(limits.hasHitStateGenerationLimit(stats));

    Timer timer;
    timer.startTimer();
    std::chrono::milliseconds dura(10);
    std::this_thread::sleep_for(dura);  // Sleep for 10 ms

    ASSERT_FALSE(limits.hasHitTimeLimit(timer));
}

/**
 * Tests that when the limits are all nonzero, they are hit as needed.
 */
TEST(SearchResourceLimitsTests, hitLimitTestsWithLimitsTest) {
    SearchResourceLimits limits;
    limits.m_state_storage_limit = 100;
    limits.m_node_eval_limit = 200;
    limits.m_get_actions_call_limit = 50;
    limits.m_goal_test_limit = 500;
    limits.m_state_generation_limit = 300;
    limits.m_time_limit_seconds = 0.03;

    ASSERT_TRUE(limits.hasHitStorageLimit(750));
    ASSERT_FALSE(limits.hasHitStorageLimit(20));

    StandardSearchStatistics stats1;
    stats1.m_num_evals = 75;
    stats1.m_num_get_actions_calls = 62;
    stats1.m_num_goal_tests = 124;
    stats1.m_num_actions_generated = 89;
    stats1.m_num_states_generated = 56;

    StandardSearchStatistics stats2;
    stats2.m_num_evals = 750;
    stats2.m_num_get_actions_calls = 6;
    stats2.m_num_goal_tests = 1245;
    stats2.m_num_actions_generated = 8;
    stats2.m_num_states_generated = 567;

    ASSERT_FALSE(limits.hasHitNumEvalLimit(stats1));
    ASSERT_TRUE(limits.hasHitNumEvalLimit(stats2));

    ASSERT_TRUE(limits.hasHitGetActionsCallLimit(stats1));
    ASSERT_FALSE(limits.hasHitGetActionsCallLimit(stats2));

    ASSERT_FALSE(limits.hasHitGoalTestLimit(stats1));
    ASSERT_TRUE(limits.hasHitGoalTestLimit(stats2));

    ASSERT_FALSE(limits.hasHitStateGenerationLimit(stats1));
    ASSERT_TRUE(limits.hasHitStateGenerationLimit(stats2));

    Timer timer;
    std::chrono::milliseconds dura1(2);
    std::chrono::milliseconds dura2(50);

    timer.startTimer();
    std::this_thread::sleep_for(dura1);  // Sleep for 2 ms
    ASSERT_FALSE(limits.hasHitTimeLimit(timer));

    std::this_thread::sleep_for(dura2);  // Sleep for 50 ms
    ASSERT_TRUE(limits.hasHitTimeLimit(timer));
}

/**
* Checks that getComponentSettings works properly
*/
TEST(SearchResourceLimitsTests, getSettingsTest) {
    using namespace searchResourceLimitsNames;
    SearchResourceLimits rl;

    rl.m_state_storage_limit = 105;
    rl.m_node_eval_limit = 904;
    rl.m_get_actions_call_limit = 3450;
    rl.m_goal_test_limit = 87501;
    rl.m_state_generation_limit = 345601;
    rl.m_time_limit_seconds = 75.678572352;
    rl.m_timer_check_frequency = 12189;

    auto log = rl.getAllSettings();

    ASSERT_EQ(log.m_name, SearchResourceLimits::CLASS_NAME);
    ASSERT_EQ(log.m_main_settings.size(), 7);
    ASSERT_EQ(log.m_main_settings[RL_STATE_STORAGE_LIMIT], "105");
    ASSERT_EQ(log.m_main_settings[RL_M_NODE_EVAL_LIMIT], "904");
    ASSERT_EQ(log.m_main_settings[RL_M_GET_ACTIONS_CALL_LIMIT], "3450");
    ASSERT_EQ(log.m_main_settings[RL_M_GOAL_TEST_LIMIT], "87501");
    ASSERT_EQ(log.m_main_settings[RL_M_STATE_GENERATION_LIMIT], "345601");
    ASSERT_EQ(log.m_main_settings[RL_TIME_LIMIT_SECONDS], "75.6786");
    ASSERT_EQ(log.m_main_settings[RL_M_TIMER_CHECK_FREQUENCY], "12189");
    ASSERT_EQ(log.m_sub_component_settings.size(), 0);
}