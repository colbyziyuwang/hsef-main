#ifndef NODE_EVALUATOR_H_
#define NODE_EVALUATOR_H_

#include "logging/settings_logger.h"
#include "search_basics/node_container.h"

#include <vector>

/**
 * Defines the interface for a function which evaluates a node. This could be a heuristic function or a more
 * complicated evaluator
 *
 * Evaluating a node requires two function calls:
 *
 * prepareToEvaluate -> prepares the evaluator to evaluate a new node
 * evaluate -> performs the actual evaluation.
 *
 * The first of these prepares the function and empties any cached values. The second then computes the evaluation.
 * However, evaluate will ignore the given node if doPrepare is not called. THIS IS INTENDED BEHAVIOUR.
 * It allows for an evaluator to be called in multiple locations  for the same node (ie. as part of other evaluators),
 * while only needing to be computed once.
 *
 * Notice that a node is specified by a NodeID. The ID allows for information for the node to be found at the given
 * index of a NodeContainer. The evaluator should cache evaluations so that they are easily accessed by ID.
 *
 * @class NodeEvaluator
 */
template<class State_t, class Action_t>
class NodeEvaluator : public SettingsLogger {
public:
    /**
     * Sets the container of nodes that this evaluator will be applied to.
     *
     * @param nodes The container of nodes this evaluator will be applied to.
     */
    virtual void setNodeContainer(const NodeContainer<State_t, Action_t>& nodes) = 0;

    /**
     * Gets the container of nodes that this evaluator is applied to.
     *
     * @return The container of nodes this evaluator is applied to.
     */
    virtual const NodeContainer<State_t, Action_t>* getNodeContainer() const = 0;

    /**
     * Prepares the evaluator for evaluating (or re-evaluating) the next node.
     *
     * This function is intended to allow for any sort of clearing process if anything was
     * stored during the calculation of the previous evaluation that is not needed.
     *
     * Once this method is called, isEvalComputed should return false until evaluate or reEvaluate
     * is called.
     */
    virtual void prepareToEvaluate() = 0;

    /**
     * Evaluates the node with the given ID in the given container, and caches the result so that
     * it can be accessed using other methods.
     *
     * If there is already information cached for this node, it is ignored.
     *
     * isEvalComputed should return true after this method is called, and getLastNodeID should be
     * able to return the ID of the last node that was evaluate.
     *
     * @param to_evaluate The ID of the node to evaluate
     */
    virtual void evaluate(NodeID to_evaluate) = 0;

    /**
     * Evaluates the node with the given ID in the given container, and caches the result so that
     * it can be accessed using other methods. Assumes that there is already a cached value for this
     *
     * Assumes that there is already an evaluation cached for this node. It may or may not be ignored
     * depending on the evaluator.
     *
     * isEvalComputed should return true after this method is called, and getLastNodeID should be
     * able to return the ID of the last node that was evaluate.
     *
     * @param to_evaluate The ID of the node to re-evaluate
     */
    virtual void reEvaluate(NodeID to_evaluate) = 0;

    /**
     * Resets the evaluator. This clears the cache and any other stored values.
     */
    virtual void reset() = 0;

    /**
     * Returns if an evaluation has been computed, or if the evaluator has been prepared for a new evaluation
     *
     * @return If an evaluation has been computed and cached, or if the evaluator is prepared for a new evaluation
     */
    virtual bool isEvalComputed() const = 0;

    /**
     * Returns the ID of the last node evaluation computed.
     *
     * @return The ID of the last node evaluation.
     */
    virtual NodeID getIDofLastEvaluatedNode() const = 0;

    /**
     * Returns the evaluation that has been stored for the node with the given id.
     *
     * @param node_id The id node id
     * @return The stored evaluation for the node with the given id
     */
    virtual double getCachedEval(NodeID node_id) const = 0;

    /**
     * Returns the stored result of if the node with the given ID is a dead-end.
     *
     * @param node_id The ID of the node
     * @return If the node with the given ID is a dead-end
     */
    virtual bool getCachedIsDeadEnd(NodeID node_id) const = 0;

    /**
     * Sets the cached value of the node with the given ID.
     *
     * @param node_id The ID of the node to update
     * @param eval The new evaluation
     */
    virtual void setCachedEval(NodeID node_id, double eval) = 0;

    /**
     * Sets the is dead end value of the node with the given ID.
     *
     * @param node_id The ID of the node to update
     * @param is_dead_end The new value for is dead end
     */
    virtual void setIsDeadEnd(NodeID node_id, bool is_dead_end) = 0;

    /**
     * Returns the list of evaluators that this evaluator depends on to calculate its evaluation.
     *
     * Only returns immediate sub-evaluators, not also the sub-evaluators of the sub-evaluators (etc.).
     *
     * @return The sub-evaluators of this evaluator.
     */
    virtual std::vector<NodeEvaluator<State_t, Action_t>*> getSubEvaluators() const = 0;

    /**
     * Returns the last node evaluation computed.
     *
     * @return The last node evaluation.
     */
    virtual double getLastNodeEval() const { return getCachedEval(getIDofLastEvaluatedNode()); }

    /**
     * Returns if the last evaluation determined the last node was a dead end.
     *
     * @return If the last node was evaluated as a dead end
     */
    virtual bool isLastNodeADeadEnd() const { return getCachedIsDeadEnd(getIDofLastEvaluatedNode()); }
};

#endif /* NODE_EVALUATOR_H_ */
