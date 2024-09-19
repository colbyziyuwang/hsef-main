#ifndef F_COST_EVALUATOR_H_
#define F_COST_EVALUATOR_H_

#include "building_tools/evaluators/node_evaluator_with_cache.h"
#include "engines/engine_components/eval_functions/eval_function_terms.h"
#include "logging/logging_terms.h"
#include "logging/search_component_settings.h"
#include "search_basics/node_container.h"
#include "search_basics/node_evaluator.h"

#include <string>
#include <vector>

/**
 * An evaluator that calculates the f-cost of a node.
 *
 * @tparam State_t The type of state
 * @tparam Action_t The type of action
 * @class FCostEvaluator
 */
template<class State_t, class Action_t>
class FCostEvaluator : public NodeEvaluatorWithCache<State_t, Action_t> {
    using NE = NodeEvaluatorWithCache<State_t, Action_t>;

public:
    inline static const std::string CLASS_NAME = "FCost";  ///< The class name. Used to define the component's name.

    /**
     * Creates a FCostEvaluator.
     *
     * @param heuristic The heuristic value
     */
    explicit FCostEvaluator(NodeEvaluator<State_t, Action_t>& heuristic)
              : m_heuristic(&heuristic) {}

    // Overriden public NodeEvaluator functions
    void setNodeContainer(const NodeContainer<State_t, Action_t>& nodes) override;
    std::vector<NodeEvaluator<State_t, Action_t>*> getSubEvaluators() const override { return {m_heuristic}; }

    // Overriden public SettingsLogger methods
    std::string getName() const override { return CLASS_NAME; }

protected:
    // Overriden protected NodeEvaluatorWithStorage functions
    void doPrepare() override { m_heuristic->prepareToEvaluate(); }
    void doEvaluateAndCache(NodeID to_evaluate) override;
    void doReEvaluateAndCache(NodeID to_evaluate) override;
    void doReset() override { m_heuristic->reset(); };

    // Overriden protected SettingsLogger methods
    StringMap getComponentSettings() const override { return {}; }
    SearchSettingsMap getSubComponentSettings() const override;

private:
    NodeEvaluator<State_t, Action_t>* m_heuristic;  ///< The base heuristic used as part of the f-cost
};

template<class State_t, class Action_t>
void FCostEvaluator<State_t, Action_t>::setNodeContainer(const NodeContainer<State_t, Action_t>& nodes) {
    m_heuristic->setNodeContainer(nodes);
    NodeEvaluatorWithCache<State_t, Action_t>::setNodeContainer(nodes);
}

template<class State_t, class Action_t>
void FCostEvaluator<State_t, Action_t>::doEvaluateAndCache(NodeID to_evaluate) {
    m_heuristic->evaluate(to_evaluate);
    double f_cost = NE::getNodeContainer()->getGValue(to_evaluate) + m_heuristic->getLastNodeEval();
    NE::setCachedValues(to_evaluate, f_cost, m_heuristic->isLastNodeADeadEnd());
}

template<class State_t, class Action_t>
void FCostEvaluator<State_t, Action_t>::doReEvaluateAndCache(NodeID to_evaluate) {
    m_heuristic->reEvaluate(to_evaluate);
    double f_cost = NE::getNodeContainer()->getGValue(to_evaluate) + m_heuristic->getLastNodeEval();
    NE::setCachedValues(to_evaluate, f_cost, m_heuristic->isLastNodeADeadEnd());
}

template<class State_t, class Action_t>
SearchSettingsMap FCostEvaluator<State_t, Action_t>::getSubComponentSettings() const {
    SearchSettingsMap sub_components;
    sub_components[evalFunctionTerms::SETTING_HEURISTIC] = m_heuristic->getAllSettings();
    return sub_components;
}

#endif  //F_COST_EVALUATOR_H_
