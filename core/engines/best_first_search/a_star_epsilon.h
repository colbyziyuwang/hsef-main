#ifndef A_STAR_EPSILON_H_
#define A_STAR_EPSILON_H_

#include "a_star_epsilon_params.h"
#include "building_tools/hashing/state_hash_function.h"
#include "engines/engine_components/eval_functions/f_cost_evaluator.h"
#include "engines/engine_components/node_containers/node_list.h"
#include "engines/engine_components/open_lists/evaluator_and_comparing_usage.h"
#include "engines/engine_components/open_lists/heap_based_open_list.h"
#include "engines/single_step_search_engine.h"
#include "logging/logging_terms.h"
#include "logging/search_component_settings.h"
#include "search_basics/node_container.h"
#include "search_basics/node_evaluator.h"
#include "search_basics/search_engine.h"

#include <cassert>
#include <cfloat>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

/**
 * An A*-epsilon engine.
 *
 * @tparam State_t The type of a state
 * @tparam Action_t The type of an action
 * @tparam Hash_t The hash type. Used to define the hash function for type lookup.
 */
template<class State_t, class Action_t, class Hash_t>
class AStarEpsilon : public SingleStepSearchEngine<State_t, Action_t> {
    using SE = SingleStepSearchEngine<State_t, Action_t>;
    using NodeMap = std::unordered_map<Hash_t, NodeID>;  ///< Defines the type for a map.

public:
    /**
     * Creates an A Star Epsilon engine with the given parameters.
     * 
     * @param params The struct containing the engines parameters
     */
    explicit AStarEpsilon(const AStarEpsilonParams& params);

    /**
     * Default destructor
     */
    virtual ~AStarEpsilon() = default;

    /** 
    * Sets the Heuristic function used in the search
    * 
    * @param heuristic The heuristic function
    */
    void setHeuristic(NodeEvaluator<State_t, Action_t>& heuristic);

    /** 
     * Sets the Hash function used in the search
     * 
     * @param hash The new hash function
     */
    void setHashFunction(const StateHashFunction<State_t, Hash_t>& hash);

    /**
     * Set the A Star Epsilon params by input
     *  
     * @param params The struct containing the engines parameters
     */
    void setEngineParams(const AStarEpsilonParams& params);

    /** 
     * Returns the maximum f-cost of any node in the focal list
     */
    double getMaxEval() const;

    /**
     * Gets the current open list being examined by the search.
     *
     * @return The current open list.
     */
    const HeapBasedOpenList<State_t, Action_t>& getOpenList() const { return m_open_list; }

    /**
     * Gets the list of nodes.
     *
     * @return The list of nodes
     */
    const NodeList<State_t, Action_t>& getNodes() const { return m_nodes; }

    /**
     * Returns the focal list.
     *
     * @return The focal list.
     */
    const HeapBasedOpenList<State_t, Action_t>& getFocalList() const { return m_focal; }

    /**
     * Returns the list of node in open, but not in focal.
     *
     * @return The list of node in open, but not in focal.
     */
    const HeapBasedOpenList<State_t, Action_t>& getNotInFocalList() const { return m_not_in_focal; }

    /**
     * Returns the size of the open list.
     *
     * @return The size of the open list.
     */
    std::size_t getOpenListSize() const { return m_open_list.getSize(); }

    /**
     * Returns the size of the closed list.
     *
     * @return The size of the closed list.
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
     * Returns the f-cost evaluator.
     *
     * @return The f-cost evaluator
     */
    const FCostEvaluator<State_t, Action_t>& getFCostEvaluator() const { return *m_evaluator; }

    /**
     * Gets the list of node IDs in the order they were expanded.
     *
     * @return The order of node expansions
     */
    const std::vector<NodeID>& getExpansionOrder();

    /**
     * Gets the ID of the last node expanded.
     *
     * @return The ID of the last node expanded
     */
    NodeID getLastExpandedNodeID();

    // Overridden public SearchEngine methods
    StringMap getEngineSpecificStatistics() const override;
    std::vector<NodeEvaluator<State_t, Action_t>*> getBaseEvaluators() const override { return {m_heuristic, m_evaluator}; }

    // Overidden public SettingsLogger methods
    std::string getName() const override { return "AStarEpsilon"; }

protected:
    // Overridden SingleStepSearchEngine methods
    void doSearchInitialization(const State_t& initial_state) override;
    EngineStatus doSingleSearchStep() override;
    bool doCanRunSearch() const override;
    void doReset() override;
    StringMap getEngineParamsLog() const override;

    // Overidden protected SettingsLogger methods
    SearchSettingsMap getSubComponentSettings() const override;

private:
    /**
     * Updates the maximum evaluation of nodes in the focal list.
     */
    void updateMaxEval();

    /**
     * Gets all the possible children nodes from the node at the top of the focal heap.
     * Closes the parent node and adds the children to the open list.
     */
    void generateChildrenNodes();

    /**
     * Moves nodes to the focal list if their evaluation is less than or equal to
     * the evaluation of the best node in the open list multiplied by the weight.
     */
    void moveNodesToFocal();

    /**
     * Removes the top node from the focal list if it no longer satisfies the list's requirement.
     * Continues removing until the top node's evaluation is less than or equal to f min.
     */
    void moveNodesFromFocal();

    StringMap getComponentSettings() const override;

    AStarEpsilonParams m_params;
    NodeList<State_t, Action_t> m_nodes;  ///< The list of nodes
    NodeMap m_node_map;  ///< The map used to determine if a hash value is already associated with a node.
    HeapBasedOpenList<State_t, Action_t> m_open_list;  ///< The open list.
    HeapBasedOpenList<State_t, Action_t> m_focal;
    HeapBasedOpenList<State_t, Action_t> m_not_in_focal;
    NodeID m_last_expanded_node_id = 0;  ///< Stores last expanded node ID

    NodeEvaluator<State_t, Action_t>* m_heuristic = nullptr;  ///< The heuristic function
    // TODO Get m_evaluator off the heap
    FCostEvaluator<State_t, Action_t>* m_evaluator = nullptr;  ///< The evaluator
    const StateHashFunction<State_t, Hash_t>* m_hash_func = nullptr;  ///< The hash function

    int64_t m_num_reex = 0;  ///< The number of re-expansions
    int64_t m_num_reopenings = 0;  ///< The number of reopenings

    double m_max_eval = 0;  ///< The maximum f-cost of all open nodes in focal

    std::vector<NodeID> m_expansion_order;  ///< A vector to store the order of the expanded node IDs
    std::vector<NodeID> m_children;  ///< The indices corresponding to the children of the current node
    std::vector<double> m_edge_costs;  ///< The edge costs of all the children of the current node
};

template<class State_t, class Action_t, class Hash_t>
AStarEpsilon<State_t, Action_t, Hash_t>::AStarEpsilon(const AStarEpsilonParams& params)
          : m_params(params) {
    assert(params.m_weight >= 1);
}

template<class State_t, class Action_t, class Hash_t>
StringMap AStarEpsilon<State_t, Action_t, Hash_t>::getEngineSpecificStatistics() const {
    StringMap stats = SingleStepSearchEngine<State_t, Action_t>::getEngineSpecificStatistics();
    stats["num_reexpansions"] = std::to_string(m_num_reex);
    stats["num_reopenings"] = std::to_string(m_num_reopenings);

    return stats;
}

template<class State_t, class Action_t, class Hash_t>
void AStarEpsilon<State_t, Action_t, Hash_t>::setEngineParams(const AStarEpsilonParams& params) {
    assert(params.m_weight >= 1);
    m_params = params;
    SE::reset();
}

template<class State_t, class Action_t, class Hash_t>
void AStarEpsilon<State_t, Action_t, Hash_t>::doReset() {
    m_nodes.clear();
    m_node_map.clear();
    m_open_list.clear();
    m_focal.clear();
    m_not_in_focal.clear();
    m_expansion_order.clear();

    if (m_heuristic) {
        m_heuristic->reset();
    }

    m_num_reex = 0;
    m_num_reopenings = 0;
}

template<class State_t, class Action_t, class Hash_t>
inline void AStarEpsilon<State_t, Action_t, Hash_t>::setHashFunction(const StateHashFunction<State_t, Hash_t>& hash) {
    m_hash_func = &hash;
    SE::reset();
}

template<class State_t, class Action_t, class Hash_t>
void AStarEpsilon<State_t, Action_t, Hash_t>::setHeuristic(NodeEvaluator<State_t, Action_t>& heuristic) {
    m_heuristic = &heuristic;
    delete m_evaluator;
    m_evaluator = new FCostEvaluator<State_t, Action_t>(*m_heuristic);

    m_evaluator->setNodeContainer(m_nodes);

    EvalsAndUsageVec<State_t, Action_t> open_evals;
    EvalsAndUsageVec<State_t, Action_t> focal_evals;
    open_evals.emplace_back(m_evaluator, true);
    focal_evals.emplace_back(heuristic, true);

    m_open_list.setEvaluators(open_evals);
    m_focal.setEvaluators(focal_evals);
    m_not_in_focal.setEvaluators(open_evals);
    SE::reset();
}

template<class State_t, class Action_t, class Hash_t>
const std::vector<NodeID>& AStarEpsilon<State_t, Action_t, Hash_t>::getExpansionOrder() {
    return m_expansion_order;
}

template<class State_t, class Action_t, class Hash_t>
NodeID AStarEpsilon<State_t, Action_t, Hash_t>::getLastExpandedNodeID() {
    return m_last_expanded_node_id;
}

template<class State_t, class Action_t, class Hash_t>
void AStarEpsilon<State_t, Action_t, Hash_t>::updateMaxEval() {
    double best_node_eval = m_evaluator->getCachedEval(m_open_list.getIDOfBestNode());
    m_max_eval = m_params.m_weight * best_node_eval;
}

template<class State_t, class Action_t, class Hash_t>
inline bool AStarEpsilon<State_t, Action_t, Hash_t>::doCanRunSearch() const {
    return m_heuristic && m_hash_func;
}

template<class State_t, class Action_t, class Hash_t>
EngineStatus AStarEpsilon<State_t, Action_t, Hash_t>::doSingleSearchStep() {
    assert(SE::getStatus() == EngineStatus::active);
    assert(m_open_list.getSize() == m_focal.getSize() + m_not_in_focal.getSize());

    if (m_open_list.isEmpty()) {
        return EngineStatus::search_completed;
    }
    updateMaxEval();

    moveNodesToFocal();
    moveNodesFromFocal();

    assert(m_open_list.getSize() == m_focal.getSize() + m_not_in_focal.getSize());

    NodeID current_best_id = m_focal.getIDOfBestNode();

    if (m_params.m_store_expansion_order) {
        m_expansion_order.push_back(current_best_id);
    }

    m_last_expanded_node_id = current_best_id;

    if (SE::isGoal(m_nodes.getState(current_best_id))) {
        SE::setIncumbentSolution(current_best_id, m_nodes);
        return EngineStatus::search_completed;
    }
    generateChildrenNodes();

    return EngineStatus::active;
}

template<class State_t, class Action_t, class Hash_t>
void AStarEpsilon<State_t, Action_t, Hash_t>::doSearchInitialization(const State_t& initial_state) {
    Hash_t init_hash = m_hash_func->getHashValue(initial_state);
    NodeID init_id = m_nodes.addNode(initial_state);
    m_node_map[init_hash] = init_id;

    m_open_list.addToOpen(init_id);

    SE::evaluateNode(init_id);

    updateMaxEval();
    m_not_in_focal.addToOpen(init_id);
}


template<class State_t, class Action_t, class Hash_t>
void AStarEpsilon<State_t, Action_t, Hash_t>::generateChildrenNodes() {
    NodeID best_id = m_focal.getAndRemoveIDOfBestNode();
    m_open_list.removeFromHeap(best_id);

    std::vector<Action_t> applicable_actions = SE::getApplicableActions(m_nodes.getState(best_id));
    m_children.clear();
    m_edge_costs.clear();

    for (auto action : applicable_actions) {
        if (SE::hasHitResourceLimit()) {
            break;
        }
        State_t child_state = SE::getChildState(m_nodes.getState(best_id), action);
        Hash_t child_hash = m_hash_func->getHashValue(child_state);
        std::optional<NodeID> possible_child_id = getNodeID(child_hash);

        double current_action_cost = SE::getActionCost(m_nodes.getState(best_id), action);
        m_edge_costs.push_back(current_action_cost);

        double child_g_cost = m_nodes.getGValue(best_id) + current_action_cost;

        if (possible_child_id) {
            NodeID child_id = possible_child_id.value();
            m_children.push_back(child_id);

            if (fpLess(child_g_cost, m_nodes.getGValue(child_id))) {
                m_nodes.setGValue(child_id, child_g_cost);
                m_nodes.setParentID(child_id, best_id);
                m_nodes.setLastAction(child_id, action);
                m_nodes.setLastActionCost(child_id, current_action_cost);

                SE::reEvaluateNode(child_id);

                if (m_open_list.isNodeInOpen(child_id)) {
                    m_open_list.evalChanged(child_id);

                    if (m_focal.isNodeInOpen(child_id)) {
                        m_focal.evalChanged(child_id);
                    } else {
                        assert(m_not_in_focal.isNodeInOpen(child_id));
                        m_not_in_focal.evalChanged(child_id);
                    }

                } else if (m_params.m_use_reopened) {
                    m_open_list.addToOpen(child_id);
                    m_not_in_focal.addToOpen(child_id);
                    m_num_reopenings++;
                }
            }

        } else {
            NodeID node_id = m_nodes.addNode(child_state, best_id, child_g_cost, action, current_action_cost);
            m_node_map[child_hash] = node_id;
            m_children.push_back(node_id);

            SE::evaluateNode(node_id);

            m_open_list.addToOpen(node_id);
            m_not_in_focal.addToOpen(node_id);
        }
    }

    // TODO Parent updating is disabled for now
    //    if (m_params.m_parent_heuristic_updating) {
    //        // set the record to be maximum double value
    //        double record_min_heuristic = DBL_MAX;
    //        assert(m_children.size() == m_edge_costs.size());
    //
    //        // Calculate minimum for parent update based on children
    //        for (unsigned i = 0; i < m_children.size(); i++) {
    //            if (!m_node_table.getNode(m_children[i]).m_is_dead_end) {
    //                record_min_heuristic =
    //                          fpMin(record_min_heuristic, m_node_table.getNode(m_children[i]).m_evals[0] + m_edge_costs[i]);
    //            }
    //        }
    //
    //        if (record_min_heuristic != DBL_MAX) {
    //            Node<State_t, Action_t>& current_node = m_node_table.getNode(best_node_index);
    //
    //            if (fpGreater(record_min_heuristic, current_node.m_evals[0])) {
    //                current_node.m_evals[0] = record_min_heuristic;
    //                current_node.m_evals[1] = nodeEval(current_node.m_g_value, record_min_heuristic);
    //                SE::increaseParentUpdateCount();
    //            }
    //        } else {
    //            m_node_table.getNode(best_node_index).m_is_dead_end = true;
    //        }
    //    }
}

template<class State_t, class Action_t, class Hash_t>
void AStarEpsilon<State_t, Action_t, Hash_t>::moveNodesToFocal() {
    while (!m_not_in_focal.isEmpty() &&
              !fpGreater(m_evaluator->getCachedEval(m_not_in_focal.getIDOfBestNode()), m_max_eval)) {
        m_focal.addToOpen(m_not_in_focal.getAndRemoveIDOfBestNode());
    }
}

template<class State_t, class Action_t, class Hash_t>
void AStarEpsilon<State_t, Action_t, Hash_t>::moveNodesFromFocal() {
    while (!m_focal.isEmpty() &&
              fpGreater(m_evaluator->getCachedEval(m_focal.getIDOfBestNode()), m_max_eval)) {
        m_not_in_focal.addToOpen(m_focal.getAndRemoveIDOfBestNode());
    }
}

template<class State_t, class Action_t, class Hash_t>
inline double AStarEpsilon<State_t, Action_t, Hash_t>::getMaxEval() const {
    return m_max_eval;
}

template<class State_t, class Action_t, class Hash_t>
StringMap AStarEpsilon<State_t, Action_t, Hash_t>::getEngineParamsLog() const {
    return m_params.getParameterLog();
}

template<class State_t, class Action_t, class Hash_t>
StringMap AStarEpsilon<State_t, Action_t, Hash_t>::getComponentSettings() const {
    auto se_log = SE::getComponentSettings();
    auto params_log = m_params.getParameterLog();

    for (const auto& [key, value] : params_log) {
        se_log[key] = value;
    }

    return se_log;
}

template<class State_t, class Action_t, class Hash_t>
SearchSettingsMap AStarEpsilon<State_t, Action_t, Hash_t>::getSubComponentSettings() const {
    SearchSettingsMap sub_components;

    sub_components["heuristic"] = m_heuristic->getAllSettings();
    sub_components["hash_function"] = m_hash_func->getAllSettings();

    return sub_components;
}

template<class State_t, class Action_t, class Hash_t>
std::optional<NodeID> AStarEpsilon<State_t, Action_t, Hash_t>::getNodeID(Hash_t hash_value) const {
    assert(m_nodes.size() == m_node_map.size());

    auto node_check = m_node_map.find(hash_value);

    if (node_check == m_node_map.end()) {
        return std::nullopt;
    }

    return node_check->second;
}

#endif  // A_STAR_EPSILON_H_
