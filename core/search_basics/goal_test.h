#ifndef GOAL_TEST_H_
#define GOAL_TEST_H_

#include "logging/settings_logger.h"

/**
 * Abstract class for defining a goal test function.
 *
 * @class GoalTest
 */
template<class State_t>
class GoalTest : public SettingsLogger {
public:
    /**
     * Performs the goal test on the given state.
     *
     * @param state The state to test.
     * @return If the given state is a goal state.
     */
    virtual bool isGoal(const State_t& state) const = 0;
};

#endif /* GOAL_TEST_H_ */
