#ifndef SINGLE_STEP_SEARCH_ENGINE_H_
#define SINGLE_STEP_SEARCH_ENGINE_H_

#include "experiment_running/search_resource_limits.h"
#include "logging/logging_terms.h"
#include "logging/standard_search_statistics.h"
#include "search_basics/goal_test.h"
#include "search_basics/node_container.h"
#include "search_basics/node_evaluator.h"
#include "search_basics/search_engine.h"
#include "search_basics/transition_system.h"
#include "utils/evaluator_utils.h"
#include "utils/floating_point_utils.h"
#include "utils/string_utils.h"
#include "utils/timer.h"

#include <algorithm>  // needed for std::reverse
#include <cassert>
#include <cstdint>
#include <cstdlib>
#include <memory>
#include <random>
#include <string>
#include <vector>


/**
 * Defines an abstract class for a SingleStepSearchEngine for engines that work in an iterative nature.
 */
template<class State_t, class Action_t>
class SingleStepSearchEngine : public virtual SearchEngine<State_t, Action_t> {

public:
    // Overriden public SearchEngine methods
    bool canRunSearch() const override { return (m_goal_test && m_transition_system && doCanRunSearch()); }
    EngineStatus searchForPlan(const State_t& initial_state) override;
    bool hasFoundSolution() const override { return m_have_incumbent_plan; }
    EngineStatus getStatus() const override { return m_status; }
    double getLastSolutionPlanCost() const override { return m_incumbent_cost; }
    std::vector<Action_t> getLastSolutionPlan() const override { return m_incumbent_plan; }
    StandardSearchStatistics getStandardEngineStatistics() const override { return m_search_stats; }
    StringMap getEngineSpecificStatistics() const override;
    std::vector<NodeEvaluator<State_t, Action_t>*> getBaseEvaluators() const override = 0;
    void setTransitionSystem(const TransitionSystem<State_t, Action_t>& trans_system) override;
    const TransitionSystem<State_t, Action_t>* getTransitionSystem() const override { return m_transition_system; }
    void setGoalTest(const GoalTest<State_t>& goal_test) override;
    const GoalTest<State_t>* getGoalTest() const override { return m_goal_test; }
    void setResourceLimits(const SearchResourceLimits& resource_limits) override { m_resource_limits = resource_limits; }
    void reset() override;

    /**
     * Initializes the engine so that the search is active and search can begin from the given initial state.
     *
     * Can only be called if the engine status is ready, and will change the status to active.
     *
     * @param initial_state The initial state
     */
    void initializeSearch(const State_t& initial_state);

    /**
     * Performs a single step of the search and returns the engine's status after that step. This function should only
     * be called if the engine status is active. Thus, the initializeSearch method should be called first.
     *
     * This method should update the engine status as need be.
     *
     * @return The engine's status after a single step of the search
     */
    EngineStatus singleSearchStep();

    /**
     * Sets the seed for the random number generator.
     *
     * @param seed The seed to use
     */
    void setRandomNumberGeneratorSeed(unsigned seed) { m_random_seed = seed; }

    /**
     * Returns the random number generator.
     *
     * @return The random number generator.
     */
    std::shared_ptr<std::mt19937> getRandomNumGenerator() const { return m_rand_gen; }

protected:
    /**
     * Performs the initialization of the search for each specific kind of engine.
     *
     * @param initial_state The initial state for search
     */
    virtual void doSearchInitialization(const State_t& initial_state) = 0;

    /**
     * Performs the single search step and returns the engine status after that step.
     *
     * @return The engine status after the step.
     */
    virtual EngineStatus doSingleSearchStep() = 0;

    /**
     * Checks if the engine in use has all necessary components set so that a search can be run.
     *
     * @return Whether the engine has been fully initialized.
     */
    virtual bool doCanRunSearch() const = 0;

    /**
     * Resets the engine in use so that it is ready to run a new search.
     */
    virtual void doReset() = 0;

    /**
    * Gets the search engine's parameters of the engine in use
    * 
    * Returns an unordered map that contains that algorithms configuration values
    * 
    * @return Configuration values of the used alorithm
    */
    virtual StringMap getEngineParamsLog() const = 0;

    /**
     * Checks if given state is a goal using the stored goal test. Also increments the number of goal tests made.
     *
     * @param state The state to test
     * @return True if the state is a goal, false otherwise.
     */
    bool isGoal(const State_t& state);

    /**
     * Generates and returns the applicable actions in a state. Also updates the count of getAction calls and
     * number of actions generated.
     *
     * @param state The state to generate actions in
     * @return The list of actions applicable in the current state.
     */
    std::vector<Action_t> getApplicableActions(const State_t& state);

    /**
     * Gets the child state of a given state and action pair, and updates the number of states generated.
     *
     * @param state The parent state
     * @param action The action to apply
     * @return The child state
     */
    State_t getChildState(const State_t& state, const Action_t& action);

    /**
     * Checks if the next action is the inverse of last action in the given state.
     *
     * @param state The current state
     * @param next_action The next action
     * @param last_action The action that led to the current state
     * @return If the next action is the inverse of the last action in a given state.
     */
    bool isInverseAction(const State_t& state, const Action_t& next_action, const Action_t& last_action) const { return m_transition_system->isInverseAction(state, next_action, last_action); }

    /**
     * Gets the cost of applying the given action in the given state.
     *
     * Assumes the action is applicable.
     *
     * @param state The state to apply the action to
     * @param action The action
     * @return The cost of the action.
     */
    double getActionCost(const State_t& state, const Action_t& action) const { return m_transition_system->getActionCost(state, action); }

    /**
     * Sets the incumbent solution to the given plan and cost. Assumes cost is correct for the given plan.
     *
     * Assumes the new plan is better.
     *
     * @param plan The new incumbent plan
     * @param cost The new incumbent plan cost
     */
    void setIncumbentSolution(const std::vector<Action_t>& plan, double cost);

    /**
     * Extracts and sets the incumbent solution from a given NodeID and NodeContainer.
     *
     * Assumes the new plan is better.
     *
     * @param path_end_id The NodeID of the node at the end of the path
     * @param node_set The set of nodes
     */
    void setIncumbentSolution(NodeID path_end_id, const NodeContainer<State_t, Action_t>& node_set);

    /**
     * Checks if the resource limit has been hit.
     *
     * @return If the resource limit has been hit.
     */
    virtual bool hasHitResourceLimit() const;

    /**
     * Extracts the full list of evaluators from the base evaluators and assigns IDs to all of them.
     */
    void initializeAllEvaluators();

    /**
      * Evaluates the node corresponding to the given ID using all of the provided evaluators.
      *
      * @param evaluators The set of evaluators to use
      * @param to_evaluate The ID of the node to evaluate
      */
    void evaluateNode(std::vector<NodeEvaluator<State_t, Action_t>*> evaluators, NodeID to_evaluate);

    /**
      * Evaluates the node corresponding to the given ID using all of the provided evaluators.
      *
      * @param to_evaluate The ID of the node to evaluate
      */
    void evaluateNode(NodeID to_evaluate) { evaluateNode(m_evaluators, to_evaluate); }

    /**
     * Re-evaluates the node corresponding to the given ID using all of the provided evaluators. Thus, previous computations may be reused as
     * applicable.
     *
     * @param evaluators The set of evaluators to use
     * @param to_evaluate The ID of the node to evaluate
     */
    void reEvaluateNode(std::vector<NodeEvaluator<State_t, Action_t>*> evaluators, NodeID to_evaluate);

    /**
     * Re-evaluates the node corresponding to the given ID using all evaluators. Thus, previous computations may be reused as applicable.
     *
     * @param to_evaluate The ID of the node to evaluate
     */
    void reEvaluateNode(NodeID to_evaluate) { reEvaluateNode(m_evaluators, to_evaluate); }

    // Overidden SettingsLogger methods
    StringMap getComponentSettings() const override;

private:
    /**
     * Initializes (or resets) the random number generator with the given seed.
     */
    void initializeRandomNumberGenerator();

    EngineStatus m_status = EngineStatus::not_ready;  ///< The current search status

    std::vector<NodeEvaluator<State_t, Action_t>*> m_evaluators;  ///< The list of all evaluators
    const TransitionSystem<State_t, Action_t>* m_transition_system = nullptr;  ///< The transition system
    const GoalTest<State_t>* m_goal_test = nullptr;  ///< The goal test function

    bool m_have_incumbent_plan = false;  ///< If an incumbent solution is stored or not
    std::vector<Action_t> m_incumbent_plan;  ///< The last solution found
    double m_incumbent_cost = -1.0;  ///< The cost of the last solution

    std::shared_ptr<std::mt19937> m_rand_gen = nullptr;  ///< Random number generator
    bool m_use_stored_seed = false;  ///< Whether to use stored seed, or random seed for next search
    unsigned m_random_seed = 0;  ///<Random number generator

    StandardSearchStatistics m_search_stats;  ///< The statistics for the search
    SearchResourceLimits m_resource_limits;  ///< The resource limits for the search

    Timer m_timer;  ///< The timer for the search
    int64_t m_num_search_steps = 0;  ///< The number of search steps performed
};

template<class State_t, class Action_t>
void SingleStepSearchEngine<State_t, Action_t>::reset() {
    m_have_incumbent_plan = false;
    m_incumbent_plan.clear();
    m_incumbent_cost = -1.0;
    m_status = EngineStatus::not_ready;
    m_num_search_steps = 0;
    m_search_stats.reset();

    for (auto eval : m_evaluators) {
        eval->reset();
    }

    doReset();

    if (canRunSearch()) {
        m_status = EngineStatus::ready;
    }
}

template<class State_t, class Action_t>
void SingleStepSearchEngine<State_t, Action_t>::initializeSearch(const State_t& initial_state) {
    assert(m_status == EngineStatus::ready);

    if (!m_use_stored_seed) {
        m_random_seed = std::random_device{}();
    }
    initializeRandomNumberGenerator();
    initializeAllEvaluators();

    m_timer.startTimer();
    doSearchInitialization(initial_state);
    m_status = EngineStatus::active;
}

template<class State_t, class Action_t>
EngineStatus SingleStepSearchEngine<State_t, Action_t>::singleSearchStep() {
    assert(m_status == EngineStatus::active);
    m_num_search_steps++;
    if (hasHitResourceLimit()) {
        m_status = EngineStatus::resource_limit_hit;
    } else {
        m_status = doSingleSearchStep();
    }

    if (m_status != EngineStatus::active) {
        m_timer.endTimer();
        m_search_stats.m_search_time_seconds = m_timer.getLastTimePeriodDuration();
    }
    return m_status;
}

template<class State_t, class Action_t>
EngineStatus SingleStepSearchEngine<State_t, Action_t>::searchForPlan(const State_t& initial_state) {
    reset();
    if (m_status == EngineStatus::not_ready) {
        return EngineStatus::not_ready;
    }

    initializeSearch(initial_state);
    while (m_status == EngineStatus::active) {
        singleSearchStep();
    }
    return m_status;
}

template<class State_t, class Action_t>
void SingleStepSearchEngine<State_t, Action_t>::setGoalTest(const GoalTest<State_t>& goal_test) {
    m_goal_test = &goal_test;
    reset();
}

template<class State_t, class Action_t>
void SingleStepSearchEngine<State_t, Action_t>::setTransitionSystem(const TransitionSystem<State_t, Action_t>& trans_system) {
    m_transition_system = &trans_system;
    reset();
}

template<class State_t, class Action_t>
StringMap SingleStepSearchEngine<State_t, Action_t>::getEngineSpecificStatistics() const {
    StringMap stats;
    stats["num_search_steps"] = std::to_string(m_num_search_steps);
    return stats;
}

template<class State_t, class Action_t>
void SingleStepSearchEngine<State_t, Action_t>::setIncumbentSolution(const std::vector<Action_t>& plan, double cost) {
    m_have_incumbent_plan = true;
    m_incumbent_plan = plan;
    m_incumbent_cost = cost;
}

template<class State_t, class Action_t>
void SingleStepSearchEngine<State_t, Action_t>::setIncumbentSolution(NodeID path_end_id, const NodeContainer<State_t, Action_t>& node_set) {
    double incumbent_cost = 0.0;
    std::vector<Action_t> incumbent_plan;

    NodeID current_id = path_end_id;

    // While can follow the path backwards
    while (node_set.getLastAction(current_id).has_value()) {
        incumbent_plan.emplace_back(node_set.getLastAction(current_id).value());
        incumbent_cost += node_set.getLastActionCost(current_id);
        current_id = node_set.getParentID(current_id);
    }

    assert(!fpGreater(incumbent_cost, node_set.getGValue(path_end_id)));
    std::reverse(incumbent_plan.begin(), incumbent_plan.end());
    setIncumbentSolution(incumbent_plan, incumbent_cost);
}

template<class State_t, class Action_t>
void SingleStepSearchEngine<State_t, Action_t>::initializeRandomNumberGenerator() {
    if (m_rand_gen == nullptr) {
        m_rand_gen = std::make_shared<std::mt19937>(m_random_seed);
    } else {
        m_rand_gen->seed(m_random_seed);
    }
}

template<class State_t, class Action_t>
bool SingleStepSearchEngine<State_t, Action_t>::isGoal(const State_t& state) {
    m_search_stats.m_num_goal_tests++;
    return m_goal_test->isGoal(state);
}

template<class State_t, class Action_t>
std::vector<Action_t> SingleStepSearchEngine<State_t, Action_t>::getApplicableActions(const State_t& state) {
    m_search_stats.m_num_get_actions_calls += 1;

    std::vector<Action_t> actions = m_transition_system->getActions(state);
    m_search_stats.m_num_actions_generated += actions.size();
    return actions;
}

template<class State_t, class Action_t>
State_t SingleStepSearchEngine<State_t, Action_t>::getChildState(const State_t& state, const Action_t& action) {
    m_search_stats.m_num_states_generated++;
    return m_transition_system->getChildState(state, action);
}

template<class State_t, class Action_t>
bool SingleStepSearchEngine<State_t, Action_t>::hasHitResourceLimit() const {
    // TODO Add space limit
    return m_resource_limits.hasHitGoalTestLimit(m_search_stats) ||
           m_resource_limits.hasHitNumEvalLimit(m_search_stats) ||
           m_resource_limits.hasHitGetActionsCallLimit(m_search_stats) ||
           m_resource_limits.hasHitStateGenerationLimit(m_search_stats) ||
           m_resource_limits.hasHitTimeLimit(m_timer);
}

template<class State_t, class Action_t>
StringMap SingleStepSearchEngine<State_t, Action_t>::getComponentSettings() const {
    StringMap log = getEngineParamsLog();
    log["use_stored_seed"] = boolToString(m_use_stored_seed);
    log["random_seed"] = std::to_string(m_random_seed);
    return log;
}

template<class State_t, class Action_t>
void SingleStepSearchEngine<State_t, Action_t>::initializeAllEvaluators() {
    auto base_evals = getBaseEvaluators();
    m_evaluators = getAllEvaluators(base_evals);

    for (int i = 0; i < static_cast<int>(m_evaluators.size()); i++) {
        m_evaluators[i]->reset();
    }
}

template<class State_t, class Action_t>
void SingleStepSearchEngine<State_t, Action_t>::evaluateNode(std::vector<NodeEvaluator<State_t, Action_t>*> evaluators, NodeID to_evaluate) {
    m_search_stats.m_num_evals++;
    for (auto eval : evaluators) {
        eval->prepareToEvaluate();
    }

    for (auto eval : evaluators) {
        eval->evaluate(to_evaluate);
    }
}

template<class State_t, class Action_t>
void SingleStepSearchEngine<State_t, Action_t>::reEvaluateNode(std::vector<NodeEvaluator<State_t, Action_t>*> evaluators, NodeID to_evaluate) {
    m_search_stats.m_num_evals++;
    for (auto eval : evaluators) {
        eval->prepareToEvaluate();
    }

    for (auto eval : evaluators) {
        eval->reEvaluate(to_evaluate);
    }
}


#endif /* SINGLE_STEP_SEARCH_ENGINE_H_ */
