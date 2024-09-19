#ifndef COST_AND_DISTANCE_TO_GO_EVALUATOR_H_
#define COST_AND_DISTANCE_TO_GO_EVALUATOR_H_

#include "search_basics/node_container.h"
#include "search_basics/node_evaluator.h"

/**
 * This class is a subset of heuristics returns a cost-to-go evaluation when evaluate or getLastNodeEval is called, and
 * returns a distance-to-go evaluation when getLastDistanceToGoEval is called.
 */
template<class State_t, class Action_t>
class CostAndDistanceToGoEvaluator : virtual public NodeEvaluator<State_t, Action_t> {

public:
    /**
     * Returns the last distance-to-go estimate computed as part of a regular evaluation
     * calculation.
     *
     * @return The last distance-to-go estimate computed
     */
    virtual double getLastDistanceToGoEval() const = 0;

    /**
     * Get's the cached value of the distance-to-go evaluation.
     *
     * @param node_id The ID of the node
     * @return The distance-to-go evaluation of the node
     */
    virtual double getCachedDistanceToGoEval(NodeID node_id) const = 0;

    /**
     * Sets the cached value of the distance-to-go evaluation for the node with the given ID.
     *
     * @param node_id The ID of the node in question.
     * @param eval The evaluation to set the node to
     */
    virtual void setCachedDistanceToGoEval(NodeID node_id, double eval) = 0;
};

#endif  //COST_AND_DISTANCE_TO_GO_EVALUATOR_H_
