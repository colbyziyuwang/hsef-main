#ifndef GRID_PATHFINDING_TRANSITIONS_H_
#define GRID_PATHFINDING_TRANSITIONS_H_

#include "grid_location.h"
#include "grid_map.h"
#include "grid_pathfinding_action.h"
#include "logging/logging_terms.h"
#include "logging/search_component_settings.h"
#include "search_basics/transition_system.h"
#include "utils/floating_point_utils.h"

#include <cstdint>
#include <optional>
#include <string>
#include <vector>

/**
 * Defines the possible connections in the 2D grid pathfinding domain.
 *
 * @class GridDirection
 */
enum class GridConnectionType : std::uint8_t {
    four,
    eight
};
/**
 * Defines the possible cost types for grid pathfinding.
 *
 * Below is the meaning of each of the values:
 *
 * unit - all actions have a cost of 1.
 * life - moving a within row r has a cost of r (assuming top row is 0).
 *
 * @class GridPathfindingCostType
 */
enum class GridPathfindingCostType : std::uint8_t {
    standard,
    life
};

/**
 * Defines the transitions for 2D grid pathfinding. Based the transition system on a given grid map.
 *
 * The origin (0,0) is set as the upper left corner in keeping with benchmark standard.
 * A location (x, y) means y rows from the top and x columns from the left.
 * Locations are either empty or have an obstacle. If the transition system is set as 4-connected,
 * then movement is only possible in the 4 cardinal directions (North, East, South, and West). If
 * it is not 4-connected, it is 8-connected, which means it is possible to move in the 4 cardinal
 * directions as well as Northeast, Southeast, Southwest, and Northwest.
 *
 * @class GridPathfindingTransitions
 */
class GridPathfindingTransitions : public TransitionSystem<GridLocation, GridDirection> {
public:
    inline static const std::string CLASS_NAME = "GridPathfindingTransitions";  ///< The name of this class

    /**
     * A constructor for a 2D grid pathfinding transition system for the given grid map.
     *
     * @param grid_map The map to use for grid pathfinding
     * @param connection_type The system's connection type. Set to four (4-connected) by default.
     * @param cost_type The type of cost function to use
     */
    explicit GridPathfindingTransitions(
              const GridMap* grid_map, GridConnectionType connection_type = GridConnectionType::four,
              GridPathfindingCostType cost_type = GridPathfindingCostType::standard);

    /**
     * Default destructor.
     */
    ~GridPathfindingTransitions() override = default;

    // Overriden public TransitionSystem methods
    std::vector<GridDirection> getActions(const GridLocation& state) const override;
    bool isApplicable(const GridLocation& state, const GridDirection& action) const override;
    double getActionCost(const GridLocation& state, const GridDirection& action) const override;
    void applyAction(GridLocation& state, const GridDirection& action) const override;
    std::optional<GridDirection> getInverse(const GridLocation& state, const GridDirection& action) const override;
    bool isValidState(const GridLocation& state) const override;

    /**
     * Updates the grid map to the newly given one.
     *
     * @param grid_map The new grid_map to use
     */
    void setGridMap(const GridMap* grid_map);

    /**
     * Returns the width of the grid map.
     *
     * @return The width of the grid map.
     */
    int getMapWidth() const;

    /**
     * Returns the height of the grid map.
     *
     * @return The height of the grid map.
     */
    int getMapHeight() const;

    /**
    * Sets the connection type of the grid
    *
    * @param connection_type The new conection type of the grid
    */
    void setConnectionType(GridConnectionType connection_type);

    /**
    * Returns the grid map connection type
    *
    * @return The grid map's connection type
    */
    GridConnectionType getConnectionType() const;

    /**
     * Sets the cost of diagonal move (ie. northeast, southeast, southwest, or northwest). 
     * By default, set as square root two, as defined in floating_point_utils.h.
     *
     * Ignores the given value if it is non-positive.
     *
     * @param diag_cost The cost of a diagonal move.
     * @return If the diagonal value was set properly.
     */
    bool setDiagonalCost(double diag_cost);

    /**
     * Sets the action costs to one of the standard types.
     *
     * @param cost_type The cost type for the transitions.
     */
    void setCostType(GridPathfindingCostType cost_type);

    // Overriden public SettingsLogger methods
    std::string getName() const override { return CLASS_NAME; }

protected:
    // Overriden protected SettingsLogger methods
    StringMap getComponentSettings() const override;
    SearchSettingsMap getSubComponentSettings() const override { return {}; }

private:
    const GridMap* m_grid_map;  ///< The grid map to generate transitions for
    GridPathfindingCostType m_cost_type;  ///< The cost type per action.
    GridConnectionType m_connection_type;  ///< The connection type of the grid map

    double m_diag_cost = ROOT_TWO;  ///< The cost of a diagonal move.
};

#endif /* GRID_PATHFINDING_TRANSITIONS_H_ */
