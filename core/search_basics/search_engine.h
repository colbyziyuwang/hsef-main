#ifndef SEARCH_ENGINE_H_
#define SEARCH_ENGINE_H_

#include "experiment_running/search_resource_limits.h"
#include "logging/logging_terms.h"
#include "logging/settings_logger.h"
#include "logging/standard_search_statistics.h"
#include "search_basics/goal_test.h"
#include "search_basics/node_evaluator.h"
#include "search_basics/transition_system.h"

#include <cassert>
#include <cstdint>
#include <cstdlib>
#include <vector>

/**
 * Defines the possible statuses for a search engine.
 *
 * Below is the meaning of each of the values:
 *
 * not_ready - the engine is not ready for search.\n
 * ready - the engine is ready for search.\n
 * active - the engine is currently searching, no solution has been found yet.\n
 * search_completed - the search has terminated without hitting resource limits.\n
 * resource_limit_hit - the search terminated due to some resource limit having been reached.\n
 * error - the engine has hit an engine\n
 *
 * @class EngineStatus
 */
enum class EngineStatus : std::uint8_t {
    not_ready,
    ready,
    active,
    search_completed,
    resource_limit_hit
};

/**
 * An abstract method class defining the interface for a SearchEngine
 */
template<class State_t, class Action_t>
class SearchEngine : public virtual SettingsLogger {

public:
    /**
     * Checks if the engine has all necessary components set so that a search can be run.
     *
     * @return Whether the engine has been fully initialized.
     */
    virtual bool canRunSearch() const = 0;

    /**
     * Searches for a plan for the current problem and returns the final status of the engine. That involves resetting
     * the engine, initializing the engine for search, and repeatedly calling singleSearchStep until the the search
     * terminates.
     *
     * Will also return the status not_ready if the engine cannot be run because the search does not have all necessary
     * components set yet.
     *
     * @param initial_state The initial state of the search
     * @return The final status of the engine
     */
    virtual EngineStatus searchForPlan(const State_t& initial_state) = 0;

    /**
     * Gets the current status of the search engine.
     *
     * @return Status of the engine.
     */
    virtual EngineStatus getStatus() const = 0;

    /**
     * Whether a solution was found the last time a search was performed.
     *
     * Will also return false if the search is active and no solution has been found yet, the engine has been reset,
     * or the engine has not been fully initialized.
     *
     * @return Whether a solution was found by the last search
     */
    virtual bool hasFoundSolution() const = 0;

    /**
     * Gets the cost of the last solution plan found. A negative value will be returned if a search is active and no
     * incumbent solution has been found, the engine has been reset, the engine has not been fully initialized, the
     * start state is a goal state, or the search has been terminated and no solution was found.
     *
     * @return The last solution plan cost
     */
    virtual double getLastSolutionPlanCost() const = 0;

    /**
     * Gets the last solution plan found.
     *
     * An empty plan will be returned if a search is active and no incumbent solution has been found, the engine has
     * been reset, the engine has not been fully initialized, the start state is a goal state, or the search has been
     * terminated and no solution was found.
     *
     * @return The last plan found
     */
    virtual std::vector<Action_t> getLastSolutionPlan() const = 0;

    /**
     * Gets the standard statistics for the current search.
     *
     * @return The search statistics
     */
    virtual StandardSearchStatistics getStandardEngineStatistics() const = 0;

    /**
     * Returns the statistics that are specific to engine.
     *
     * @return The statistics specific to the engine.
     */
    virtual StringMap getEngineSpecificStatistics() const = 0;

    /**
     * Gets the list of base evaluators. These are all the evaluators used by the search engine
     * directly used by the engine, but is not necessarily all the evaluators that are implicitly
     * called by the base evaluators. By recursively calling each of the base evaluator's
     * getSubEvaluators method, we should be able to get the list of all evaluators both excplicitly
     * and implicitly.
     *
     * @return A vector of the base evaluators.
     */
    virtual std::vector<NodeEvaluator<State_t, Action_t>*> getBaseEvaluators() const = 0;

    /**
     * Sets the transition system to be used by the engine.
     *
     * Resets the engine whenever this is called.
     *
     * @param trans_system The new transition function.
     */
    virtual void setTransitionSystem(const TransitionSystem<State_t, Action_t>& trans_system) = 0;

    /**
     * Gets the transition system being called by this search engine.
     *
     * @return The search engine being called by this search engine.
     */
    virtual const TransitionSystem<State_t, Action_t>* getTransitionSystem() const = 0;

    /**
     * Sets the goal test function to be used by the engine.
     *
     * Resets the engine whenever this is called.
     *
     * @param goal_test The new goal test function.
     */
    virtual void setGoalTest(const GoalTest<State_t>& goal_test) = 0;

    /**
     * Gets the goal test being called by this search engine.
     *
     * @return The goal test being called by this search engine.
     */
    virtual const GoalTest<State_t>* getGoalTest() const = 0;

    /**
     * Sets the resource limits for any search done by this engine.
     *
     * @param resource_limits
     */
    virtual void setResourceLimits(const SearchResourceLimits& resource_limits) = 0;

    /**
     * Resets the engine so that it is ready to run a new search.
     */
    virtual void reset() = 0;
};

#endif  //SEARCH_ENGINE_H_
