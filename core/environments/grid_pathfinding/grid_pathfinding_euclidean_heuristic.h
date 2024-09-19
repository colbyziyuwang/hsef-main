#ifndef GRID_PATHFINDING_EUCLIDEAN_HEURISTIC_H_
#define GRID_PATHFINDING_EUCLIDEAN_HEURISTIC_H_

#include "building_tools/evaluators/node_evaluator_with_cache.h"
#include "building_tools/evaluators/single_goal_state_evaluator.h"
#include "grid_location.h"
#include "grid_pathfinding_action.h"
#include "search_basics/node_container.h"
#include "search_basics/node_evaluator.h"

#include "logging/logging_terms.h"
#include "logging/search_component_settings.h"
#include <string>
#include <vector>

/**
 * A class that defines the Euclidean heuristic function for a grid map pathfinding domain.
 *
 * @class GridPathfindingEuclideanHeuristic
 */
class GridPathfindingEuclideanHeuristic
          : public NodeEvaluatorWithCache<GridLocation, GridDirection>,
            public SingleGoalStateEvaluator<GridLocation> {
public:
    inline static const std::string CLASS_NAME = "GridPathfindingEuclideanHeuristic";  ///< The name of the class. Defines this component's name
    /**
     * Creates a Euclidean distance heuristic based on the given goal state.
     *
     * @param goal_state The goal state.
     */
    explicit GridPathfindingEuclideanHeuristic(GridLocation goal_state);

    /**
     * Creates a Euclidean distance heuristic based on the given coordiates for the goal state.
     *
     * @param x_coord The x coordinate of the goal state
     * @param y_coord The y coordinate of the goal state
     */
    explicit GridPathfindingEuclideanHeuristic(int x_coord = 0, int y_coord = 0);

    /**
     * Default destructor.
     */
    ~GridPathfindingEuclideanHeuristic() override = default;

    /**
     * Sets the goal to use the given coordinates.
     *
     * @param x_coord The x coordinate of the goal.
     * @param y_coord The y coordinate of the goal.
     */
    void setGoalState(int x_coord, int y_coord);

    // Overriden public NodeEvaluator functions
    std::vector<NodeEvaluator<GridLocation, GridDirection>*> getSubEvaluators() const override { return {}; }

    // Overriden SingleGoalStateComponent functions
    void setGoalState(const GridLocation& goal_state) override;
    GridLocation getGoalState() const override;

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

    GridLocation m_goal_state;  ///< The single goal state
};

#endif /* GRID_PATHFINDING_EUCLIDEAN_HEURISTIC_H_ */
