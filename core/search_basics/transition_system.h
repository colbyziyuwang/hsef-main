#ifndef TRANSITIONSYSTEM_H_
#define TRANSITIONSYSTEM_H_

#include "logging/settings_logger.h"

#include <cassert>
#include <optional>
#include <vector>


/**
 * An abstract class defining the way transition systems are to be applied.
 *
 * @class TransitionSystem
 */
template<class State_t, class Action_t>
class TransitionSystem : public SettingsLogger {
public:
    /**
     * Gets the actions applicable in a given state and appends them to the given list.
     *
     * @param state The state to generate actions in.
     * @return The list of applicable actions
     */
    virtual std::vector<Action_t> getActions(const State_t& state) const = 0;

    /**
     * Checks if the action is applicable in the given state.
     *
     * @param state The state in the domain.
     * @param action The action to test for applicability.
     * @return If the given action is applicable in the given state.
     */
    virtual bool isApplicable(const State_t& state, const Action_t& action) const = 0;

    /**
     * Applies the given action to the given state.
     *
     * Assumes the given action is applicable in the given state.
     *
     * @param state The state to update.
     * @param action The action to apply.
     */
    virtual void applyAction(State_t& state, const Action_t& action) const = 0;

    /**
     * Returns the cost of applying the given action in the given state.
     *
     * Assumes the action is actually applicable in that state.
     *
     * @param state The state to apply the action in.
     * @param action The action to apply.
     * @return The cost of applying the action in the given state.
     */
    virtual double getActionCost(const State_t& state, const Action_t& action) const = 0;

    /**
     * Returns the inverse of a given action.
     *
     * If no inverse exists, returns std::nullopt
     *
     * @param state The state the action is being applied in.
     * @param action The action to invert.
     * @return The inverse of the given action or std::nullopt.
     */
    virtual std::optional<Action_t> getInverse(const State_t& state, const Action_t& action) const = 0;

    /**
     * Checks if the given next action is the inverse of last action from the given state. This means that if last
     * action was applied to the last state to generate the current state, then next action will result back in the last
     * state.
     *
     * @param state The current state.
     * @param next_action The next action to apply apply.
     * @param last_action The action used to generate the current state.
     * @return If the next action is the inverse of last action from the given state.
     */
    virtual bool isInverseAction(const State_t& state, const Action_t& next_action, const Action_t& last_action) const;

    /**
     * Checks if the given state is valid with respect to this transition function.
     *
     * Largely used for debugging.
     *
     * @param state The state whose validity is to be checked.
     * @return If the transition system can be applied to the given state.
     */
    virtual bool isValidState(const State_t& state) const = 0;

    /**
     * Gets the child state that is the result of applying the given action in the given state.
     *
     * Assumes the action is applicable in the given state.
     *
     * @param state The parent state.
     * @param action The action to apply to the parent state.
     * @return The outcome of applying the action to the parent state.
     */
    virtual State_t getChildState(const State_t& state, const Action_t& action) const;
};

template<class State_t, class Action_t>
State_t TransitionSystem<State_t, Action_t>::getChildState(const State_t& state, const Action_t& action) const {
    assert(isApplicable(state, action));

    State_t child = state;
    applyAction(child, action);
    return child;
}

template<class State_t, class Action_t>
bool TransitionSystem<State_t, Action_t>::isInverseAction(const State_t& state, const Action_t& next_action,
          const Action_t& last_action) const {
    std::optional<Action_t> action = getInverse(state, next_action);
    return (action.has_value() && action == last_action);
}

#endif /* TRANSITIONSYSTEM_H_ */
