#ifndef SLIDING_TILE_UTILS_H_
#define SLIDING_TILE_UTILS_H_

#include "sliding_tile_state.h"
#include "sliding_tile_transitions.h"

#include <ostream>
#include <string>
#include <vector>

/**
 * Gets the tile move costs for the puzzle size and cost type. This is returned as a vector where tile i appears in
 * the ith index.
 *
 * @param puzzle_size The number of tiles in the puzzle (blank included)
 * @param cost_type The cost type
 * @return A vector of tile move costs
 */
std::vector<double> getTileMoveCosts(int puzzle_size, SlidingTileCostType cost_type);

/**
 * Outputs string representation of the cost_type.
 *
 * @param out The output stream
 * @param cost_type The cost type
 * @return The output stream
 */
std::ostream& operator<<(std::ostream& out, const SlidingTileCostType& cost_type);

/**
 * Reads in a list of sliding tile states from a file.
 *
 * @param filename The filename to read in from
 * @param num_rows The number of rows in the puzzle
 * @param num_cols The number of columns in the puzzle
 * @return The list of sliding tile puzzle states in the file
 */
std::vector<SlidingTileState> readSlidingTileStatesFromFile(const std::string& filename, int num_rows, int num_cols);

#endif  //SLIDING_TILE_UTILS_H_
