#ifndef ID_ENGINE_H_
#define ID_ENGINE_H_

#include "engines/engine_components/node_containers/node_list.h"
#include "engines/single_step_search_engine.h"
#include "id_engine_params.h"
#include "logging/logging_terms.h"
#include "logging/search_component_settings.h"
#include "search_basics/node_container.h"
#include "search_basics/node_evaluator.h"
#include "search_basics/search_engine.h"
#include "utils/floating_point_utils.h"
#include "utils/random_gen_utils.h"

#include <cassert>
#include <string>
#include <vector>

/**
 * Defines an iterative deepening engine.
 *
 * TODO Add storage resource limits
 * @class IDEngine
 */
template<class State_t, class Action_t>
class IDEngine : public SingleStepSearchEngine<State_t, Action_t> {
    using SE = SingleStepSearchEngine<State_t, Action_t>;  // Allows succinct access to the protected members

public:
    /**
     * Creates an iterative deepening engine with the given parameters.
     *
     * @param params The parameters to use for the search
     */
    explicit IDEngine(const IDEngineParams& params);

    /**
     * Updates the engine parameters. Resets the engine as well.
     *
     * @param params The new parameters
     */
    void setEngineParams(const IDEngineParams& params);

    /**
     * Sets the evaluation function to use by the engine.
     *
     * @param evaluator The evaluation function to use.
     */
    void setEvaluator(NodeEvaluator<State_t, Action_t>& evaluator);

    /**
     * Gets the list of nodes defining the current state of the search
     *
     * @return The list of nodes defining the current state of the search.
     */
    const NodeList<State_t, Action_t>& getNodes() const { return m_nodes; }

    /**
     * Gets the action stack of the search. This is the rows of actions at each depth that were generated. The current
     * plan consists of one from each row.
     *
     * @return The action stack.
     */
    const std::vector<std::vector<Action_t>>& getActionStack() const { return m_action_stack; }

    /**
     * Gets the stack of indices indicating which action at each depth of the action stack corresponds to the current
     * plan.
     *
     * @return The action index stack.
     */
    const std::vector<unsigned>& getActionIndexStack() const { return m_action_index_stack; }

    /**
     * Gets the thresholds used for each of the iterations thus far.
     *
     * @return The IDA* thresholds thus far.
     */
    const std::vector<double>& getThresholds() const { return m_thresholds; }

    /**
     * Gets the threshold to use for the next iteration. A negative value means no threshold has been initialized yet.
     *
     * @return The threshold to use on the next iteration.
     */
    double getNextThreshold() const { return m_next_threshold; }

    // Overridden public SingleStepSearchEngine  methods
    StringMap getEngineSpecificStatistics() const override;
    std::vector<NodeEvaluator<State_t, Action_t>*> getBaseEvaluators() const override { return {m_evaluator}; }

    // Overidden public SettingsLogger methods
    std::string getName() const override { return "IDEngine"; }

protected:
    // Overridden SingleStepSearchEngine methods
    void doSearchInitialization(const State_t& initial_state) override;

    EngineStatus doSingleSearchStep() override;
    bool doCanRunSearch() const override { return m_evaluator != nullptr; }
    void doReset() override;
    StringMap getEngineParamsLog() const override { return m_params.getParameterLog(); }

    // Overidden protected SettingsLogger methods
    SearchSettingsMap getSubComponentSettings() const override;

    /**
     * Checks if the search can expand the node with the given ID or if the evaluation is too high.
     *
     * @param to_check The ID of the candidate to be check
     * @return Whether we can expand this node or not
     */
    bool canExpandNode(NodeID to_check) const { return !fpGreater(m_evaluator->getCachedEval(to_check), m_thresholds.back()); }

    /**
     * Expands the latest node and adds the new actions to the search stack. Also updates the corresponding search
     * statistics
     */
    void addNewActionsToStack();

    /**
     * Updates the action and current node stack so that the search can proceed. This may involve backtracking to find
     * the next node to expand, and updating the action and action index stacks as needed.
     *
     * There are two possible outcomes:
     * 1. The search backtracks all the way to the initial node. This corresponds to the case where an iteration has
     * ended. The action and action index stacks will be empty at this point
     * 2. The search backtracks to a point where the current path and action stacks are the same size. In this case,
     * the next node to be considered will be given by applying the topmost action in the stacks to the deepest node
     * on the current path.
     */
    void findNextToGenerate();

    /**
     * Generates the next node for the search. This next node is given by the the child of the current node
     * corresponding to the next action to take according to the action stack and action indices. Updates the statistics
     * accordingly as well.
     */
    void generateNextNode();

private:
    IDEngineParams m_params;  ///< The parameters of the IDEngine
    NodeEvaluator<State_t, Action_t>* m_evaluator = nullptr;  ///< The evaluation function

    NodeList<State_t, Action_t> m_nodes;  ///< The set of nodes which holds the current path being considered
    std::vector<std::vector<Action_t>> m_action_stack;  ///< The stack of actions to backtrack over
    std::vector<unsigned> m_action_index_stack;  ///< The index of the actions in the action stack that the current path coresponds to

    double m_next_threshold = -1.0;  ///< The current value of the threshold to use on the next iteration
    std::vector<double> m_thresholds;  ///< The list of thresholds use thus far
};

template<class State_t, class Action_t>
StringMap IDEngine<State_t, Action_t>::getEngineSpecificStatistics() const {
    StringMap stats = SingleStepSearchEngine<State_t, Action_t>::getEngineSpecificStatistics();
    stats["num_iterations"] = std::to_string(m_thresholds.size());

    return stats;
}

template<class State_t, class Action_t>
void IDEngine<State_t, Action_t>::setEvaluator(NodeEvaluator<State_t, Action_t>& evaluator) {
    m_evaluator = &evaluator;
    m_evaluator->setNodeContainer(m_nodes);
}

template<class State_t, class Action_t>
IDEngine<State_t, Action_t>::IDEngine(const IDEngineParams& params)
          : m_params(params) {
}

template<class State_t, class Action_t>
void IDEngine<State_t, Action_t>::doReset() {
    m_action_stack.clear();
    m_action_index_stack.clear();
    m_nodes.clear();

    m_next_threshold = -1.0;
    m_thresholds.clear();

    if (m_evaluator) {
        m_evaluator->reset();
    }
}

template<class State_t, class Action_t>
void IDEngine<State_t, Action_t>::doSearchInitialization(const State_t& initial_state) {
    // Puts initial state on current path
    m_nodes.addNode(initial_state);

    SE::evaluateNode(0);

    m_thresholds.emplace_back(m_evaluator->getLastNodeEval());  // sets the current threshold
}

template<class State_t, class Action_t>
EngineStatus IDEngine<State_t, Action_t>::doSingleSearchStep() {
    assert(SE::getStatus() == EngineStatus::active);

    NodeID current_id = m_nodes.size() - 1;

    if (m_evaluator->getCachedIsDeadEnd(current_id)) {  // Have hit dead end
        if (m_nodes.size() == 1) {  // If initial state is a dead end, then search is complete, otherwise will backtrack below
            return EngineStatus::search_completed;
        }
    } else if (!canExpandNode(current_id)) {  // Node does not satisfy the current threshold
        if (m_next_threshold < 0.0 || fpLess(m_evaluator->getCachedEval(current_id), m_next_threshold)) {  // Update next threshold if need to
            m_next_threshold = m_evaluator->getCachedEval(current_id);
        }
    } else {  // If can perform goal test

        if (SE::isGoal(m_nodes.getState(current_id))) {  // Perform goal test and extract if is is a goal
            SE::setIncumbentSolution(current_id, m_nodes);
            return EngineStatus::search_completed;
        } else {  // Is not goal, so generate actions if haven't hit limit
            addNewActionsToStack();
        }
    }

    findNextToGenerate();

    if (m_action_index_stack.empty()) {  // If backtracked to first node, start new iteration
        if (m_next_threshold == -1.0) {  // No nodes found outside the threshold, so have exhausted search space
            return EngineStatus::search_completed;
        } else {  // Initiate new iteration
            m_thresholds.emplace_back(m_next_threshold);
            m_next_threshold = -1.0;
        }
    } else {
        // Generate and evaluate next node if resource has not been hit
        generateNextNode();
    }

    SE::evaluateNode(m_nodes.size() - 1);

    return EngineStatus::active;
}

template<class State_t, class Action_t>
void IDEngine<State_t, Action_t>::addNewActionsToStack() {

    // Generates actions and update action stack
    m_action_stack.emplace_back(SE::getApplicableActions(m_nodes.getState(m_nodes.size() - 1)));

    // randomly reorder action list if need to
    if (m_action_stack.back().empty() && m_params.m_use_random_op_ordering) {
        randomlyReorderVector(m_action_stack.back(), *SE::getRandomNumGenerator().get());
    }
}

template<class State_t, class Action_t>
void IDEngine<State_t, Action_t>::findNextToGenerate() {
    if (m_action_index_stack.size() < m_action_stack.size()) {
        // This case corresponds to new actions being added to the action stack. In this case, we must add an index of 0
        // to the index stack so that the first action we look at is the first in the new level
        assert(m_action_index_stack.size() + 1 == m_action_stack.size());
        assert(m_nodes.size() == m_action_stack.size());

        m_action_index_stack.emplace_back(0);
    } else {
        // In this case, we do not have a new level of actions to consider first, and are instead backtracking
        assert(m_action_index_stack.size() == m_action_stack.size());
        assert(m_nodes.size() == m_action_stack.size() + 1);

        m_nodes.popBack();
        m_action_index_stack.back()++;  // moves to next action in current depth
    }

    assert(m_action_index_stack.size() == m_action_stack.size());
    assert(m_nodes.size() == m_action_index_stack.size());

    while (!m_action_index_stack.empty()) {  // Run until node is found (break below) or backtracked to start
        if (m_action_index_stack.back() >= m_action_stack.back().size()) {  // run out of actions at current depth, so backtrack
            if (m_nodes.size() > 1) {  // Don't pop off very first node, just stop there
                m_nodes.popBack();
            }
            m_action_index_stack.pop_back();
            m_action_stack.pop_back();
        } else {
            NodeID current_id = m_nodes.size() - 1;
            const Action_t& next_action = m_action_stack.back()[m_action_index_stack.back()];

            // Breaks if we have found a node to be next. Note that it keeps backtracking if this is a loop to the parent
            if (!m_params.m_use_parent_pruning || m_action_index_stack.size() <= 1 ||
                      !SE::isInverseAction(m_nodes.getState(current_id), next_action, m_nodes.getLastAction(current_id).value())) {
                break;
            }
        }

        // If there are more actions (ie. haven't backtracked to start), move to next action at current depth
        if (!m_action_index_stack.empty()) {
            m_action_index_stack.back()++;
        }
    }

    if (m_action_index_stack.empty()) {  // If backtracked to starting node
        assert(m_action_stack.empty());
        assert(m_nodes.size() == 1);
    } else {  // Did not backtrack all the way
        assert(m_action_index_stack.size() == m_action_stack.size());
        assert(m_nodes.size() == m_action_index_stack.size());
    }
}

template<class State_t, class Action_t>
void IDEngine<State_t, Action_t>::generateNextNode() {
    NodeID current_id = m_nodes.size() - 1;

    Action_t& next_action = m_action_stack.back()[m_action_index_stack.back()];
    double next_action_cost = SE::getActionCost(m_nodes.getState(current_id), next_action);
    double next_g = m_nodes.getGValue(current_id) + next_action_cost;

    m_nodes.addNode(SE::getChildState(m_nodes.getState(current_id), next_action), current_id, next_g, next_action, next_action_cost);
}

template<class State_t, class Action_t>
void IDEngine<State_t, Action_t>::setEngineParams(const IDEngineParams& params) {
    m_params = params;
    SE::reset();
}

template<class State_t, class Action_t>
SearchSettingsMap IDEngine<State_t, Action_t>::getSubComponentSettings() const {
    SearchSettingsMap sub_components;
    sub_components["eval_function"] = m_evaluator->getAllSettings();

    return sub_components;
}

#endif  //ID_ENGINE_H_
