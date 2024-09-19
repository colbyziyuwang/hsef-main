#ifndef SINGLE_GOAL_STATE_EVALUATOR_H_
#define SINGLE_GOAL_STATE_EVALUATOR_H_

/**
 * An interface for evaluators that are specific to some single goal state. Defines a standardized way to set
 * that goal state in that case.
 */
template<class State_t>
class SingleGoalStateEvaluator {
public:
    /**
     * Updates the evaluator to be specific to this goal state
     *
     * @param goal_state The goal state.
     */
    virtual void setGoalState(const State_t& goal_state) = 0;

    /**
     * Returns the goal state.
     *
     * @return The goal state
     */
    virtual State_t getGoalState() const = 0;
};

#endif  //SINGLE_GOAL_STATE_EVALUATOR_H_
