#ifndef EVALUATOR_AND_COMPARING_USAGE_H_
#define EVALUATOR_AND_COMPARING_USAGE_H_

#include "search_basics/node_evaluator.h"

#include <vector>


/**
 * A struct that stores a NodeEvaluator and whether it is used to prioritize nodes by min or max.
 *
 */
template<class State_t, class Action_t>
struct EvaluatorAndComparingUsage {

    /**
     * Creates the object. The usage is assumed to prioritize lower evaluations by default.
     *
     * @param evaluator The evaluator
     * @param lower_is_better Whether to prioritize by lower or higher values
     */
    explicit EvaluatorAndComparingUsage(NodeEvaluator<State_t, Action_t>* evaluator, bool lower_is_better = true);

    /**
     * Creates the object. The usage is assumed to prioritize lower evaluations by default.
     *
     * @param evaluator The evaluator
     * @param lower_is_better Whether to prioritize by lower or higher values
     */
    explicit EvaluatorAndComparingUsage(NodeEvaluator<State_t, Action_t>& evaluator, bool lower_is_better = true);

    NodeEvaluator<State_t, Action_t>* m_evaluator;  ///< The evaluator
    bool m_lower_is_better;  ///< Whether to prioritize lower or higher values
};

template<class State_t, class Action_t>
EvaluatorAndComparingUsage<State_t, Action_t>::EvaluatorAndComparingUsage(NodeEvaluator<State_t, Action_t>* evaluator, bool lower_is_better)
          : m_evaluator(evaluator), m_lower_is_better(lower_is_better) {
}

template<class State_t, class Action_t>
EvaluatorAndComparingUsage<State_t, Action_t>::EvaluatorAndComparingUsage(NodeEvaluator<State_t, Action_t>& evaluator, bool lower_is_better)
          : m_evaluator(&evaluator), m_lower_is_better(lower_is_better) {
}

template<class State_t, class Action_t>
using EvalsAndUsageVec = std::vector<EvaluatorAndComparingUsage<State_t, Action_t>>;

#endif  //EVALUATOR_AND_COMPARING_USAGE_H_
