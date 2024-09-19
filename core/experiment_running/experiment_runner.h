#ifndef EXPERIMENT_RUNNER_H_
#define EXPERIMENT_RUNNER_H_

#include "building_tools/goal_tests/single_state_goal_test.h"
#include "experiment_results.h"
#include "experiment_running/search_resource_limits.h"
#include "logging/experiment_results_writer.h"
#include "search_basics/goal_test.h"
#include "search_basics/node_evaluator.h"
#include "search_basics/search_engine.h"
#include "search_basics/transition_system.h"
#include "utils/evaluator_utils.h"

#include <cassert>
#include <cstddef>
#include <iostream>
#include <string>
#include <vector>


/**
 * Runs an experiment for the given engine starting from the given start state.
 *
 * Assumes the transition system and goal test have already been set.
 *
 * @tparam State_t The type of state
 * @tparam Action_t The type of action
 * @param engine The engine to run
 * @param start The start state to run the experiment from
 * @return The result of the run experiment
 */
template<class State_t, class Action_t>
ExperimentResults<Action_t> runExperiment(SearchEngine<State_t, Action_t>& engine, const State_t& start);

/**
 * Runs an experiment using the given engine on a given start and goal state.
 *
 * Assumes the transition system is already set, and the list of evaluators is the set of all evaluators
 * in the engine (both base evaluators and those they call) that are to be made specific to the goal
 * state.
 *
 * @tparam State_t The type of state
 * @tparam Action_t The type of action
 * @param engine The engine to run
 * @param start The start state for the experiment
 * @param goal The goal state for the experiment
 * @param evaluators The set of evaluators to make specific to the goal state
 * @return
 */
template<class State_t, class Action_t>
ExperimentResults<Action_t> runExperiment(SearchEngine<State_t, Action_t>& engine, const State_t& start, const State_t& goal,
          std::vector<NodeEvaluator<State_t, Action_t>*>& evaluators);

/**
 * Runs a suite of experiments that all use the same goal test and returns the results.
 *
 * @tparam State_t The type of state
 * @tparam Action_t The type of action
 * @param engine The engine to run
 * @param transition_system The transition system for the experiment
 * @param goal_test The goal test for the experiment
 * @param resource_limits The resource limits for the experiment
 * @param starts The set of start states to use
 * @param incremental_output Whether to output results as a CSV incrementally
 * @return The results of all the experiments
 */
template<class State_t, class Action_t>
std::vector<ExperimentResults<Action_t>> runExperiments(
          SearchEngine<State_t, Action_t>& engine, const TransitionSystem<State_t, Action_t>& transition_system,
          const GoalTest<State_t>& goal_test, const SearchResourceLimits& resource_limits, const std::vector<State_t>& starts,
          bool incremental_output = false);

/**
 * Runs a suite of experiments given a list of start and goal states and returns the results.
 *
 * @tparam State_t The type of state
 * @tparam Action_t The type of action
 * @param engine The engine to run
 * @param transition_system The transition system for the experiment
 * @param resource_limits The resource limits for the experiment
 * @param starts The set of start states to use
 * @param goals The set of goal states to use
 * @param incremental_output Whether to output results as a CSV incrementally
 * @return The results of all the experiments
 */
template<class State_t, class Action_t>
std::vector<ExperimentResults<Action_t>> runExperiments(
          SearchEngine<State_t, Action_t>& engine, const TransitionSystem<State_t, Action_t>& transition_system,
          const SearchResourceLimits& resource_limits, const std::vector<State_t>& starts, const std::vector<State_t>& goals,
          bool incremental_output = false);

template<class State_t, class Action_t>
ExperimentResults<Action_t> runExperiment(SearchEngine<State_t, Action_t>& engine, const State_t& start) {
    engine.searchForPlan(start);

    ExperimentResults<Action_t> output;
    output.m_standard_stats = engine.getStandardEngineStatistics();
    output.m_engine_specific_stats = engine.getEngineSpecificStatistics();
    output.m_plan = engine.getLastSolutionPlan();
    output.m_plan_cost = engine.getLastSolutionPlanCost();
    output.m_has_found_plan = engine.hasFoundSolution();
    output.m_engine_settings = engine.getAllSettings();
    output.m_transitions_settings = engine.getTransitionSystem()->getAllSettings();
    output.m_goal_test_settings = engine.getGoalTest()->getAllSettings();

    return output;
}

template<class State_t, class Action_t>
ExperimentResults<Action_t> runExperiment(SearchEngine<State_t, Action_t>& engine, const State_t& start, const State_t& goal, std::vector<NodeEvaluator<State_t, Action_t>*>& evaluators) {
    SingleStateGoalTest<State_t> goal_test = SingleStateGoalTest<State_t>(goal);

    engine.setGoalTest(goal_test);
    updateEvaluatorGoalState(evaluators, goal);

    return runExperiment(engine, start);
}

template<class State_t, class Action_t>
std::vector<ExperimentResults<Action_t>> runExperiments(
          SearchEngine<State_t, Action_t>& engine, const TransitionSystem<State_t, Action_t>& transition_system,
          const GoalTest<State_t>& goal_test, const SearchResourceLimits& resource_limits, const std::vector<State_t>& starts,
          bool incremental_output) {

    engine.setTransitionSystem(transition_system);
    engine.setGoalTest(goal_test);
    engine.setResourceLimits(resource_limits);

    std::vector<ExperimentResults<Action_t>> results;
    results.reserve(starts.size());

    unsigned experiment_num = 1;
    for (auto& state : starts) {
        results.emplace_back(runExperiment(engine, state));
        if (incremental_output) {
            if (experiment_num == 1) {
                std::cout << getCSVHeader(results[0]) << "\n";
            }
            std::cout << getResultAsCSV(results.back(), std::to_string(experiment_num)) << "\n";
        }
        experiment_num++;
    }
    return results;
}

template<class State_t, class Action_t>
std::vector<ExperimentResults<Action_t>> runExperiments(
          SearchEngine<State_t, Action_t>& engine, const TransitionSystem<State_t, Action_t>& transition_system,
          const SearchResourceLimits& resource_limits,
          const std::vector<State_t>& starts, const std::vector<State_t>& goals, bool incremental_output) {

    if (starts.size() != goals.size()) {
        //TODO Throw an error
    }
    engine.setTransitionSystem(transition_system);
    engine.setResourceLimits(resource_limits);

    auto base_evals = engine.getBaseEvaluators();
    auto all_evaluators = getAllEvaluators(base_evals);

    std::vector<ExperimentResults<Action_t>> results;
    results.reserve(starts.size());

    for (std::size_t i = 0; i < starts.size(); i++) {
        results.emplace_back(runExperiment(engine, starts[i], goals[i], all_evaluators));
        if (incremental_output) {
            if (i == 0) {
                std::cout << getCSVHeader(results[i]) << "\n";
            }
            std::cout << getResultAsCSV(results.back(), std::to_string(i + 1)) << "\n";
        }
    }
    return results;
}
#endif  // !EXPERIMENT_RUNNER_
