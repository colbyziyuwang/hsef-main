#ifndef NODE_CONTAINER_H_
#define NODE_CONTAINER_H_

#include <cstdlib>
#include <optional>

using NodeID = std::size_t;  ///< ID of a node is a unique value for each

/**
 * A container of nodes, where each node is given a unique ID that they can be accessed according to.
 *
 * @tparam State_t The type for the states in the node
 * @tparam Action_t The type for the actions in the node
 */
template<class State_t, class Action_t>
class NodeContainer {

public:
    /**
     * Adds a new search node for the given state. All other fields are given default values.
     *
     * Intended for initial states.
     *
     * @param state The state to create a node for
     * @return The ID of the created node
     */
    virtual NodeID addNode(const State_t& state) = 0;

    /**
     * Adds a new search node corresponding to the given attributes.
     *
     * @param state The state for the new node
     * @param parent_id The ID of the parent of the created node
     * @param g_cost The g-cost of the created node
     * @param last_action The last action that generated this node
     * @param last_action_cost The cost of the last action that led to this node
     * @return The ID of the created node
     */
    virtual NodeID addNode(const State_t& state, NodeID parent_id, double g_cost, const Action_t& last_action, double last_action_cost) = 0;

    /**
     * Gets the state corresponding to the node with the given ID.
     *
     * Assumes that a node with the given ID is in the container.
     *
     * @param node_id The ID of the node of interest
     * @return The state of the node with the given ID
     */
    virtual const State_t& getState(NodeID node_id) const = 0;

    /**
     * Returns the action that led to the node with the given ID.
     *
     * Assumes that a node with the given ID is in the container.
     *
     * @param node_id The ID of the node of interest
     * @return The last action that led to the node with the given ID
     */
    virtual const std::optional<Action_t>& getLastAction(NodeID node_id) const = 0;

    /**
     * Sets the action that induced the given node to the new value.
     *
     * Assumes that a node with the given ID is in the container.
     *
     * @param node_id The ID of the node whose inducing action is to be updated
     * @param action The new action to be set to
     */
    virtual void setLastAction(NodeID node_id, const std::optional<Action_t>& action) = 0;

    /**
     * Returns the cost of the action that led to the state with the given ID.
     *
     * Assumes that a node with the given ID is in the container.
     *
     * @param node_id The ID of the node of interest
     * @return The cost of the action that led to the given state
     */
    virtual double getLastActionCost(NodeID node_id) const = 0;

    /**
     * Sets the action cost induced the given node to the new value.
     *
     * Assumes that a node with the given ID is in the container.
     *
     * @param node_id The ID of the node whose action cost is to be updated
     * @param last_action_cost The new value for the last action cost
     */
    virtual void setLastActionCost(NodeID node_id, double last_action_cost) = 0;

    /**
     * Returns the ID of the parent of the node with the given ID.
     *
     * Assumes that a node with the given ID is in the container.
     *
     * @param node_id The ID of the node of interest
     * @return The ID of the parent of the node with the given ID
     */
    virtual NodeID getParentID(NodeID node_id) const = 0;

    /**
     * Sets the parent ID of the node with the given ID to the given ID.
     *
     * Assumes that a node with the given ID is in the container.
     *
     * @param node_id The ID of the node whose parent ID is to be updated
     * @param parent_id The ID to update the parent ID to
     */
    virtual void setParentID(NodeID node_id, NodeID parent_id) = 0;

    /**
     * Returns the G-value of the node with the given ID.
     *
     * Assumes that a node with the given ID is in the container.
     *
     * @param node_id The ID of the node of interest
     * @return The G-valueof the node with the given ID
     */
    virtual double getGValue(NodeID node_id) const = 0;

    /**
     * Sets the g-value of the node with the given ID.
     *
     * Assumes that a node with the given ID is in the container.
     *
     * @param node_id The ID of the node whose g-value is to be updated
     * @param g_value The new value for the g-value
     */
    virtual void setGValue(NodeID node_id, double g_value) = 0;

    /**
     * Clears the node container.
     */
    virtual void clear() = 0;

    /**
     * Returns the number of nodes in the container
     *
     * @return The number of nodes in the container
     */
    virtual std::size_t size() const = 0;
};


#endif  //NODE_CONTAINER_H_
