#ifndef SET_AGGREGATE_EVALUATOR_FACTORY_H_
#define SET_AGGREGATE_EVALUATOR_FACTORY_H_

#include "building_tools/evaluators/set_aggregate_evaluator.h"
#include "evaluator_tools_terms.h"
#include "search_basics/node_evaluator.h"

#include <algorithm>
#include <cassert>
#include <vector>

/**
 * Generates an evaluator that returns the max evaluation over a set of given evaluators.
 *
 * @param sub_evaluators The list of sub-evaluators aggregated over
 * @param re_evaluate_type Defines how the evaluator is to behave on re-evaluation
 * @return The max evaluator
 */
template<class State_t, class Action_t>
SetAggregateEvaluator<State_t, Action_t> getMaxEvaluator(
          const std::vector<NodeEvaluator<State_t, Action_t>*>& sub_evaluators,
          SetAggregateReEvaluateType re_evaluate_type = SetAggregateReEvaluateType::standard) {
    return SetAggregateEvaluator<State_t, Action_t>(
              sub_evaluators,
              [](double eval1, double eval2) { return std::max(eval1, eval2); },
              evaluatorToolsTerms::OP_NAME_MAX, re_evaluate_type);
}

/**
 * Generates an evaluator that returns the sum of the evaluations of a given set of evaluators.
 *
 * @param sub_evaluators The list of sub-evaluators aggregated over
 * @param re_evaluate_type Defines how the evaluator is to behave on re-evaluation
 * @return The sum evaluator
 */
template<class State_t, class Action_t>
SetAggregateEvaluator<State_t, Action_t> getSumEvaluator(
          const std::vector<NodeEvaluator<State_t, Action_t>*>& sub_evaluators,
          SetAggregateReEvaluateType re_evaluate_type = SetAggregateReEvaluateType::standard) {
    return SetAggregateEvaluator<State_t, Action_t>(
              sub_evaluators,
              [](double eval1, double eval2) { return eval1 + eval2; },
              evaluatorToolsTerms::OP_NAME_SUM, re_evaluate_type);
}

#endif  //SET_AGGREGATE_EVALUATOR_FACTORY_H_
