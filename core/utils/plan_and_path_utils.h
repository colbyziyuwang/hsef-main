#ifndef PLAN_AND_PATH_UTILS_H_
#define PLAN_AND_PATH_UTILS_H_

#include "search_basics/transition_system.h"
#include <optional>
#include <vector>

/**
 * The result of checking a sequence of states for validity or a sequence of actions for validity.
 */
struct SequenceCheckResult {
    bool m_is_valid;  ///< Is the sequence (plan or state sequence) valid or not
    double m_sequence_cost;  ///< The cost of the sequence
};

/**
     * Applies the given plan (ie. sequence of actions) to the given state, according to the given transition system,
     * and returns the plan result (whether the plan successfully applies, and the cost of that plan).
     *
     * If the plan is not entirely applicable. The state will be left at the point immediately prior to the sequence no
     * longer being applicable. The cost returned will be the cost of the applicable part of the sequence.
     *
     * @param transitions The transition system
     * @param state The state to apply the sequence to.
     * @param actions The sequence of actions to apply.
     * @return The result of applying the plan
     */
template<class State_t, class Action_t>
SequenceCheckResult applyPlan(State_t& state, const std::vector<Action_t>& actions,
          const TransitionSystem<State_t, Action_t>& transitions);

/**
 * Checks if the plan can be applied to the given state.
 *
 * @param state The state to apply the plan to
 * @param actions The sequence of actions to apply
 * @param transitions The transition system to use
 * @return The result of checking this plan
 */
template<class State_t, class Action_t>
SequenceCheckResult checkPlan(const State_t& state, const std::vector<Action_t>& actions,
          const TransitionSystem<State_t, Action_t>& transitions);

/**
 * Checkes if the given sequence of states is a valid path of states.
 *
 * If the sequence is invalid, the returned sequence cost is the cost of the prefix of the sequence that is valid.
 *
 * @param states The sequence of states
 * @param transitions The transition system to use
 * @return The result of the sequence check
 */
template<class State_t, class Action_t>
SequenceCheckResult checkStateSequence(const std::vector<State_t>& states,
          const TransitionSystem<State_t, Action_t>& transitions);

/**
 * Checks if the plan is a valid solution plan. The plan is only valid if all actions are applicable and the last state
 * is a goal state.
 *
 * If the plan is not entirely applicable. The state will be left at the point immediately prior to the sequence no
 * longer being applicable. The cost returned will be the cost of the applicable part of the sequence.
 *
 * Having earlier states be goal states does not make it a valid solution plan.
 *
 * @param state The starting state
 * @param actions The plan
 * @param transitions The transition system
 * @param goal The goal test applied to the last state
 * @return The result of the sequence check
 */
template<class State_t, class Action_t>
SequenceCheckResult checkSolutionPlan(const State_t& state, const std::vector<Action_t>& actions,
          const TransitionSystem<State_t, Action_t>& transitions, const GoalTest<State_t>& goal);

/**
 * Checks if the state sequence is a solution path. The path is only valid if all states are children of the previous
 * state, and the last state is a goal state.
 *
 * If the sequence is invalid, the returned sequence cost is the cost of the prefix of the sequence that is valid.
 *
 * Having earlier states be goal states does not make it a valid solution path.
 *
 * @param states
 * @param transitions
 * @param goal
 * @return
 */
template<class State_t, class Action_t>
SequenceCheckResult checkSolutionStateSequence(const std::vector<State_t>& states,
          const TransitionSystem<State_t, Action_t>& transitions,
          const GoalTest<State_t>& goal);

/**
  * Gets the successors of a given state and appends them to the given list.
  *
  * @param transitions The transitions system
  * @param state The state to generate successors for
  * @return The list of successor states.
  */
template<class State_t, class Action_t>
std::vector<State_t> getSuccessors(const TransitionSystem<State_t, Action_t>& transitions, const State_t& state);

/**
 * Gets the action that resulted from the transition from the parent to the child.
 *
 * Returns the dummy action if there is no such transition.
 *
 * @param parent The parent of the transition
 * @param child The child of the transition
 * @return The action that would cause this transition
 */
template<class State_t, class Action_t>
std::optional<Action_t> getTransitionAction(
          const TransitionSystem<State_t, Action_t>& transitions, const State_t& parent, const State_t& child);


template<class State_t, class Action_t>
SequenceCheckResult applyPlan(State_t& state, const std::vector<Action_t>& actions,
          const TransitionSystem<State_t, Action_t>& transitions) {
    double plan_cost = 0.0;
    for (const auto& action : actions) {
        if (!transitions.isApplicable(state, action)) {
            return {false, plan_cost};
        }
        plan_cost += transitions.getActionCost(state, action);
        transitions.applyAction(state, action);
    }

    return {true, plan_cost};
}

template<class State_t, class Action_t>
SequenceCheckResult checkPlan(const State_t& state, const std::vector<Action_t>& actions,
          const TransitionSystem<State_t, Action_t>& transitions) {
    State_t state_copy(state);
    return applyPlan(state_copy, actions, transitions);
}

template<class State_t, class Action_t>
SequenceCheckResult checkStateSequence(
          const std::vector<State_t>& states, const TransitionSystem<State_t, Action_t>& transitions) {
    if (states.empty()) {  // empty path
        return {true, 0.0};
    } else if (!transitions.isValidState(states[0])) {  // first state is invalid
        return {false, 0.0};
    }

    std::vector<State_t> successors;
    double path_cost = 0.0;

    for (unsigned i = 0; i < states.size() - 1; i++) {
        if (!transitions.isValidState(states[i + 1])) {
            return {false, path_cost};
        }

        std::optional<Action_t> action = getTransitionAction(transitions, states[i], states[i + 1]);
        if (!action.has_value()) {
            return {false, path_cost};
        }

        path_cost += transitions.getActionCost(states[i], action.value());
    }
    return {true, path_cost};
}

template<class State_t, class Action_t>
SequenceCheckResult checkSolutionPlan(const State_t& state, const std::vector<Action_t>& actions,
          const TransitionSystem<State_t, Action_t>& transitions, const GoalTest<State_t>& goal) {
    State_t state_copy(state);
    SequenceCheckResult result = applyPlan(state_copy, actions, transitions);
    if (result.m_is_valid) {
        if (!goal.isGoal(state_copy)) {
            result.m_is_valid = false;
        }
    }
    return result;
}

template<class State_t, class Action_t>
SequenceCheckResult checkSolutionStateSequence(const std::vector<State_t>& states,
          const TransitionSystem<State_t, Action_t>& transitions,
          const GoalTest<State_t>& goal) {
    SequenceCheckResult result = checkStateSequence(states, transitions);

    if (result.m_is_valid) {
        if (states.empty() || !goal.isGoal(states.back())) {
            result.m_is_valid = false;
        }
    }
    return result;
}

template<class State_t, class Action_t>
std::optional<Action_t> getTransitionAction(
          const TransitionSystem<State_t, Action_t>& transitions, const State_t& parent, const State_t& child) {
    std::vector<Action_t> actions = transitions.getActions(parent);

    for (auto& action : actions) {
        State_t result = transitions.getChildState(parent, action);

        if (child == result) {
            return action;
        }
    }
    return std::nullopt;
}

template<class State_t, class Action_t>
std::vector<State_t> getSuccessors(const TransitionSystem<State_t, Action_t>& transitions, const State_t& state) {
    std::vector<Action_t> actions = transitions.getActions(state);
    std::vector<State_t> successors;

    for (const auto& action : actions) {
        successors.emplace_back(transitions.getChildState(state, action));
    }
    return successors;
}
#endif  //PLAN_AND_PATH_UTILS_H_
