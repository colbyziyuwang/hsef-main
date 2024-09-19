#ifndef NODE_LIST_H_
#define NODE_LIST_H_

#include "search_basics/node_container.h"

#include <cassert>
#include <cstdio>
#include <optional>
#include <vector>

/**
 * A list of search nodes that maintains a hash map that can be used to tell if items are already held in the list.
 */
template<class State_t, class Action_t>
class NodeList : public NodeContainer<State_t, Action_t> {

public:
    /**
     * The constructor for the node list. Does nothing.
     */
    NodeList() = default;

    /**
     * Destructor for the node list. Does nothing.
     */
    virtual ~NodeList() = default;

    // ovverridden public NodeContainer methods
    NodeID addNode(const State_t& state) override;
    NodeID addNode(const State_t& state, NodeID parent_id, double g_cost, const Action_t& last_action, double last_action_cost) override;
    const State_t& getState(NodeID node_id) const override;
    const std::optional<Action_t>& getLastAction(NodeID node_id) const override;
    void setLastAction(NodeID node_id, const std::optional<Action_t>& action) override;
    double getLastActionCost(NodeID node_id) const override;
    void setLastActionCost(NodeID node_id, double last_action_cost) override;
    NodeID getParentID(NodeID node_id) const override;
    void setParentID(NodeID node_id, NodeID parent_id) override;
    double getGValue(NodeID node_id) const override;
    void setGValue(NodeID node_id, double g_value) override;
    void clear() override;
    std::size_t size() const override { return m_states.size(); };

    /**
     * Pops off the last element of the node list.
     */
    void popBack();

private:
    std::vector<State_t> m_states;  ///< The list of states being stored
    std::vector<std::optional<Action_t>> m_last_actions;  ///< The list of last actions being stored
    std::vector<double> m_last_action_costs;  ///< The list of last action costs
    std::vector<NodeID> m_parent_ids;  ///< The list of parent IDs
    std::vector<double> m_g_values;  ///< The list of g-values
};

template<class State_t, class Action_t>
NodeID NodeList<State_t, Action_t>::addNode(const State_t& state, NodeID parent_id, double g_cost, const Action_t& last_action, double last_action_cost) {
    NodeID new_node_id = m_states.size();
    m_states.emplace_back(state);
    m_last_actions.emplace_back(last_action);
    m_last_action_costs.emplace_back(last_action_cost);
    m_parent_ids.emplace_back(parent_id);
    m_g_values.emplace_back(g_cost);
    return new_node_id;
}

template<class State_t, class Action_t>
NodeID NodeList<State_t, Action_t>::addNode(const State_t& state) {
    NodeID new_node_id = m_states.size();
    m_states.emplace_back(state);
    m_last_actions.emplace_back(std::nullopt);
    m_last_action_costs.emplace_back(0);
    m_parent_ids.emplace_back(0);
    m_g_values.emplace_back(0);
    return new_node_id;
}

template<class State_t, class Action_t>
void NodeList<State_t, Action_t>::clear() {
    m_states.clear();
    m_last_actions.clear();
    m_last_action_costs.clear();
    m_parent_ids.clear();
    m_g_values.clear();
}

template<class State_t, class Action_t>
const State_t& NodeList<State_t, Action_t>::getState(NodeID node_id) const {
    assert(node_id < m_states.size());
    return m_states[node_id];
}

template<class State_t, class Action_t>
const std::optional<Action_t>& NodeList<State_t, Action_t>::getLastAction(NodeID node_id) const {
    assert(node_id < m_last_actions.size());
    return m_last_actions[node_id];
}

template<class State_t, class Action_t>
void NodeList<State_t, Action_t>::setLastAction(NodeID node_id, const std::optional<Action_t>& action) {
    assert(node_id < m_last_actions.size());
    m_last_actions[node_id] = action;
}

template<class State_t, class Action_t>
double NodeList<State_t, Action_t>::getLastActionCost(NodeID node_id) const {
    assert(node_id < m_last_action_costs.size());
    return m_last_action_costs[node_id];
}

template<class State_t, class Action_t>
void NodeList<State_t, Action_t>::setLastActionCost(NodeID node_id, double last_action_cost) {
    assert(node_id < m_last_action_costs.size());
    m_last_action_costs[node_id] = last_action_cost;
}

template<class State_t, class Action_t>
NodeID NodeList<State_t, Action_t>::getParentID(NodeID node_id) const {
    assert(node_id < m_parent_ids.size());
    return m_parent_ids[node_id];
}

template<class State_t, class Action_t>
void NodeList<State_t, Action_t>::setParentID(NodeID node_id, NodeID parent_id) {
    assert(node_id < m_parent_ids.size());
    m_parent_ids[node_id] = parent_id;
}

template<class State_t, class Action_t>
double NodeList<State_t, Action_t>::getGValue(NodeID node_id) const {
    assert(node_id < m_g_values.size());
    return m_g_values[node_id];
}

template<class State_t, class Action_t>
void NodeList<State_t, Action_t>::setGValue(NodeID node_id, double g_value) {
    assert(node_id < m_g_values.size());
    m_g_values[node_id] = g_value;
}

template<class State_t, class Action_t>
void NodeList<State_t, Action_t>::popBack() {
    assert(m_states.size() > 0);
    assert(m_states.size() == m_last_actions.size());
    assert(m_last_actions.size() == m_last_action_costs.size());
    assert(m_last_action_costs.size() == m_parent_ids.size());
    assert(m_parent_ids.size() == m_g_values.size());

    m_states.pop_back();
    m_last_actions.pop_back();
    m_last_action_costs.pop_back();
    m_parent_ids.pop_back();
    m_g_values.pop_back();
}
#endif /* NODE_LIST_H_ */