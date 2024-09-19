#include "building_tools/evaluators/evaluation_cache.h"
#include "search_basics/node_container.h"

#include <cassert>


void EvaluationCache::setValues(NodeID node_id, double value, bool is_dead_end) {
    updateCacheSizesForSet(node_id);

    m_evals[node_id] = value;
    m_is_dead_ends[node_id] = is_dead_end;
}

void EvaluationCache::setEvaluation(NodeID node_id, double value) {
    updateCacheSizesForSet(node_id);

    m_evals[node_id] = value;
}

void EvaluationCache::setIsDeadEnd(NodeID node_id, bool is_dead_end) {
    updateCacheSizesForSet(node_id);

    m_is_dead_ends[node_id] = is_dead_end;
}

double EvaluationCache::getEvaluation(NodeID node_id) const {
    assert(m_evals.size() > node_id);
    return m_evals[node_id];
}

bool EvaluationCache::getIsDeadEnd(NodeID node_id) const {
    assert(m_is_dead_ends.size() > node_id);
    return m_is_dead_ends[node_id];
}

void EvaluationCache::clearCache() {
    m_evals.clear();
    m_is_dead_ends.clear();
}

void EvaluationCache::updateCacheSizesForSet(NodeID node_id) {
    assert(m_evals.size() == m_is_dead_ends.size());
    if (node_id >= m_evals.size()) {
        m_evals.resize(node_id + 1, 0.0);
        m_is_dead_ends.resize(node_id + 1, false);
    }
    assert(m_evals.size() == m_is_dead_ends.size());
}
