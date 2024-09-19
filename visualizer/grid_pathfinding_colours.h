#ifndef GRID_PATHFINDING_VISUALIZATION_NAMES_H_
#define GRID_PATHFINDING_VISUALIZATION_NAMES_H_

#include <ostream>
#include <string>

#include "grid_pathfinding.h"

/**
 * Names for grid color name
 */
namespace GridPathfindingColours {
    const sf::Color PASSABLE_COLOR = sf::Color::White;
    const sf::Color OBSTACLE_COLOR = sf::Color(105, 105, 105);
    const sf::Color TREE_COLOR = sf::Color::Green;
    const sf::Color SWAMP_COLOR = sf::Color(139, 69, 19);
    const sf::Color WATER_COLOR = sf::Color::Blue;
    const sf::Color OUTSIDE_GRID_COLOR = sf::Color::Red;
    const sf::Color START_STATE_COLOR = sf::Color(255, 213, 128);
    const sf::Color GOAL_STATE_COLOR = sf::Color(255, 114, 118);
}  // namespace GridPathfindingColours

#endif  //GRID_PATHFINDING_VISUALIZATION_NAMES_H_