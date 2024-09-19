#include "search_resource_limits.h"
#include "logging/logging_terms.h"
#include "logging/standard_search_statistics.h"
#include "utils/string_utils.h"
#include "utils/timer.h"

#include <cstdint>
#include <string>

bool SearchResourceLimits::hasHitStorageLimit(int64_t current_storage) const {
    return (m_state_storage_limit > 0 && current_storage >= m_state_storage_limit);
}

bool SearchResourceLimits::hasHitNumEvalLimit(const StandardSearchStatistics& stats) const {
    return (m_node_eval_limit > 0 && stats.m_num_evals >= m_node_eval_limit);
}

bool SearchResourceLimits::hasHitGetActionsCallLimit(const StandardSearchStatistics& stats) const {
    return (m_get_actions_call_limit > 0 && stats.m_num_get_actions_calls >= m_get_actions_call_limit);
}

bool SearchResourceLimits::hasHitGoalTestLimit(const StandardSearchStatistics& stats) const {
    return (m_goal_test_limit > 0 && stats.m_num_goal_tests >= m_goal_test_limit);
}

bool SearchResourceLimits::hasHitStateGenerationLimit(const StandardSearchStatistics& stats) const {
    return (m_state_generation_limit > 0 && stats.m_num_states_generated >= m_state_generation_limit);
}

bool SearchResourceLimits::hasHitTimeLimit(const Timer& timer) const {
    return (m_time_limit_seconds > 0.0 && timer.getCurrentTimePeriodDuration() >= m_time_limit_seconds);
}

StringMap SearchResourceLimits::getComponentSettings() const {
    using namespace searchResourceLimitsNames;

    StringMap resource_limits;
    resource_limits[RL_STATE_STORAGE_LIMIT] = std::to_string(m_state_storage_limit);
    resource_limits[RL_M_NODE_EVAL_LIMIT] = std::to_string(m_node_eval_limit);
    resource_limits[RL_M_GET_ACTIONS_CALL_LIMIT] = std::to_string(m_get_actions_call_limit);
    resource_limits[RL_M_GOAL_TEST_LIMIT] = std::to_string(m_goal_test_limit);
    resource_limits[RL_M_STATE_GENERATION_LIMIT] = std::to_string(m_state_generation_limit);
    resource_limits[RL_TIME_LIMIT_SECONDS] = roundAndToString(m_time_limit_seconds, 4);
    resource_limits[RL_M_TIMER_CHECK_FREQUENCY] = std::to_string(m_timer_check_frequency);
    return resource_limits;
}