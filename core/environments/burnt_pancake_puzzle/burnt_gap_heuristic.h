#ifndef BURNT_GAP_HEURISTIC_H_
#define BURNT_GAP_HEURISTIC_H_

#include "building_tools/evaluators/cost_and_distance_to_go_evaluator.h"
#include "building_tools/evaluators/node_evaluator_with_cache.h"
#include "burnt_pancake_state.h"
#include "logging/logging_terms.h"
#include "logging/search_component_settings.h"
#include "search_basics/node_container.h"
#include "search_basics/node_evaluator.h"

#include "environments/pancake_puzzle/pancake_action.h"
#include "environments/pancake_puzzle/pancake_transitions.h"

#include <string>
#include <vector>

/**
 * Defines the gap heuristic.
 * h^G(v) = | {j | 1 <= j <= N, |v[j] - v[j + 1]| > 1} |
 * Where v is an extended permutation of the original pancake puzzle state permutation p.
 * An extended permutation of a permutation p is p but with N + 1 appended
 * to the end of the permutation. N is the integer representing the largest pancake in p.
 *
 * @class BurntGapHeuristic
 */
class BurntGapHeuristic : public NodeEvaluatorWithCache<BurntPancakeState, NumToFlip>,
                          virtual public CostAndDistanceToGoEvaluator<BurntPancakeState, NumToFlip> {
public:
    inline static const std::string CLASS_NAME = "BurntGapHeuristic";  ///< The name of the class. Defines this component's name
    /**
     * Constructs a gap heuristic
     */
    explicit BurntGapHeuristic(PancakePuzzleCostType cost_type = PancakePuzzleCostType::unit);

    /**
     * Default destructor.
     */
    ~BurntGapHeuristic() override = default;

    // Overriden public NodeEvaluator functions
    std::vector<NodeEvaluator<BurntPancakeState, NumToFlip>*> getSubEvaluators() const override { return {}; }

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
    // Overriden private NodeEvaluatorWithStorage functions
    void doPrepare() override {}
    void doEvaluateAndCache(NodeID to_evaluate) override;
    void doReEvaluateAndCache(NodeID /* to_evaluate */) override {}
    void doReset() override {}

    PancakePuzzleCostType m_cost_type;  ///< The cost type to use
    std::vector<double> m_distance_to_go_evals;  ///< The cached distance-to-go estimates of all nodes
};

#endif /* BURNT_GAP_HEURISTIC_H_ */
