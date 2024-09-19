#ifndef NODE_EVALUATOR_WITH_CACHE_H_
#define NODE_EVALUATOR_WITH_CACHE_H_

#include "building_tools/evaluators/evaluation_cache.h"
#include "search_basics/node_container.h"
#include "search_basics/node_evaluator.h"

#include <cassert>
#include <optional>

/**
 * A NodeEvaluator with a built-in cache to store the evaluations.
 *
 * Added to simplify the implementation of new heuristics.
 *
 * @tparam State_t The type of state
 * @tparam Action_t The type of action
 */
template<class State_t, class Action_t>
class NodeEvaluatorWithCache : virtual public NodeEvaluator<State_t, Action_t> {
public:
    // Overriden public NodeEvaluator functions
    void setNodeContainer(const NodeContainer<State_t, Action_t>& nodes) override { m_nodes = &nodes; }
    const NodeContainer<State_t, Action_t>* getNodeContainer() const override { return m_nodes; }
    void prepareToEvaluate() override;
    void evaluate(NodeID to_evaluate) override;
    void reEvaluate(NodeID to_evaluate) override;
    void reset() override;
    NodeID getIDofLastEvaluatedNode() const override;
    bool isEvalComputed() const override { return m_last_eval_node_id.has_value(); }
    double getCachedEval(NodeID node_id) const override { return m_evals.getEvaluation(node_id); }
    bool getCachedIsDeadEnd(NodeID node_id) const override { return m_evals.getIsDeadEnd(node_id); }
    void setCachedEval(NodeID node_id, double eval) override { m_evals.setEvaluation(node_id, eval); }
    void setIsDeadEnd(NodeID node_id, bool is_dead_end) override { m_evals.setIsDeadEnd(node_id, is_dead_end); }

protected:
    /**
     * Sets the cached values for both the evaluation and whether the node is a dead end.
     *
     * @param node_id The ID of the node whose evaluation needs to be updated
     * @param eval The new value for the evaluation
     * @param is_dead_end Whether the node is a dead end or not
     */
    void setCachedValues(NodeID node_id, double eval, bool is_dead_end) { m_evals.setValues(node_id, eval, is_dead_end); }

private:
    /**
     * Performs the evaluator specific oart if the preparation.
     *
     * Should call prepare on any sub-evaluators.
     */
    virtual void doPrepare() = 0;

    /**
     * Performs the evaluator specific part of the evaluation. Also caches the value if need be.
     *
     * Should also call setCachedValues to set the values in the cache.
     *
     * @param to_evaluate The ID of the node to evaluate
     */
    virtual void doEvaluateAndCache(NodeID to_evaluate) = 0;

    /**
     * Performs the evaluator specific part of the re-evaluation. Caches the value if need be.
     *
     * May do nothing if re-evaluation is not needed. Otherwise should also call setCachedValues to set the values in the cache.
     *
     * @param to_evaluate The ID of the node to re-evaluate
     */
    virtual void doReEvaluateAndCache(NodeID to_evaluate) = 0;

    /**
     * Performs the evaluator specific part of the reset.
     *
     * Will often do nothing, but should clear any additional caches and call reset on any sub-evaluators.
     */
    virtual void doReset() = 0;

    const NodeContainer<State_t, Action_t>* m_nodes = nullptr;  ///< The container of nodes this evaluator is acting on
    std::optional<NodeID> m_last_eval_node_id = std::nullopt;  ///< The NodeID of the last evaluation computed
    EvaluationCache m_evals;  ///< The cache of node evaluations
};

template<class State_t, class Action_t>
void NodeEvaluatorWithCache<State_t, Action_t>::prepareToEvaluate() {
    assert(m_nodes);

    if (m_last_eval_node_id.has_value()) {
        m_last_eval_node_id = std::nullopt;

        doPrepare();
    }
    assert(!m_last_eval_node_id.has_value());
}

template<class State_t, class Action_t>
void NodeEvaluatorWithCache<State_t, Action_t>::evaluate(NodeID to_evaluate) {
    assert(m_nodes);

    if (!m_last_eval_node_id.has_value()) {
        doEvaluateAndCache(to_evaluate);

        m_last_eval_node_id = to_evaluate;
    }

    assert(m_last_eval_node_id.has_value());
    assert(m_last_eval_node_id.value() == to_evaluate);
}

template<class State_t, class Action_t>
void NodeEvaluatorWithCache<State_t, Action_t>::reEvaluate(NodeID to_evaluate) {
    assert(m_nodes);
    assert(to_evaluate < m_evals.size());

    if (!m_last_eval_node_id.has_value()) {
        doReEvaluateAndCache(to_evaluate);
        m_last_eval_node_id = to_evaluate;
    }

    assert(m_last_eval_node_id.has_value());
    assert(m_last_eval_node_id.value() == to_evaluate);
}

template<class State_t, class Action_t>
void NodeEvaluatorWithCache<State_t, Action_t>::reset() {
    m_evals.clearCache();
    m_last_eval_node_id = std::nullopt;

    doReset();
}

template<class State_t, class Action_t>
NodeID NodeEvaluatorWithCache<State_t, Action_t>::getIDofLastEvaluatedNode() const {
    assert(m_last_eval_node_id.has_value());
    return m_last_eval_node_id.value();
}

#endif  //NODE_EVALUATOR_WITH_CACHE_H_
