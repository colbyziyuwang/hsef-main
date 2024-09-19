#ifndef CONSTANT_HEURISTIC_H_
#define CONSTANT_HEURISTIC_H_

#include "building_tools/evaluators/node_evaluator_with_cache.h"
#include "evaluator_tools_terms.h"
#include "logging/logging_terms.h"
#include "logging/search_component_settings.h"
#include "search_basics/node_container.h"
#include "search_basics/node_evaluator.h"
#include "utils/string_utils.h"

#include <string>
#include <vector>

/**
 * Defines a heuristic that returns a given constant value for any state.
 *
 * @class ConstantHeuristic
 */
template<class State_t, class Action_t>
class ConstantHeuristic : public NodeEvaluatorWithCache<State_t, Action_t> {

public:
    inline static const std::string CLASS_NAME = "ConstantHeuristic";  ///< The name of the heuristic when a constant is returned

    /**
     * Constructs a constant heuristic.
     *
     * @param default_h_value The h-value of any node. Default is 0.
     */
    explicit ConstantHeuristic(double default_h_value = 0)
              : m_default_h_value(default_h_value) {}

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
    SearchSettingsMap getSubComponentSettings() const override { return {}; }

private:
    double m_default_h_value;  ///< The default h-value of any non-goal node
};


template<class State_t, class Action_t>
void ConstantHeuristic<State_t, Action_t>::doEvaluateAndCache(NodeID to_evaluate) {
    using NE = NodeEvaluatorWithCache<State_t, Action_t>;
    NE::setCachedValues(to_evaluate, m_default_h_value, false);
}

template<class State_t, class Action_t>
StringMap ConstantHeuristic<State_t, Action_t>::getComponentSettings() const {
    return {{evaluatorToolsTerms::SETTING_DEFAULT_H_VALUE, roundAndToString(m_default_h_value, 6)}};
}
#endif  //CONSTANT_HEURISTIC_H_
