#ifndef SLIDING_TILE_MANHATTAN_HEURISTIC_H_
#define SLIDING_TILE_MANHATTAN_HEURISTIC_H_

#include "building_tools/evaluators/cost_and_distance_to_go_evaluator.h"
#include "building_tools/evaluators/node_evaluator_with_cache.h"
#include "building_tools/evaluators/single_goal_state_evaluator.h"
#include "logging/logging_terms.h"
#include "logging/search_component_settings.h"
#include "search_basics/node_container.h"
#include "search_basics/node_evaluator.h"
#include "sliding_tile_action.h"
#include "sliding_tile_state.h"
#include "sliding_tile_transitions.h"

#include <string>
#include <vector>

/**
 * A heuristic function for the sliding tile puzzle based on Manhattan distance. Allows for different cost types
 * as well.
 */
class SlidingTileManhattanHeuristic
          : public NodeEvaluatorWithCache<SlidingTileState, BlankSlide>,
            virtual public CostAndDistanceToGoEvaluator<SlidingTileState, BlankSlide>,
            public SingleGoalStateEvaluator<SlidingTileState> {

public:
    inline static const std::string CLASS_NAME = "SlidingTileManhattanHeuristic";  ///< The name of the class. Defines this component's name

    /**
     * Constructor that stores the given goal and tile move costs.
     *
     * @param goal_state The goal state.
     * @param cost_type The cost type to use during heuristic computation
     */
    SlidingTileManhattanHeuristic(const SlidingTileState& goal_state, SlidingTileCostType cost_type);

    /**
     * Default destructor.
     */
    ~SlidingTileManhattanHeuristic() override = default;

    /**
     * Checks if the given state is valid for this evaluator. Intended for debugging purposes.
     *
     * @param state The state to check
     * @return If the given state is valid.
     */
    bool isValidState(const SlidingTileState& state) const;

    /**
     * Sets the cost type to use for the heuristic computation.
     *
     * @param cost_type The cost type to use
     */
    void setCostType(SlidingTileCostType cost_type);

    // Overriden public NodeEvaluator functions
    std::vector<NodeEvaluator<SlidingTileState, BlankSlide>*> getSubEvaluators() const override { return {}; }

    // Overriden public DistanceToGoEvaluation functions
    double getLastDistanceToGoEval() const override;
    double getCachedDistanceToGoEval(NodeID node_id) const override;
    void setCachedDistanceToGoEval(NodeID node_id, double eval) override;

    // Overriden SingleGoalStateComponent functions
    void setGoalState(const SlidingTileState& goal_state) override;
    SlidingTileState getGoalState() const override;

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

    /**
     * Updates the information cached for quick heuristic computation.
     */
    void updateHeuristicCache();

    SlidingTileState m_goal_state;  ///< The single goal state
    int m_num_rows;  ///< The number of rows in the puzzle.
    int m_num_cols;  ///< The number of columns in the puzzle.
    int m_puzzle_size;  ///< The total number of locations in the puzzle.

    SlidingTileCostType m_cost_type;  ///< The cost type used

    std::vector<std::vector<double>> m_tile_h_value;  ///< The heuristic impact of the current tile in the current position. The first index (for the blank) is unused.
    std::vector<std::vector<double>> m_tile_distance_to_go;  ///< The distance-to-go of the current tile in the current position. The first index (for the blank) is unused.
    std::vector<double> m_distance_to_go_evals;  ///< The cached distance-to-go estimates of all nodes
};

#endif /* SLIDING_TILE_MANHATTAN_HEURISTIC_H_ */