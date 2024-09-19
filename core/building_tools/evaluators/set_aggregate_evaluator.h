#ifndef SET_AGGREGATE_EVALUATOR_H_
#define SET_AGGREGATE_EVALUATOR_H_

#include "building_tools/evaluators/node_evaluator_with_cache.h"
#include "evaluator_tools_terms.h"
#include "logging/logging_terms.h"
#include "logging/search_component_settings.h"
#include "search_basics/node_container.h"
#include "search_basics/node_evaluator.h"

#include <cassert>
#include <cstdint>
#include <functional>
#include <string>
#include <tuple>
#include <vector>

enum class SetAggregateReEvaluateType : std::uint8_t {
    standard,  ///< Always re-evaluates and ignores previously stored value
    none,  ///< Never re-evaluates
    max  ///< Always re-evaluates and takes the max of the computed and previously stored valud
};

/**
 * Represents an evaluator that returns values by aggregating the results from several evaluators according to some
 * binary aggregator operator (ie. max, min, sum, etc.). The operator is assumed to be commutative and associative
 * 
 * TODO: Can't handle single heuristics
 * 
 * @class SetAggregateEvaluator
 */
template<class State_t, class Action_t>
class SetAggregateEvaluator : public NodeEvaluatorWithCache<State_t, Action_t> {

public:
    inline static const std::string CLASS_NAME = "SetAggregateEvaluator";  ///< Class name as used to define the component name

    /**
     * Creates a evaluator aggregator with the given aggregator operator.
     *
     * @param sub_evaluators The list of sub-evaluators aggregated over
     * @param aggregator_operator The aggregator operator
     * @param op_label Thge name of the operator. Used for generating the log for this SetAggregateEvaluator
     * @param re_evaluate_type Defines how the evaluator is to behave on re-evaluation
     */
    SetAggregateEvaluator(const std::vector<NodeEvaluator<State_t, Action_t>*>& sub_evaluators,
              const std::function<double(double, double)>& aggregator_operator, const std::string& op_label,
              SetAggregateReEvaluateType re_evaluate_type = SetAggregateReEvaluateType::standard);

    // Overriden public NodeEvaluator functions
    void setNodeContainer(const NodeContainer<State_t, Action_t>& nodes) override;
    std::vector<NodeEvaluator<State_t, Action_t>*> getSubEvaluators() const override { return m_sub_evaluators; }

    // Overriden public SettingsLogger methods
    std::string getName() const override { return CLASS_NAME; }

protected:
    // Overriden protected NodeEvaluatorWithStorage functions
    void doPrepare() override;
    void doEvaluateAndCache(NodeID to_evaluate) override;
    void doReset() override;
    void doReEvaluateAndCache(NodeID to_evaluate) override;

    // Overriden protected SettingsLogger methods
    StringMap getComponentSettings() const override { return {{evaluatorToolsTerms::SETTING_AGGREGATION_OPERATOR, m_op_label}}; }
    SearchSettingsMap getSubComponentSettings() const override;

private:
    /**
     * Calculates the aggregate evaluation.
     *
     * Assumes that evaluate or re-evaluate has been called for all the sub-evaluators.
     * Also ignores the previous calculated value and does not cache anything.
     *
     * @return The newly calculated aggregate eval and is_dead_end value.
     */
    std::tuple<double, bool> calculateEvaluation();

    std::vector<NodeEvaluator<State_t, Action_t>*> m_sub_evaluators;  ///< The collection of sub-evaluators to aggregate over
    std::function<double(double, double)> m_aggregator_operator;  ///< The operator for aggregating heuristic values
    std::string m_op_label;  ///< The name of the operation (max, min, sum, etc.)
    SetAggregateReEvaluateType m_re_evaluate_type;  ///< Defines the behaviour to use when re-evaluating
};

template<class State_t, class Action_t>
void SetAggregateEvaluator<State_t, Action_t>::setNodeContainer(const NodeContainer<State_t, Action_t>& nodes) {
    for (auto* sub_eval : m_sub_evaluators) {
        sub_eval->setNodeContainer(nodes);
    }
    NodeEvaluatorWithCache<State_t, Action_t>::setNodeContainer(nodes);
}

template<class State_t, class Action_t>
SetAggregateEvaluator<State_t, Action_t>::SetAggregateEvaluator(
          const std::vector<NodeEvaluator<State_t, Action_t>*>& sub_evaluators,
          const std::function<double(double, double)>& aggregator_operator,
          const std::string& op_label, SetAggregateReEvaluateType re_evaluate_type)
          : m_sub_evaluators(sub_evaluators), m_aggregator_operator(aggregator_operator), m_op_label(op_label), m_re_evaluate_type(re_evaluate_type) {
}

template<class State_t, class Action_t>
void SetAggregateEvaluator<State_t, Action_t>::doPrepare() {
    for (auto* sub_eval : m_sub_evaluators) {
        sub_eval->prepareToEvaluate();
    }
}

template<class State_t, class Action_t>
std::tuple<double, bool> SetAggregateEvaluator<State_t, Action_t>::calculateEvaluation() {
    double eval = 0.0;
    bool is_dead_end = false;
    bool first = true;

    for (auto& evaluator : m_sub_evaluators) {

        double current_value = evaluator->getLastNodeEval();
        if (evaluator->isLastNodeADeadEnd()) {
            is_dead_end = true;
        }
        if (first) {
            eval = current_value;
            first = false;
        } else {
            eval = m_aggregator_operator(eval, current_value);
        }
    }

    return {eval, is_dead_end};
}

template<class State_t, class Action_t>
void SetAggregateEvaluator<State_t, Action_t>::doEvaluateAndCache(NodeID to_evaluate) {
    for (auto& evaluator : m_sub_evaluators) {
        evaluator->evaluate(to_evaluate);
    }

    auto [eval, is_dead_end] = calculateEvaluation();
    NodeEvaluatorWithCache<State_t, Action_t>::setCachedValues(to_evaluate, eval, is_dead_end);
}

template<class State_t, class Action_t>
void SetAggregateEvaluator<State_t, Action_t>::doReEvaluateAndCache(NodeID to_evaluate) {
    if (m_re_evaluate_type == SetAggregateReEvaluateType::none) {
        return;
    }
    for (auto& evaluator : m_sub_evaluators) {
        evaluator->reEvaluate(to_evaluate);
    }

    auto [eval, is_dead_end] = calculateEvaluation();

    if (m_re_evaluate_type == SetAggregateReEvaluateType::max) {
        eval = std::max(eval, NodeEvaluatorWithCache<State_t, Action_t>::getCachedEval(to_evaluate));
        is_dead_end = is_dead_end || NodeEvaluatorWithCache<State_t, Action_t>::getCachedIsDeadEnd(to_evaluate);
    }
    NodeEvaluatorWithCache<State_t, Action_t>::setCachedValues(to_evaluate, eval, is_dead_end);
}

template<class State_t, class Action_t>
void SetAggregateEvaluator<State_t, Action_t>::doReset() {
    for (auto& evaluator : m_sub_evaluators) {
        evaluator->reset();
    }
}

template<class State_t, class Action_t>
SearchSettingsMap SetAggregateEvaluator<State_t, Action_t>::getSubComponentSettings() const {
    SearchSettingsMap sub_components;
    for (int i = 0; i < static_cast<int>(m_sub_evaluators.size()); ++i) {
        std::string subeval_role = evaluatorToolsTerms::SETTING_SUBEVALUATOR_PREFIX + std::to_string(i);
        sub_components[subeval_role] = m_sub_evaluators[i]->getAllSettings();
    }

    return sub_components;
}

#endif  //SET_AGGREGATE_EVALUATOR_H_
