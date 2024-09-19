#ifndef GRID_LOCATION_HASH_FUNCTION_H_
#define GRID_LOCATION_HASH_FUNCTION_H_

#include "building_tools/hashing/state_hash_function.h"
#include "grid_location.h"
#include "grid_pathfinding_transitions.h"
#include "logging/logging_terms.h"
#include "logging/search_component_settings.h"

#include <cstdint>
#include <string>
#include <vector>

/**
 * A hash function for grid location that is based on grid width (or assumes it is less than 2^16).
 */
class GridLocationHashFunction : public StateHashFunction<GridLocation, uint32_t> {
public:
    inline static const std::string CLASS_NAME = "GridLocationHashFunction";  ///< The name of the class. Defines this component's name

    /**
     * Default destructor.
     */
    ~GridLocationHashFunction() override = default;

    /**
     * Sets the grid width to be the given value. Used to make it a "tighter" hash function. Uses 2^16 for the width
     * if it is set to 0.
     *
     * @param map_width The width of the grid map.
     */
    void setMapWidth(uint32_t map_width);

    /**
     * Sets the map width to be the given value. Used to make it a "tighter" hash function.
     *
     * @param transitions The transitions with the map.
     */
    void setMapWidth(const GridPathfindingTransitions& transitions);

    uint32_t getHashValue(const GridLocation& state) const override;
    bool isPerfectHashFunction() const override { return true; }

    // Overriden public SettingsLogger methods
    std::string getName() const override { return CLASS_NAME; }

protected:
    // Overriden protected SettingsLogger methods
    StringMap getComponentSettings() const override;
    SearchSettingsMap getSubComponentSettings() const override { return {}; }

private:
    uint32_t m_first_dimension_size = 65536;  ///< The width of the map. Default is 65536.
};


#endif  //GRID_LOCATION_HASH_FUNCTION_H_
