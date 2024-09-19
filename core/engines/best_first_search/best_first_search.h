#ifndef BEST_FIRST_SEARCH_H_
#define BEST_FIRST_SEARCH_H_

#include "building_tools/hashing/state_hash_function.h"
#include "engines/best_first_search/best_first_search_params.h"
#include "engines/engine_components/node_containers/node_list.h"
#include "engines/engine_components/open_lists/evaluator_and_comparing_usage.h"
#include "engines/engine_components/open_lists/heap_based_open_list.h"
#include "engines/single_step_search_engine.h"
#include "logging/logging_terms.h"
#include "logging/search_component_settings.h"
#include "search_basics/node_container.h"
#include "search_basics/node_evaluator.h"
#include "search_basics/search_engine.h"
#include "utils/floating_point_utils.h"

#include <cassert>
#include <cfloat>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

/**
 *
 * An template for best-first search.
*
 * @class BestFirstSearch
 */
template<class State_t, class Action_t, class Hash_t>
class BestFirstSearch : public SingleStepSearchEngine<State_t, Action_t> {
    using SE = SingleStepSearchEngine<State_t, Action_t>;  // Allows succinct access to the protected members
    using NodeMap = std::unordered_map<Hash_t, NodeID>;  ///< Defines the type for a map.

public:
    /**
     * Constructor for Best First Search
     */
    explicit BestFirstSearch(const BestFirstSearchParams& params)
              : m_params(params) {}

    /**
     * Default destructor
     */
    virtual ~BestFirstSearch() = default;

    /**
     * Sets the hash function used by the search.
     *
     * @param hash The new hash function
     */
    void setHashFunction(const StateHashFunction<State_t, Hash_t>& hash);

    /**
     * Sets the evaluation function used by the search.
     *
     * @param evaluator The evaluator to use
     */
    void setEvaluator(NodeEvaluator<State_t, Action_t>& evaluator);

    /**
     * Sets the evaluators to be used in the order given.
     *
     * @param evaluators The evaluators to use
     */
    void setEvaluators(const EvalsAndUsageVec<State_t, Action_t>& evaluators);

    /**
     * Gets the current open list being examined by the search.
     *
     * @return The current open list.
     */
    const HeapBasedOpenList<State_t, Action_t>& getOpenList() const { return m_open_list; }

    /**
     * Gets the list of nodes.
     *
     * @return The list of nodes.
     */
    const NodeList<State_t, Action_t>& getNodes() const { return m_nodes; }

    /**
     * Gets the number of nodes in the open list.
     *
     * @return The number of nodes in the open list.
     */
    std::size_t getOpenListSize() const { return m_open_list.getSize(); }

    /**
     * Gets the number of nodes in the closed list.
     *
     * @return The number of nodes in the closed list.
     */
    std::size_t getClosedListSize() const { return m_nodes.size() - m_open_list.getSize(); }

    /**
     * Returns the ID of the node with the given hash value
     *
     * Returns std::nullopt if the node does not exist
     *
     * @param hash_value The hash value searching for
     * @return The node ID associated with the given hash value or null value.
     */
    std::optional<NodeID> getNodeID(Hash_t hash_value) const;

    /**
     * Gets the current action list being examined by the search.
     *
     * @return The current open list.
     */
    const std::vector<Action_t>& getCurrentActionList() { return m_app_actions; }

    /**
     * Set the BFS params by input
     */
    void setEngineParams(const BestFirstSearchParams& params);

    /**
     * Gets the list of node IDs in the order they were expanded.
     *
     * @return The order of node expansions.
     */
    const std::vector<NodeID>& getExpansionOrder() { return m_expansion_order; }

    /**
     * Gets the ID of the last node expanded.
     *
     * @return The ID of the last node expanded
     */
    NodeID getLastExpandedNodeID() { return m_last_expanded_node_id; }

    // Overridden public SearchEngine  methods
    StringMap getEngineSpecificStatistics() const override;
    std::vector<NodeEvaluator<State_t, Action_t>*> getBaseEvaluators() const override { return {m_evaluators[0].m_evaluator}; }

    // Overidden public SettingsLogger methods
    std::string getName() const override { return "BestFirstSearch"; }

protected:
    // Overridden SingleStepSearchEngine methods
    bool doCanRunSearch() const override { return m_evaluators.size() > 0 && m_hash_func; }
    void doReset() override;
    StringMap getEngineParamsLog() const override { return m_params.getParameterLog(); }
    void doSearchInitialization(const State_t& initial_state) override;
    EngineStatus doSingleSearchStep() override;
    StringMap getComponentSettings() const override;

    // Overidden protected SettingsLogger methods
    SearchSettingsMap getSubComponentSettings() const override;

private:
    BestFirstSearchParams m_params;  ///< The params to set BFS
    EvalsAndUsageVec<State_t, Action_t> m_evaluators;
    const StateHashFunction<State_t, Hash_t>* m_hash_func = nullptr;  ///< The hash function.
    NodeMap m_node_map;  ///< The map used to determine if a hash value is already associated with a node.

    NodeList<State_t, Action_t> m_nodes;  ///< The list of nodes
    HeapBasedOpenList<State_t, Action_t> m_open_list;  ///< The open list
    NodeID m_last_expanded_node_id = 0;  ///< Stores last expanded node ID

    int64_t m_num_reex = 0;  ///< The number of re-expansions
    int64_t m_num_reopenings = 0;  ///< The number of reopenings

    std::vector<Action_t> m_app_actions;  ///< A vector to store the set of applicable actions.

    std::vector<NodeID> m_expansion_order;  ///< A vector to store the order of the expanded node IDs
    std::vector<NodeID> m_children;  ///< The indices corresponding to the children of the current node
    std::vector<int> m_node_expansion_count;  ///< The number of times each node was expanded
};

template<class State_t, class Action_t, class Hash_t>
StringMap BestFirstSearch<State_t, Action_t, Hash_t>::getEngineSpecificStatistics() const {
    StringMap stats = SingleStepSearchEngine<State_t, Action_t>::getEngineSpecificStatistics();
    stats["num_reexpansions"] = std::to_string(m_num_reex);
    stats["num_reopenings"] = std::to_string(m_num_reopenings);

    return stats;
}

template<class State_t, class Action_t, class Hash_t>
inline void BestFirstSearch<State_t, Action_t, Hash_t>::setHashFunction(const StateHashFunction<State_t, Hash_t>& hash) {
    m_hash_func = &hash;
    SE::reset();
}

template<class State_t, class Action_t, class Hash_t>
void BestFirstSearch<State_t, Action_t, Hash_t>::setEvaluators(const EvalsAndUsageVec<State_t, Action_t>& evaluators) {
    m_evaluators = evaluators;

    for (auto& eval_and_usage : evaluators) {
        eval_and_usage.m_evaluator->setNodeContainer(m_nodes);
    }

    m_open_list.setEvaluators(m_evaluators);
    SE::reset();
}

template<class State_t, class Action_t, class Hash_t>
void BestFirstSearch<State_t, Action_t, Hash_t>::setEvaluator(NodeEvaluator<State_t, Action_t>& evaluator) {
    EvalsAndUsageVec<State_t, Action_t> evals;
    evals.emplace_back(evaluator, true);
    setEvaluators(evals);
}

template<class State_t, class Action_t, class Hash_t>
void BestFirstSearch<State_t, Action_t, Hash_t>::setEngineParams(const BestFirstSearchParams& params) {
    m_params = params;
    SE::reset();
}

template<class State_t, class Action_t, class Hash_t>
void BestFirstSearch<State_t, Action_t, Hash_t>::doSearchInitialization(const State_t& initial_state) {
    Hash_t init_hash = m_hash_func->getHashValue(initial_state);
    NodeID init_id = m_nodes.addNode(initial_state);
    m_node_map[init_hash] = init_id;

    SE::evaluateNode(init_id);

    m_open_list.addToOpen(init_id);
    m_node_expansion_count.resize(1, 0);
}

template<class State_t, class Action_t, class Hash_t>
EngineStatus BestFirstSearch<State_t, Action_t, Hash_t>::doSingleSearchStep() {
    if (m_open_list.isEmpty()) {
        return EngineStatus::not_ready;  // TODO: This should be search completed, but needs testing
    }

    NodeID to_expand_id = m_open_list.getAndRemoveIDOfBestNode();
    m_node_expansion_count[to_expand_id]++;
    if (m_node_expansion_count[to_expand_id] > 1) {
        m_num_reex++;
    }

    if (m_params.m_store_expansion_order) {
        m_expansion_order.push_back(to_expand_id);
    }
    m_last_expanded_node_id = to_expand_id;

    if (SE::isGoal(m_nodes.getState(to_expand_id))) {  // checks for goal
        SE::setIncumbentSolution(to_expand_id, m_nodes);
        return EngineStatus::search_completed;
    }

    double parent_g = m_nodes.getGValue(to_expand_id);

    m_app_actions.clear();
    m_children.clear();

    m_app_actions = SE::getApplicableActions(m_nodes.getState(to_expand_id));

    for (unsigned i = 0; i < m_app_actions.size(); i++) {
        double current_action_cost = SE::getActionCost(m_nodes.getState(to_expand_id), m_app_actions[i]);
        double child_g = parent_g + current_action_cost;

        State_t child_state = SE::getChildState(m_nodes.getState(to_expand_id), m_app_actions[i]);

        Hash_t child_hash = m_hash_func->getHashValue(child_state);
        std::optional<NodeID> possible_child_id = getNodeID(child_hash);

        if (possible_child_id) {  // Node is in open or closed
            NodeID child_id = possible_child_id.value();
            m_children.push_back(child_id);

            if (fpLess(child_g, m_nodes.getGValue(child_id))) {
                m_nodes.setGValue(child_id, child_g);
                m_nodes.setParentID(child_id, to_expand_id);
                m_nodes.setLastAction(child_id, m_app_actions[i]);
                m_nodes.setLastActionCost(child_id, current_action_cost);

                SE::reEvaluateNode(child_id);

                if (m_open_list.isNodeInOpen(child_id)) {
                    m_open_list.evalChanged(child_id);
                } else if (m_params.m_use_reopened) {
                    m_num_reopenings++;
                    m_open_list.addToOpen(child_id);
                }
            }
        } else {
            if (SE::hasHitResourceLimit()) {
                return EngineStatus::resource_limit_hit;
            }

            NodeID child_id = m_nodes.addNode(child_state, to_expand_id, child_g, m_app_actions[i], current_action_cost);
            m_node_map[child_hash] = child_id;
            m_children.push_back(child_id);

            SE::evaluateNode(child_id);

            m_open_list.addToOpen(child_id);
        }
    }

    if (m_nodes.size() > m_node_expansion_count.size()) {
        m_node_expansion_count.resize(m_nodes.size(), 0);
    }
    return EngineStatus::active;
}

template<class State_t, class Action_t, class Hash_t>
void BestFirstSearch<State_t, Action_t, Hash_t>::doReset() {
    m_open_list.clear();
    m_node_map.clear();
    m_app_actions.clear();
    m_expansion_order.clear();
    m_nodes.clear();
    m_node_expansion_count.clear();

    m_num_reex = 0;
    m_num_reopenings = 0;
}

template<class State_t, class Action_t, class Hash_t>
StringMap BestFirstSearch<State_t, Action_t, Hash_t>::getComponentSettings() const {
    auto se_log = SE::getComponentSettings();
    auto params_log = m_params.getParameterLog();

    for (const auto& [key, value] : params_log) {
        se_log[key] = value;
    }

    return se_log;
}

template<class State_t, class Action_t, class Hash_t>
SearchSettingsMap BestFirstSearch<State_t, Action_t, Hash_t>::getSubComponentSettings() const {
    SearchSettingsMap sub_components;

    sub_components["eval_function"] = m_evaluators[0].m_evaluator->getAllSettings();
    sub_components["hash_function"] = m_hash_func->getAllSettings();

    return sub_components;
}

template<class State_t, class Action_t, class Hash_t>
std::optional<NodeID> BestFirstSearch<State_t, Action_t, Hash_t>::getNodeID(Hash_t hash_value) const {
    assert(m_nodes.size() == m_node_map.size());

    auto node_check = m_node_map.find(hash_value);

    if (node_check == m_node_map.end()) {
        return std::nullopt;
    }

    return node_check->second;
}
#endif  //BEST_FIRST_SEARCH_H_