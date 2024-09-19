#ifndef HASH_MAP_HEURISTIC_H_
#define HASH_MAP_HEURISTIC_H_

#include "building_tools/evaluators/node_evaluator_with_cache.h"
#include "building_tools/hashing/state_hash_function.h"
#include "evaluator_tools_terms.h"
#include "logging/logging_terms.h"
#include "logging/search_component_settings.h"
#include "search_basics/node_container.h"
#include "search_basics/node_evaluator.h"
#include "utils/string_utils.h"

#include <cassert>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

/**
 * A heuristic that is sotred as a hash table. Can be used to create user-defined heuristics.
 *
 * @class HashMapHeuristic
 */
template<class State_t, class Action_t, class Hash_t>
class HashMapHeuristic : public NodeEvaluatorWithCache<State_t, Action_t> {
    using NodeEval = std::pair<double, bool>;

public:
    inline static const std::string CLASS_NAME = "HashMapHeuristic";  ///< The name of this component

    /**
     * Creates a hash-based heuristic that used the given hash function to store heuristic values. Any state that
     * does not have entry in the table will have the given default value.
     *
     * Assumes the hash function has no collisions.
     *
     * @param hash_function The hash function used to internally store heuristic values
     * @param default_h_value The heuristic value given to states that have no stored value
     */
    explicit HashMapHeuristic(const StateHashFunction<State_t, Hash_t>& hash_function,
              double default_h_value = 0.0);

    /**
     * Adds a heuristic value to the hash map.
     *
     * @param state The state to assign a heuristic value for
     * @param heuristic_value The heuristic value for the given state
     * @param is_dead_end Whether the state is identified as a dead end or not
     */
    void addHeuristicValue(const State_t& state, double heuristic_value, bool is_dead_end = false);

    // Overriden public NodeEvaluator functions
    std::vector<NodeEvaluator<State_t, Action_t>*> getSubEvaluators() const override { return {}; }

    // Overriden public SettingsLogger methods
    std::string getName() const override { return CLASS_NAME; }

protected:
    // Overriden protected NodeEvaluatorWithStorage functions
    void doPrepare() override {}
    void doEvaluateAndCache(NodeID to_evaluate) override;
    void doReEvaluateAndCache(NodeID /* to_evaluate */) override {}
    void doReset() override {}

    // Overriden protected SettingsLogger methods
    StringMap getComponentSettings() const override;
    SearchSettingsMap getSubComponentSettings() const override;

private:
    const StateHashFunction<State_t, Hash_t>* m_hash_function;  ///< The hash function used to find states
    std::unordered_map<Hash_t, NodeEval> m_heuristic_map;  ///< Used to store heuristic values
    double m_default_h_value;  ///< The default heuristic value for any state not entered
};

template<class State_t, class Action_t, class Hash_t>
HashMapHeuristic<State_t, Action_t, Hash_t>::HashMapHeuristic(
          const StateHashFunction<State_t, Hash_t>& hash_function, double default_h_value)
          : m_hash_function(&hash_function), m_default_h_value(default_h_value) {
    assert(hash_function.isPerfectHashFunction());
}

template<class State_t, class Action_t, class Hash_t>
void HashMapHeuristic<State_t, Action_t, Hash_t>::doEvaluateAndCache(NodeID to_evaluate) {
    using NE = NodeEvaluatorWithCache<State_t, Action_t>;

    auto state_heuristic = m_heuristic_map.find(m_hash_function->getHashValue(NE::getNodeContainer()->getState(to_evaluate)));
    if (state_heuristic != m_heuristic_map.end()) {
        NE::setCachedValues(to_evaluate, (*state_heuristic).second.first, (*state_heuristic).second.second);
    } else {
        NE::setCachedValues(to_evaluate, m_default_h_value, false);
    }
}

template<class State_t, class Action_t, class Hash_t>
void HashMapHeuristic<State_t, Action_t, Hash_t>::addHeuristicValue(
          const State_t& state, double heuristic_value, bool is_dead_end) {
    m_heuristic_map[m_hash_function->getHashValue(state)] = {heuristic_value, is_dead_end};
}

template<class State_t, class Action_t, class Hash_t>
StringMap HashMapHeuristic<State_t, Action_t, Hash_t>::getComponentSettings() const {
    using namespace evaluatorToolsTerms;
    StringMap log;
    log[SETTING_DEFAULT_H_VALUE] = roundAndToString(m_default_h_value, 6);
    return log;
}

template<class State_t, class Action_t, class Hash_t>
SearchSettingsMap HashMapHeuristic<State_t, Action_t, Hash_t>::getSubComponentSettings() const {
    SearchSettingsMap sub_components;
    sub_components[evaluatorToolsTerms::SETTING_HASH_FUNCTION] = m_hash_function->getAllSettings();
    return sub_components;
}

#endif  //HASH_MAP_HEURISTIC_H_
