/**
 * Utilities for maps.
 *
 * @file grid_pathfinding_utils.h
 */

#ifndef GRID_PATHFINDING_UTILS_H_
#define GRID_PATHFINDING_UTILS_H_

#include "grid_location.h"
#include "grid_pathfinding_action.h"
#include "grid_pathfinding_transitions.h"

#include <istream>
#include <ostream>
#include <vector>

const std::vector<GridDirection> GRID_PATHFINDING_CARDINAL_ACTIONS{
          GridDirection::north,
          GridDirection::east,
          GridDirection::south,
          GridDirection::west};  ///< A list of all cardinal actions

const std::vector<GridDirection> GRID_PATHFINDING_DIAGONAL_ACTIONS{
          GridDirection::northeast,
          GridDirection::southeast,
          GridDirection::southwest,
          GridDirection::northwest};  ///< A list of all diagonal actions

const std::vector<GridDirection> GRID_PATHFINDING_ALL_ACTIONS{
          GridDirection::north,
          GridDirection::northeast,
          GridDirection::east,
          GridDirection::southeast,
          GridDirection::south,
          GridDirection::southwest,
          GridDirection::west,
          GridDirection::northwest};  ///< A list of all actions

/**
 * Outputs string representation of a cost type.
 *
 * @param out The output stream
 * @param cost_type The cost type
 * @return The output stream
 */
std::ostream& operator<<(std::ostream& out, const GridPathfindingCostType& cost_type);

/**
 * Outputs a string representation of the connection type to a stream.
 *
 * @param out The output stream
 * @param connection_type The connection type
 * @return The output stream
 */
std::ostream& operator<<(std::ostream& out, const GridConnectionType& connection_type);

/**
 * Reads in the pathfinding problems from the given file.
 *
 * The input format is assumed to be as follows. Each line represents a different problem, and each
 * is defined by 4 integers. The first two are the x and y coordinates of the start state. The last
 * two are the x and y coordinates of the goal state.
 *
 * @todo Add more extensive error checking.
 *
 * @param file_stream The std::istream object representing the file to read from.
 * @param starts The vector in which to store the start states.
 * @param goals The vector in which to store the goal states.
 * @return If the read was successful or not.
 */
bool readInGridPathfindingProblems(
          std::istream& file_stream, std::vector<GridLocation>& starts, std::vector<GridLocation>& goals);

#endif /* GRID_PATHFINDING_UTILS_H_ */
