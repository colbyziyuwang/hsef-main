#ifndef EVALUATION_CACHE_H_
#define EVALUATION_CACHE_H_

#include "search_basics/node_container.h"

#include <cstddef>
#include <vector>

/**
 * A cache for node evaluations. Internally, stores the evaluations in a vector, indexed by the ID of a node.
 */
class EvaluationCache {

public:
    /**
     * Creates an evaluation cache.
     */
    EvaluationCache() = default;

    /**
     * Sets the cached values of the evaluation and is dead end value of the node with the given ID.
     *
     * @param node_id The ID of the node to set values for
     * @param value The new node evaluation
     * @param is_dead_end The new value for is dead end
     */
    void setValues(NodeID node_id, double value, bool is_dead_end);

    /**
     * Sets the evaluation of the node with the given ID.
     *
     * If the ID is outside the current range of IDs considered, the evaluation of all IDs below this ID that
     * have not previously been set will be set to 0.0.
     *
     * @param node_id The ID of the node whose value is to be set.
     * @param value The evaluation to store for the corresponding node.
     */
    void setEvaluation(NodeID node_id, double value);

    /**
     * Sets the value for whether a node is a dead end for the node with the given ID.
     *
     * If the ID is outside the current range of IDs considered, then the nodes with all IDs below this ID, who
     * have not had a value for dead end set, will be set to false.
     *
     * @param node_id The ID of the node whose value is to be set.
     * @param is_dead_end Whether the node is identified as a dead end or not.
     */
    void setIsDeadEnd(NodeID node_id, bool is_dead_end);

    /**
     * Gets the evaluation of the node with the given ID.
     *
     * @param node_id The ID of the node whose value is required.
     * @return The evaluation of the node with the corresponding ID.
     */
    double getEvaluation(NodeID node_id) const;

    /**
     * Gets whether the node with the given ID is a dead end.
     *
     * @param node_id The ID of the node whose value is required.
     * @return Whether the node for the corresponding ID is a dead end.
     */
    bool getIsDeadEnd(NodeID node_id) const;

    /**
     * Clears the cache of all stored evaluations.
     */
    void clearCache();

    /**
     * Gets the number of values stored in the cache.
     * @return
     */
    std::size_t size() const { return m_evals.size(); }

private:
    void updateCacheSizesForSet(NodeID node_id);

    std::vector<double> m_evals;  ///< The evaluations of all nodes, indexed by their ID
    std::vector<bool> m_is_dead_ends;  ///< Whether each node is a dead end, indexed by their ID
};
#endif  //EVALUATION_CACHE_H_
