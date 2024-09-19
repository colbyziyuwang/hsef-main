#ifndef WEIGHTED_F_COST_EVALUATOR_H_
#define WEIGHTED_F_COST_EVALUATOR_H_

#include "building_tools/evaluators/node_evaluator_with_cache.h"
#include "engines/engine_components/eval_functions/eval_function_terms.h"
#include "logging/logging_terms.h"
#include "logging/search_component_settings.h"
#include "search_basics/node_container.h"
#include "search_basics/node_evaluator.h"
#include "utils/string_utils.h"

#include <string>
#include <vector>

/**
 * An evaluator that calculates the weighted f-cost of a node.
 *
 * @tparam State_t The type of the state
 * @tparam Action_t The type of the action
 */
template<class State_t, class Action_t>
class WeightedFCostEvaluator : public NodeEvaluatorWithCache<State_t, Action_t> {
    using NE = NodeEvaluatorWithCache<State_t, Action_t>;

public:
    inline static const std::string CLASS_NAME = "WeightedFCost";  ///< The name of this component when the weight is > 1.0

    /**
     * Creates a WeightedFCostEvaluator.
     *
     * @param heuristic The heuristic value
     * @param weight The weight to use
     */
    explicit WeightedFCostEvaluator(NodeEvaluator<State_t, Action_t>& heuristic, double weight);

    // Overriden public NodeEvaluator functions
    void setNodeContainer(const NodeContainer<State_t, Action_t>& nodes) override;
    std::vector<NodeEvaluator<State_t, Action_t>*> getSubEvaluators() const override { return {m_heuristic}; }

    // Overriden public SettingsLogger methods
    std::string getName() const override { return CLASS_NAME; }

    /**
     * Sets the weight for the f-cost.
     *
     * @param weight The new weight value
     */
    void setWeight(double weight) { m_weight = weight; }

protected:
    // Overriden protected NodeEvaluatorWithStorage functions
    void doPrepare() override { m_heuristic->prepareToEvaluate(); }
    void doEvaluateAndCache(NodeID to_evaluate) override;
    void doReEvaluateAndCache(NodeID to_evaluate) override;
    void doReset() override { m_heuristic->reset(); };

    // Overriden protected SettingsLogger methods
    StringMap getComponentSettings() const override;
    SearchSettingsMap getSubComponentSettings() const override;

private:
    NodeEvaluator<State_t, Action_t>* m_heuristic;  ///< The base heuristic used as part of the f-cost
    double m_weight;  ///< The weight on the heuristic
};

template<class State_t, class Action_t>
WeightedFCostEvaluator<State_t, Action_t>::WeightedFCostEvaluator(NodeEvaluator<State_t, Action_t>& heuristic, double weight)
          : m_heuristic(&heuristic), m_weight(weight) {
}

template<class State_t, class Action_t>
void WeightedFCostEvaluator<State_t, Action_t>::setNodeContainer(const NodeContainer<State_t, Action_t>& nodes) {
    m_heuristic->setNodeContainer(nodes);
    NodeEvaluatorWithCache<State_t, Action_t>::setNodeContainer(nodes);
}

template<class State_t, class Action_t>
void WeightedFCostEvaluator<State_t, Action_t>::doEvaluateAndCache(NodeID to_evaluate) {
    m_heuristic->evaluate(to_evaluate);
    double eval = NE::getNodeContainer()->getGValue(to_evaluate) + m_weight * m_heuristic->getLastNodeEval();
    NE::setCachedValues(to_evaluate, eval, m_heuristic->isLastNodeADeadEnd());
}

template<class State_t, class Action_t>
void WeightedFCostEvaluator<State_t, Action_t>::doReEvaluateAndCache(NodeID to_evaluate) {
    m_heuristic->reEvaluate(to_evaluate);
    double eval = NE::getNodeContainer()->getGValue(to_evaluate) + m_weight * m_heuristic->getLastNodeEval();
    NE::setCachedValues(to_evaluate, eval, m_heuristic->isLastNodeADeadEnd());
}

template<class State_t, class Action_t>
StringMap WeightedFCostEvaluator<State_t, Action_t>::getComponentSettings() const {
    return {{evalFunctionTerms::SETTING_HEURISTIC_WEIGHT, roundAndToString(m_weight, 6)}};
}

template<class State_t, class Action_t>
SearchSettingsMap WeightedFCostEvaluator<State_t, Action_t>::getSubComponentSettings() const {
    SearchSettingsMap sub_components;
    sub_components[evalFunctionTerms::SETTING_HEURISTIC] = m_heuristic->getAllSettings();
    return sub_components;
}

#endif  //WEIGHTED_F_COST_EVALUATOR_H_
