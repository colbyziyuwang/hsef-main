#ifndef SINGLE_STATE_GOAL_TEST_H_
#define SINGLE_STATE_GOAL_TEST_H_

#include "building_tools/evaluators/single_goal_state_evaluator.h"
#include "building_tools/goal_tests/goal_tests_tools_terms.h"
#include "logging/logging_terms.h"
#include "logging/search_component_settings.h"
#include "search_basics/goal_test.h"
#include "utils/string_utils.h"

#include <string>

/**
 * Defines a goal test function when there is a single goal state.
 *
 * @class SingleStateGoalTest
 */
template<class State_t>
class SingleStateGoalTest : public GoalTest<State_t>, public SingleGoalStateEvaluator<State_t> {
public:
    inline static const std::string CLASS_NAME = "SingleStateGoalTest";  ///< The name of the class. Defines this component's name
    /**
     * Constructs a goal test function where there is a given single goal state.
     *
     * @param goal_state The goal state.
     */
    explicit SingleStateGoalTest(const State_t& goal_state);

    // Overriden GoalTest functions
    bool isGoal(const State_t& state) const override;

    // Overriden SingleGoalStateComponent functions
    void setGoalState(const State_t& goal_state) override;
    State_t getGoalState() const override;

    std::string getName() const override { return CLASS_NAME; }

protected:
    StringMap getComponentSettings() const override;
    SearchSettingsMap getSubComponentSettings() const override;

private:
    State_t m_goal_state;  ///< The single goal state
};

template<class State_t>
SingleStateGoalTest<State_t>::SingleStateGoalTest(const State_t& goal_state)
          : m_goal_state(goal_state) {
}

template<class State_t>
bool SingleStateGoalTest<State_t>::isGoal(const State_t& state) const {
    return m_goal_state == state;
}

template<class State_t>
void SingleStateGoalTest<State_t>::setGoalState(const State_t& goal_state) {
    m_goal_state = goal_state;
}

template<class State_t>
State_t SingleStateGoalTest<State_t>::getGoalState() const {
    return m_goal_state;
}

template<class State_t>
StringMap SingleStateGoalTest<State_t>::getComponentSettings() const {
    StringMap settings;
    settings[goalTestToolsTerms::SETTING_GOAL_STATE] = streamableToString(m_goal_state);
    return settings;
}

template<class State_t>
SearchSettingsMap SingleStateGoalTest<State_t>::getSubComponentSettings() const {
    return {};
}

#endif /* SINGLE_STATE_GOAL_TEST_H_ */
