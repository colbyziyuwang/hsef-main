#ifndef NON_GOAL_HEURISTIC_H_
#define NON_GOAL_HEURISTIC_H_

#include "building_tools/evaluators/node_evaluator_with_cache.h"
#include "evaluator_tools_terms.h"
#include "logging/logging_terms.h"
#include "logging/search_component_settings.h"
#include "search_basics/goal_test.h"
#include "search_basics/node_container.h"
#include "search_basics/node_evaluator.h"
#include "utils/string_utils.h"

#include <string>
#include <vector>

/**
 * Defines a heuristic that returns 0 for any goal state, and the minimum action cost for any other state.
 *
 * @class NonGoalHeuristic
 */
template<class State_t, class Action_t>
class NonGoalHeuristic : public NodeEvaluatorWithCache<State_t, Action_t> {

public:
    inline static const std::string CLASS_NAME = "NonGoalHeuristic";  ///< The name of the heuristic when a goal test is assigned

    /**
     * Constructs a non-goal heuristic for the given goal test function and minimum action cost.
     *
     * @param goal_test The goal test function.
     * @param default_h_value The h-value of any non-goal node (default is set to 1)
     */
    explicit NonGoalHeuristic(const GoalTest<State_t>& goal_test, double default_h_value = 1);

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
    const GoalTest<State_t>* m_goal_test;  ///< The stored goal test function.
    double m_default_h_value;  ///< The default h-value of any non-goal node
};

template<class State_t, class Action_t>
NonGoalHeuristic<State_t, Action_t>::NonGoalHeuristic(const GoalTest<State_t>& goal_test, double default_h_value)
          : m_goal_test(&goal_test), m_default_h_value(default_h_value) {
}

template<class State_t, class Action_t>
void NonGoalHeuristic<State_t, Action_t>::doEvaluateAndCache(NodeID to_evaluate) {
    using NE = NodeEvaluatorWithCache<State_t, Action_t>;

    if (m_goal_test->isGoal(NE::getNodeContainer()->getState(to_evaluate))) {
        NE::setCachedValues(to_evaluate, 0, false);
    } else {
        NE::setCachedValues(to_evaluate, m_default_h_value, false);
    }
}

template<class State_t, class Action_t>
StringMap NonGoalHeuristic<State_t, Action_t>::getComponentSettings() const {
    using namespace evaluatorToolsTerms;
    return {{SETTING_DEFAULT_H_VALUE, roundAndToString(m_default_h_value, 6)}};
}

template<class State_t, class Action_t>
SearchSettingsMap NonGoalHeuristic<State_t, Action_t>::getSubComponentSettings() const {
    SearchSettingsMap sub_components;
    sub_components[evaluatorToolsTerms::SETTING_GOAL_TEST] = m_goal_test->getAllSettings();
    return sub_components;
}

#endif /* NON_GOAL_HEURISTIC_H_ */
