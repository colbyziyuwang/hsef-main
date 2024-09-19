#ifndef G_COST_EVALUATOR_H_
#define G_COST_EVALUATOR_H_

#include "logging/logging_terms.h"
#include "logging/search_component_settings.h"
#include "search_basics/node_container.h"
#include "search_basics/node_evaluator.h"

#include <cassert>
#include <optional>
#include <stdexcept>
#include <string>
#include <vector>

/**
 * A class for a g-cost evaluator. Simply returns the values for g-costs stored in a container.
 */
template<class State_t, class Action_t>
class GCostEvaluator : public NodeEvaluator<State_t, Action_t> {

public:
    inline static const std::string CLASS_NAME = "GCost";  ///< The name of the class. Used to define the component's name

    /**
     * Constructor for a g-cost evaluator.
     */
    GCostEvaluator() = default;

    // Overridden public NodeEvaluator methods
    void setNodeContainer(const NodeContainer<State_t, Action_t>& nodes) override { m_nodes = &nodes; }
    const NodeContainer<State_t, Action_t>* getNodeContainer() const override { return m_nodes; }
    void prepareToEvaluate() override { m_last_node_id = std::nullopt; }
    void evaluate(NodeID to_evaluate) override;
    void reEvaluate(NodeID to_evaluate) override;
    void reset() override { m_last_node_id = std::nullopt; }
    NodeID getIDofLastEvaluatedNode() const override { return m_last_node_id.value(); }
    bool isEvalComputed() const override { return m_last_node_id.has_value(); }
    double getCachedEval(NodeID node_id) const override;
    bool getCachedIsDeadEnd(NodeID /* node_id */) const override { return false; }
    void setCachedEval(NodeID node_id, double eval) override;
    void setIsDeadEnd(NodeID node_id, bool is_dead_end) override;
    std::vector<NodeEvaluator<State_t, Action_t>*> getSubEvaluators() const override { return {}; }

    // Overriden public SettingsLogger methods
    std::string getName() const override { return CLASS_NAME; }

protected:
    // Overriden protected SettingsLogger methods
    StringMap getComponentSettings() const override { return {}; }
    SearchSettingsMap getSubComponentSettings() const override { return {}; }

private:
    const NodeContainer<State_t, Action_t>* m_nodes = nullptr;  ///< The node container used to access g-cost values
    std::optional<NodeID> m_last_node_id = std::nullopt;  ///< The ID of the last node evaluated
};

template<class State_t, class Action_t>
void GCostEvaluator<State_t, Action_t>::evaluate(NodeID to_evaluate) {
    assert(!m_last_node_id.has_value() || m_last_node_id.value() == to_evaluate);
    m_last_node_id = to_evaluate;
}

template<class State_t, class Action_t>
void GCostEvaluator<State_t, Action_t>::reEvaluate(NodeID to_evaluate) {
    assert(!m_last_node_id.has_value() || m_last_node_id.value() == to_evaluate);
    m_last_node_id = to_evaluate;
}

template<class State_t, class Action_t>
double GCostEvaluator<State_t, Action_t>::getCachedEval(NodeID node_id) const {
    assert(m_nodes);
    return m_nodes->getGValue(node_id);
}

template<class State_t, class Action_t>
void GCostEvaluator<State_t, Action_t>::setCachedEval(NodeID /* node_id */, double /* eval */) {
    throw std::logic_error("Cannot set the g-cost using GCostEvaluator's setCachedEval");
}

template<class State_t, class Action_t>
void GCostEvaluator<State_t, Action_t>::setIsDeadEnd(NodeID /* node_id */, bool /* is_dead_end */) {
    throw std::logic_error("Cannot call setIsDeadEnd of a GCostEvaluator");
}

#endif  //G_COST_EVALUATOR_H_
