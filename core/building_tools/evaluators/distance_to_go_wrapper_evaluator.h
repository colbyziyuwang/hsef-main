#ifndef DISTANCE_TO_GO_WRAPPER_EVALUATOR_H_
#define DISTANCE_TO_GO_WRAPPER_EVALUATOR_H_

#include "building_tools/evaluators/cost_and_distance_to_go_evaluator.h"
#include "building_tools/evaluators/evaluator_tools_terms.h"
#include "logging/logging_terms.h"
#include "logging/search_component_settings.h"
#include "search_basics/node_container.h"
#include "search_basics/node_evaluator.h"

#include <string>
#include <vector>

/**
 * A Wrapper around a DistanceToGo evaluator, that allows for the distance-to-go estimates to be used as needed.
 *
 * @tparam State_t The type of state
 * @tparam Action_t The type of action
 */
template<class State_t, class Action_t>
class DistanceToGoWrapperEvaluator : public NodeEvaluator<State_t, Action_t> {

public:
    inline static const std::string CLASS_NAME_POSTFIX = "-DistanceToGo";  ///< The postfix to this evaluator. Used to define the component's name alongside the name of the base evaluator

    /**
     * Creates a wrapper around a distance-to-go evaluator that acts like an evaluator, but returns the distance-to-go
     * estimate computed by the base evaluator.
     *
     * @param base_evaluator The base distance-to-go evaluator wrapped by this object
     */
    explicit DistanceToGoWrapperEvaluator(CostAndDistanceToGoEvaluator<State_t, Action_t>& base_evaluator)
              : m_base_evaluator(&base_evaluator) {}

    // Overridden public NodeEvaluator methods
    void setNodeContainer(const NodeContainer<State_t, Action_t>& nodes) override { m_base_evaluator->setNodeContainer(nodes); }
    const NodeContainer<State_t, Action_t>* getNodeContainer() const override { return m_base_evaluator->getNodeContainer(); }
    void prepareToEvaluate() override { m_base_evaluator->prepareToEvaluate(); }
    void evaluate(NodeID to_evaluate) override { m_base_evaluator->evaluate(to_evaluate); }
    void reEvaluate(NodeID to_evaluate) override { m_base_evaluator->reEvaluate(to_evaluate); }
    void reset() override { m_base_evaluator->reset(); }
    NodeID getIDofLastEvaluatedNode() const override { return m_base_evaluator->getIDofLastEvaluatedNode(); }
    bool isEvalComputed() const override { return m_base_evaluator->isEvalComputed(); }
    double getCachedEval(NodeID node_id) const override { return m_base_evaluator->getCachedDistanceToGoEval(node_id); }
    bool getCachedIsDeadEnd(NodeID node_id) const override { return m_base_evaluator->getCachedIsDeadEnd(node_id); }
    void setCachedEval(NodeID node_id, double eval) override { m_base_evaluator->setCachedDistanceToGoEval(node_id, eval); }
    void setIsDeadEnd(NodeID node_id, bool is_dead_end) override { m_base_evaluator->setIsDeadEnd(node_id, is_dead_end); }
    std::vector<NodeEvaluator<State_t, Action_t>*> getSubEvaluators() const override { return {m_base_evaluator}; }

    // Overriden public SettingsLogger methods
    std::string getName() const override { return m_base_evaluator->getName() + CLASS_NAME_POSTFIX; }

protected:
    // Overriden protected SettingsLogger methods
    StringMap getComponentSettings() const override { return {}; }
    SearchSettingsMap getSubComponentSettings() const override { return {{evaluatorToolsTerms::SETTING_BASE_EVALUATOR, m_base_evaluator->getAllSettings()}}; }

private:
    CostAndDistanceToGoEvaluator<State_t, Action_t>* m_base_evaluator;  ///< The base evaluator this object wraps
};

#endif  //DISTANCE_TO_GO_WRAPPER_EVALUATOR_H_
