#ifndef EVALUATOR_UTILS_H_
#define EVALUATOR_UTILS_H_

#include "building_tools/evaluators/single_goal_state_evaluator.h"
#include "search_basics/node_evaluator.h"

#include <iostream>
#include <unordered_map>
#include <vector>

/**
 * Gets all the evaluators in the given vector, and those called by the given vectors. All evaluators are ordered in the
 * vector such that if an evaluator A relies on the output of evaluator B, B occurs before A.
 *
 * The dependencies of the given vector of evaluators are assumed to form a subset of a DAG. Thus, this function is
 * returning a topological sort of the DAG.
 *
 * @param base_evaluators A list of evaluators whose sub evaluators should be found
 * @return A list of all the evaluators needed for calling the given list, ordered by dependencies
 */
template<class State_t, class Action_t>
std::vector<NodeEvaluator<State_t, Action_t>*> getAllEvaluators(std::vector<NodeEvaluator<State_t, Action_t>*>& base_evaluators);

/**
 * Any of the given evaluators which correspond to a single goal state are updated to use that new goal state.
 * Other evaluators are ignored.
 *
 * @tparam State_t The type of state
 * @tparam Action_t The type of action
 * @param evaluators The vector of evaluators
 * @param goal_state The new goal state
 */
template<class State_t, class Action_t>
void updateEvaluatorGoalState(std::vector<NodeEvaluator<State_t, Action_t>*>& evaluators, const State_t& goal_state);

/**
 * A namespace to be used with getAllEvaluators. Specifically it contains the helper method and some additional
 * naming just for help.
 */
namespace evaluatorTopologicalSort {

    template<class State_t, class Action_t>
    using EvalProcessedMap = std::unordered_map<NodeEvaluator<State_t, Action_t>*, bool>;  ///< A map where the entry is true if the evaluator has been processed and false if it is currently being processed

    template<class State_t, class Action_t>
    using EvalVector = std::vector<NodeEvaluator<State_t, Action_t>*>;  ///< A vector of node evaluator pointers

    /**
     * A helper function for getAllEvaluators, which performs the topological sort using depth-first search. Returns the
     * list of unprocessed evaluators needed by the given evaluator.
     *
     * @param evaluator The evaluator currently being processed
     * @param status_map A map which identifies which evaluators have already been processed
     * @return A list of unprocessed evaluators needed by the given evaluator
     */
    template<class State_t, class Action_t>
    EvalVector<State_t, Action_t> getAllSubEvaluators(NodeEvaluator<State_t, Action_t>* evaluator, EvalProcessedMap<State_t, Action_t>& status_map) {
        auto eval_check = status_map.find(evaluator);

        if (eval_check != status_map.end()) {
            // Evaluator processed already, so return nothing
            if (eval_check->second) {
                return {};
            } else {
                std::cerr << "Vector of Input Evaluators is not acyclic";
                // TODO Output error message because the input is not a DAG
            }
        }

        status_map[evaluator] = false;  // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)

        auto children = evaluator->getSubEvaluators();

        EvalVector<State_t, Action_t> all_sub_evaluators;

        for (auto eval : children) {
            auto next = getAllSubEvaluators(eval, status_map);

            all_sub_evaluators.insert(all_sub_evaluators.end(), next.begin(), next.end());
        }

        status_map[evaluator] = true;  // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
        all_sub_evaluators.push_back(evaluator);

        return all_sub_evaluators;
    }
}  // namespace evaluatorTopologicalSort

template<class State_t, class Action_t>
std::vector<NodeEvaluator<State_t, Action_t>*> getAllEvaluators(std::vector<NodeEvaluator<State_t, Action_t>*>& base_evaluators) {
    using namespace evaluatorTopologicalSort;

    EvalVector<State_t, Action_t> sub_evaluators;
    EvalProcessedMap<State_t, Action_t> status_map;

    for (auto evaluator : base_evaluators) {
        auto next = getAllSubEvaluators(evaluator, status_map);
        sub_evaluators.insert(sub_evaluators.end(), next.begin(), next.end());
    }
    return sub_evaluators;
}

template<class State_t, class Action_t>
void updateEvaluatorGoalState(std::vector<NodeEvaluator<State_t, Action_t>*>& evaluators, const State_t& goal_state) {
    for (auto eval : evaluators) {
        assert(eval != nullptr);
        auto* converted_evaluator = dynamic_cast<SingleGoalStateEvaluator<State_t>*>(eval);

        if (converted_evaluator != nullptr) {
            if (converted_evaluator->getGoalState() != goal_state) {
                converted_evaluator->setGoalState(goal_state);
            }
        }
    }
}

#endif  //EVALUATOR_UTILS_H_
