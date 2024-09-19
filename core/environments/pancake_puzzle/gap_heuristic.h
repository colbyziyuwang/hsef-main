#ifndef GAP_HEURISTIC_H_
#define GAP_HEURISTIC_H_

#include "building_tools/evaluators/cost_and_distance_to_go_evaluator.h"
#include "building_tools/evaluators/node_evaluator_with_cache.h"
#include "logging/logging_terms.h"
#include "logging/search_component_settings.h"
#include "pancake_action.h"
#include "pancake_state.h"
#include "pancake_transitions.h"
#include "search_basics/node_container.h"
#include "search_basics/node_evaluator.h"

#include <string>
#include <vector>

/**
 * Defines the gap heuristic.
 * h^G(v) = | {j | 1 <= j <= N, |v[j] - v[j + 1]| > 1} |
 * Where v is an extended permutation of the original pancake puzzle state permutation p.
 * An extended permutation of a permutation p is p but with N + 1 appended
 * to the end of the permutation. N is the integer representing the largest pancake in p.
 *
 * @class GapHeuristic
 */
class GapHeuristic : public NodeEvaluatorWithCache<PancakeState, NumToFlip>,
                     virtual public CostAndDistanceToGoEvaluator<PancakeState, NumToFlip> {
public:
    inline static const std::string CLASS_NAME = "GapHeuristic";  ///< The name of the class. Defines this component's name
    /**
     * Constructs a gap heuristic.
     *
     * @param cost_type The cost type to use
     */
    explicit GapHeuristic(PancakePuzzleCostType cost_type = PancakePuzzleCostType::unit);

    /**
     * Default destructor.
     */
    ~GapHeuristic() override = default;

    // Overriden public NodeEvaluator functions
    std::vector<NodeEvaluator<PancakeState, NumToFlip>*> getSubEvaluators() const override { return {}; }

    // Overriden public DistanceToGoEvaluation functions
    double getLastDistanceToGoEval() const override;
    double getCachedDistanceToGoEval(NodeID node_id) const override;
    void setCachedDistanceToGoEval(NodeID node_id, double eval) override;

    // Overriden public SettingsLogger methods
    std::string getName() const override { return CLASS_NAME; }

protected:
    // Overriden protected SettingsLogger methods
    StringMap getComponentSettings() const override;
    SearchSettingsMap getSubComponentSettings() const override { return {}; }

private:
    // Overriden private NodeEvaluateWithStorage functions
    void doPrepare() override {}
    void doEvaluateAndCache(NodeID to_evaluate) override;
    void doReEvaluateAndCache(NodeID /* to_evaluate */) override {}
    void doReset() override {}

    PancakePuzzleCostType m_cost_type;  ///< The cost type to use
    std::vector<double> m_distance_to_go_evals;  ///< The cached distance-to-go estimates of all nodes
};

#endif /* GAP_HEURISTIC_H_ */
