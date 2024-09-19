#ifndef GRID_PATHFINDING_OCTILE_HEURISTIC_H_
#define GRID_PATHFINDING_OCTILE_HEURISTIC_H_

#include "building_tools/evaluators/cost_and_distance_to_go_evaluator.h"
#include "building_tools/evaluators/node_evaluator_with_cache.h"
#include "building_tools/evaluators/single_goal_state_evaluator.h"
#include "grid_location.h"
#include "grid_pathfinding_action.h"
#include "logging/logging_terms.h"
#include "logging/search_component_settings.h"
#include "search_basics/node_container.h"
#include "search_basics/node_evaluator.h"
#include "utils/floating_point_utils.h"

#include <string>
#include <vector>

/**
 * A class that defines the octile heuristic function for a grid map pathfinding domain.
 *
 * @class GridPathfindingOctileHeuristic
 */
class GridPathfindingOctileHeuristic
          : public NodeEvaluatorWithCache<GridLocation, GridDirection>,
            virtual public CostAndDistanceToGoEvaluator<GridLocation, GridDirection>,
            public SingleGoalStateEvaluator<GridLocation> {
public:
    inline static const std::string CLASS_NAME = "GridPathfindingOctileHeuristic";  ///< The name of the class. Defines this component's name
    /**
     * Creates an octile distance heuristic based on the given goal state.
     *
     * @param goal_state The goal state.
     * @param diag_cost The diagonal cost to use
     */
    explicit GridPathfindingOctileHeuristic(GridLocation goal_state, double diag_cost = ROOT_TWO);

    /**
     * Creates an octile distance heuristic based on the given coordiates for the goal state.
     *
     * @param x_coord The x coordinate of the goal state
     * @param y_coord The y coordinate of the goal state
     * @param diag_cost The diagonal cost to use
     */
    explicit GridPathfindingOctileHeuristic(int x_coord = 0, int y_coord = 0, double diag_cost = ROOT_TWO);

    /**
     * Default destructor.
     */
    ~GridPathfindingOctileHeuristic() override = default;

    /**
     * Sets the goal to use the given coordinates.
     *
     * @param x_coord The x coordinate of the goal.
     * @param y_coord The y coordinate of the goal.
     */
    void setGoalState(int x_coord, int y_coord);

    /**
     * Sets the cost of a diagonal move.
     *
     * Value must be larger than 0.0. False is returned if it is not.
     *
     * @param diag_cost The new diagonal move cost.
     * @return If the diagonal move cost was successfully set;
     */
    bool setDiagonalCost(double diag_cost);

    // Overriden public NodeEvaluator functions
    std::vector<NodeEvaluator<GridLocation, GridDirection>*> getSubEvaluators() const override { return {}; }

    // Overriden public DistanceToGoEvaluation functions
    double getLastDistanceToGoEval() const override;
    double getCachedDistanceToGoEval(NodeID node_id) const override;
    void setCachedDistanceToGoEval(NodeID node_id, double eval) override;

    // Overriden SingleGoalStateComponent functions
    void setGoalState(const GridLocation& goal_state) override;
    GridLocation getGoalState() const override;

    // Overriden public SettingsLogger methods
    std::string getName() const override { return CLASS_NAME; }

protected:
    // Overriden private NodeEvaluateWithStorage functions
    void doPrepare() override {}
    void doEvaluateAndCache(NodeID to_evaluate) override;
    void doReEvaluateAndCache(NodeID /* to_evaluate */) override {}
    void doReset() override {}

    // Overriden protected SettingsLogger methods
    StringMap getComponentSettings() const override;
    SearchSettingsMap getSubComponentSettings() const override { return {}; }

private:
    GridLocation m_goal_state;  ///< The single goal state
    double m_diag_cost = ROOT_TWO;  ///< The cost of diagonal moves.
    std::vector<double> m_distance_to_go_evals;  ///< The cached distance-to-go estimates of all nodes
};

#endif /* GRID_PATHFINDING_OCTILE_HEURISTIC_H_ */
